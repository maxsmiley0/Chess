//
//  searcher.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/25/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "searcher.h"

Searcher::Searcher()
{
    moveGenerator = new Movegen();
}

Searcher::~Searcher()
{
    delete moveGenerator;
}

int Searcher::reccomendMove()
{
    //if (!ponderMode)
    //{
        prepSearch(); //If we're pondering, we don't clear the table, as it is filled while pondering. We need these values for the pondering time to be of use
    //}
    
    //If ponder hit somehow join the results
    //This is the score of the position considering a given depth
    float score = 0;
    
    //Iterative deepening
    for (; true; searchDepth++)
    {
        int tempScore = alphaBeta(-INFINITY, INFINITY, searchDepth);
        
        //Only update the score if we weren't stopped due to time... because this always returns 0 by default
        if (!stop && searchDepth <= MAXDEPTH)
        {
            score = tempScore;
        }
        //If we are stopped or somehow exceed MAXDEPTH, exit loop
        else
        {
            break;
        }
    }
    
    if (debugMode)
    {
        std::cout << "Nodes Searched: " << stat.nodes << std::endl;
        std::cout << "Search Speed: " << (stat.nodes / timeAllocated) << " kN/s" << std::endl;
        std::cout << "Move Ordering: " << (100 * stat.failHighFirst / (stat.failHigh + 1)) << '%' <<  std::endl;
        std::cout << "Depth: " << (searchDepth - 1) << std::endl;
        printPvLine(searchDepth - 1);
        if (getBoard()->getSide() == BLACK){score *= -1;}
        std::cout << "Score: " << score / 100 << std::endl;
    }
    
    return getPvMove();
}

int Searcher::alphaBeta (int alpha, int beta, int depth)
{
    int movesMade = 0;
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    
    //Called at leaf nodes
    if (depth <= 0 || depth >= MAXDEPTH)
    {
        return quiescenceSearch(alpha, beta);
    }
    
    if ((stat.nodes & 2047) == 0)
    {
        checkTime();
    }
    
    //Statistics collection
    stat.nodes++;
    
    //Check for 50 move violation or 3 fold repetition
    if (getBoard()->numRep() >= 2 || (getBoard()->getHisPly() - getBoard()->getFiftyMove() >= 100))
    {
        return 0;
    }
    
    int side = getBoard()->getSide();
    int kingR = getBoard()->getKingR(side);
    int kingC = getBoard()->getKingC(side);
    
    bool inCheck = moveGenerator->squareAttacked(kingR, kingC, side);
    //Search extension, allocate extra ply if in check
    if (inCheck)
    {
        depth++;
    }
    
    //Generate moves
    std::list<int> moveList = moveGenerator->generateMoves();
    
    while (!moveList.empty())
    {
        int nextMove = pickNextMove(moveList, depth);
        //Evaluate position by calling alpha beta recursively
        if (moveGenerator->makeMove(nextMove))
        {
            movesMade++;
            int moveScore = -alphaBeta(-beta, -alpha, depth - 1);
            moveGenerator->takeBack();
            
            if (stop)
            {
                return 0;
            }
            
            if (moveScore > alpha)
            {
                if (moveScore >= beta)
                {
                    //Statistics collection purposes
                    if (movesMade == 1)
                    {
                        stat.failHighFirst++;
                    }
                    stat.failHigh++;
                    
                    //Storing killer move
                    if (captured(nextMove) == NOPIECE)
                    {
                        storeKillerMove(nextMove, depth);
                        storeHistoryMove(nextMove, depth * depth);
                    }
                    
                    return beta;
                }
                else
                {
                    alpha = moveScore;
                    bestMove = nextMove;
                }
            }
        }
    }
    if (movesMade == 0)
    {
        if (inCheck)
        {
            /*
             Subtract depth so mates found at lower depths are favored over higher depths
             So the computer will treat a mate in 6 as more favorable than a mate in 2
             */
            return -MATE - depth;
        }
        else
        {
            return 0;
        }
    }
    
    if (alpha != oldAlpha)
    {
        storePvMove(bestMove);
    }
    
    return alpha;
}

int Searcher::quiescenceSearch(int alpha, int beta)
{
    if ((stat.nodes & 2047) == 0)
    {
        checkTime();
    }
    
    //Statistics collection
    stat.nodes++;
    
    //Check for 50 move violation or 3 fold repetition
    if (getBoard()->numRep() >= 2 || (getBoard()->getHisPly() - getBoard()->getFiftyMove() >= 100))
    {
        return 0;
    }
    
    int score = static_eval(*getBoard());
    int movesMade = 0;
    int bestMove = NOMOVE;
    
    //Standing pat
    if (score >= beta)
    {
        return beta;
    }
    if (score >= alpha)
    {
        alpha = score;
    }
    
    //Generate captures
    std::list<int> captureList = moveGenerator->generateCaptures();
    
    while (!captureList.empty())
    {
        int nextMove = pickNextMove(captureList);
        //Evaluate captures, recursively call quiescence search
        if (moveGenerator->makeMove(nextMove))
        {
            movesMade++;
            int moveScore = -quiescenceSearch(-beta, -alpha);
            moveGenerator->takeBack();
            
            if (stop)
            {
                return 0;
            }
            
            if (moveScore > alpha)
            {
                if (moveScore >= beta)
                {
                    //Statistics collection
                    if (movesMade == 1)
                    {
                        stat.failHighFirst++;
                    }
                    stat.failHigh++;
                    
                    return beta;
                }
                else
                {
                    alpha = moveScore;
                    bestMove = nextMove;
                }
            }
        }
    }
    return alpha;
}

void Searcher::ponder()
{
    //Get info about PV's here...
    //std::cout << "Recommended move: " << printMove(getPvMove()) << std::endl;
    
    prepSearch();
    
    float* score = new float;   //To print out position score for debug info
    timeAllocated += 1000000000;//Effectively infinite time to ponder (until interrupt)
    
    //Iterative deepening
    for (; true; searchDepth++)
    {
        score[searchDepth] = alphaBeta(-INFINITY, INFINITY, searchDepth);
        std::cout << "reached depth " << searchDepth << std::endl;
        
        if (stop)
        {
            break;
        }
    }
    timeAllocated -= 1000000000;//Restore to its default
    
    delete score;
}

int Searcher::pickNextMove(std::list<int>& li, int depth)
{
    int bestMove = NOMOVE;
    std::list<int>::iterator toBeDeleted;
    
    for (std::list<int>::iterator itr = li.begin(); itr != li.end(); itr++)
    {
        if (bestMove == NOMOVE || movePriority(*itr, depth) > movePriority(bestMove, depth))
        {
            bestMove = *itr;
            toBeDeleted = itr;
        }
    }
    
    toBeDeleted = li.erase(toBeDeleted);
    return bestMove;
}

int Searcher::movePriority(int move, int depth) const
{
    //check if principal variation, will return value 300,000
    if (getPvMove() == move)
    {
        return 400000;
    }
    
    //MVVLVA, can return values from ~150k to ~250k
    int cap = captured(move);
    
    if (cap != NOPIECE)
    {
        int pce = getBoard()->getPce(fromR(move), fromC(move));
        int weight = 250000;
        
        weight += abs(worth(cap)) * 10;   //primary decider is worth of captured piece
        weight += (1000 - abs(worth(pce)));    //secondary decider is worth of capturing piece
        
        if (abs(worth(cap)) < abs(worth(pce)))
        {
            weight -= 100000;
        }
        
        return weight;
    }
    
    //Killer
    
    if (depth != -1 && depth < 2 * MAXDEPTH + 1)
    {
        if (move == getKiller1(depth))
        {
            return 125000;
        }
        else if (move == getKiller2(depth))
        {
            return 100000;
        }
    }
    
    //History
    return getHistoryScore(move);
}

void Searcher::storePvMove(int move)
{
    if (move != NOMOVE)
    {
        Board* b = getBoard();
        
        int i = b->getPosKey() % TTABLEENTRIES;
        
        //Trying to overwrite pv node for root position
        if (pvTable[i].posKey == rootPosKey)
        {
            //Can only do so if we are overwriting it with new info about the root node
            if (rootPosKey == b->getPosKey())
            {
                pvTable[i] = PVNode {getBoard()->getPosKey(), move};
            }
        }
        //General case, update pv table
        else
        {
            pvTable[i] = PVNode {getBoard()->getPosKey(), move};
        }
        
    }
    else
    {
        std::cerr << "storing null PV move" << std::endl;
        exit(1);
    }
}

int Searcher::getPvMove() const
{
    int i = getBoard()->getPosKey() % TTABLEENTRIES;
    
    //If this board has been stored in the pv nodes ttable
    if (pvTable[i].posKey == getBoard()->getPosKey())
    {
        return pvTable[i].move;
    }
    
    return NOMOVE;
}

void Searcher::printPvLine(int depth)
{
    int move = getPvMove();
    int count = 0;
    
    //While there is still another move in the line, or the line exceeds the depth
    while (move != NOMOVE && count < depth)
    {
        std::cout << printMove(move) << std::endl;    //Print out the move
        getMoveGenerator()->makeMove(move); //Make the move on the board
        move = getPvMove();                 //Update the pv move
        count++;
    }
    
    //Take back moves, according to the number of moves we made
    while (count > 0)
    {
        moveGenerator->takeBack();
        count--;
    }
}

void Searcher::storeKillerMove(int move, int depth)
{
    //Putting the current killer 1 into the killer 2 container
    killerMoves[depth + MAXDEPTH] = killerMoves[depth];
    //Storing the new killer move as the killer 1
    killerMoves[depth] = move;
}

int Searcher::getKiller1(int depth) const
{
    return killerMoves[depth];
}

int Searcher::getKiller2(int depth) const
{
    return killerMoves[depth + MAXDEPTH];
}

void Searcher::storeHistoryMove(int move, int score)
{
    int pce = getBoard()->getPce(fromR(move), fromC(move));
    
    if (pce == NOPIECE)
    {
        std::cerr << "something has gone horribly wrong" << std::endl;
        exit(1);
    }
    
    int index = (64 * pce) + (8 * toR(move)) + toC(move);
    historyMoves[index] += score;
}

int Searcher::getHistoryScore(int move) const
{
    int pce = getBoard()->getPce(fromR(move), fromC(move));
    
    if (pce == NOPIECE)
    {
        std::cerr << "something has gone horribly wrong" << std::endl;
        exit(1);
    }
    
    int index = (64 * pce) + (8 * toR(move)) + toC(move);
    return historyMoves[index];
}

void Searcher::storeTransPos(int score, int depth)
{
    int index = getBoard()->getPosKey() % TTABLEENTRIES;
    
    if (depth >= tTable[index].depth)
    {
        tTable[index] = Trans {getBoard()->getPosKey(), score, depth};
    }
}

int Searcher::getTransScore(int depth)
{
    int index = getBoard()->getPosKey() % TTABLEENTRIES;
    
    if (tTable[index].depth >= depth && tTable[index].pos == getBoard()->getPosKey())
    {
        return tTable[index].score;
    }
    
    return INFINITY;
}

void Searcher::prepSearch()
{
    for (int i = 0; i < TTABLEENTRIES; i++)
    {
        pvTable[i] = PVNode {0, 0};
        tTable[i] = Trans {0, 0, 0};
    }
    
    for (int i = 0; i < 2 * MAXDEPTH + 1; i++)
    {
        killerMoves[i] = 0;
    }
    
    for (int i = 0; i < 12 * 64; i++)
    {
        historyMoves[i] = 0;
    }
    
    stat.reset();
    stop = false;
    searchDepth = 1;
    rootPosKey = getBoard()->getPosKey();
    
    timer.start();
}

void Searcher::checkTime()
{
    if (timer.elapsed() > timeAllocated)
    {
        stop = true;
    }
}
