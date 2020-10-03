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

int Searcher::quiescenceSearch(int alpha, int beta)
{
    if ((stat.nodes & 2047) == 0)
    {
        checkTime();
    }
    
    //Statistics collection
    stat.nodes++;
    
    int score = static_eval(moveGenerator->getBoard());
    int movesMade = 0;
    int oldAlpha = alpha;
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
    
    std::list<int> captureList = moveGenerator->generateCaptures();
    captureList = orderedMoves(captureList);
    
    for (std::list<int>::iterator itr = captureList.begin(); itr != captureList.end(); itr++)
    {
        if (moveGenerator->makeMove(*itr))
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
                    bestMove = *itr;
                }
            }
        }
    }
    
    return alpha;
}

int Searcher::alphaBeta (int alpha, int beta, int depth)
{
    int movesMade = 0;
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    
    if (depth <= 0 || depth >= MAXDEPTH)
    {
        return quiescenceSearch(alpha, beta);
    }
    
    if ((stat.nodes & 2047) == 0)
    {
        checkTime();
    }
    
    if (moveGenerator->getBoard()->numRep() >= 2)
    {
        return 0;
    }
    
    //Statistics collection
    stat.nodes++;
    
    int side = moveGenerator->getBoard()->getSide();
    int kingR = moveGenerator->getBoard()->getKingR(side);
    int kingC = moveGenerator->getBoard()->getKingC(side);
    
    bool inCheck = moveGenerator->squareAttacked(kingR, kingC, side);
    
    if (inCheck)
    {
        depth++;
    }
    
    std::list<int> moveList = moveGenerator->generateMoves();
    moveList = orderedMoves(moveList, depth);
    
    for (std::list<int>::iterator itr = moveList.begin(); itr != moveList.end(); itr++)
    {
        if (moveGenerator->makeMove(*itr))
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
                    if (captured(*itr) == NOPIECE)
                    {
                        storeKillerMove(*itr, depth);
                    }
                    
                    return beta;
                }
                else
                {
                    alpha = moveScore;
                    bestMove = *itr;
                    
                    if (captured(*itr) == NOPIECE)
                    {
                        storeHistoryMove(bestMove, depth * depth);
                    }
                }
            }
        }
    }
    
    //worryabout mates later... wait we have to worry about them now...
    
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

int Searcher::reccomendMove()
{
    prepSearch();
    
    //Iterative deepening
    for (; true; searchDepth++)
    {
        alphaBeta(-INFINITY, INFINITY, searchDepth);
        
        if (getPvMove() == NOMOVE)
        {
            //principalMove = getPvMove();
            std::cerr << "pv move is null move" << std::endl;
            std::cout << printMove(getPvMove()) << std::endl;
            std::cout << printMove(pvTable[moveGenerator->getBoard()->getPosKey() % TTABLEENTRIES].move) << std::endl;
            std::cout << "Stored board id:  " << pvTable[moveGenerator->getBoard()->getPosKey() % TTABLEENTRIES].posKey << std::endl;
            std::cout << "Current GameBoard Id: " << moveGenerator->getBoard()->getPosKey() << std::endl;
            exit(1);
        }
        
        if (stop)
        {
            break;
        }
    }
    
    std::cout << "Nodes Searched: " << stat.nodes << std::endl;
    std::cout << "Search Speed: " << (stat.nodes / timeAllocated) << " kN/s" << std::endl;
    std::cout << "Move Ordering: " << (100 * stat.failHighFirst / (stat.failHigh + 1)) << '%' <<  std::endl;
    std::cout << "Depth: " << (searchDepth - 1) << std::endl;
    printPvLine(searchDepth - 1);
    
    totalNodes += stat.nodes;
    totalDepth += searchDepth - 1;
    
    std::cout << "Total Nodes: " << totalNodes << std::endl;
    std::cout << "Avg Depth: " << (totalDepth / 100) << std::endl;
    
     /*
    
    stat.reset();
    Timer t;
    t.start();
    
    alphaBeta(-INFINITY, INFINITY, depth);
    
    std::cout << "Nodes Searched: " << stat.nodes << std::endl;
    std::cout << "Search Speed: " << (stat.nodes / t.elapsed()) << " kN/s" << std::endl;
    std::cout << "Move Ordering: " << (100 * stat.failHighFirst / stat.failHigh) << '%' <<  std::endl;
    printPvLine(depth);*/
    //best move not being updated or finding the same move?
    std::cout << printMove(getPvMove()) << std::endl;
    //std::cout << printMove(principalMove) << std::endl;
    
    if (getPvMove() == NOMOVE)
    {
        //realloc time
    }
    
    return getPvMove();
}

Movegen* Searcher::getMoveGenerator()
{
    return moveGenerator;
}

std::list<int> Searcher::orderedMoves(std::list<int> moves, int depth)
{
    std::list<int> li;  //list to store ordered moves
    
    //Loop through the given list
    for(std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        //If empty list, insert
        if (li.empty() == *itr)
        {
            li.push_back(*itr);
        }
        else
        {
            //Obtain weight of move
            int weight = movePriority(*itr, depth);
            bool inserted = false;
            //Loop through new list, insert when the weight is greater than next value, so we will have sorted this list from highest to lowest priority
            for (std::list<int>::iterator i = li.begin(); i != li.end(); i++)
            {
                if (weight > movePriority(*i, depth))
                {
                    li.insert(i, *itr);
                    inserted = true;
                    break;
                }
            }
            //If not inserted, it never found a place, and so it must have lowest priority
            if (!inserted)
            {
                li.push_back(*itr);
            }
        }
    }
    
    return li;
}

int Searcher::movePriority(int move, int depth)
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
        int pce = moveGenerator->getBoard()->getPce(fromR(move), fromC(move));
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

int Searcher::getPvMove()
{
    int i = moveGenerator->getBoard()->getPosKey() % TTABLEENTRIES;
    
    //If this board has been stored in the pv nodes ttable
    if (pvTable[i].posKey == moveGenerator->getBoard()->getPosKey())
    {
        return pvTable[i].move;
    }
    
    return NOMOVE;
}

void Searcher::prepSearch()
{
    for (int i = 0; i < TTABLEENTRIES; i++)
    {
        pvTable[i] = PVNode {0, 0};
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
    rootPosKey = moveGenerator->getBoard()->getPosKey();
    
    timer.start();
}

void Searcher::storePvMove(int move)
{
    if (move != NOMOVE)
    {
        Board* b = moveGenerator->getBoard();
        
        int i = b->getPosKey() % TTABLEENTRIES;
        
        //Trying to overwrite pv node for root position
        if (pvTable[i].posKey == rootPosKey)
        {
            //Can only do so if we are overwriting it with new info about the root node
            if (rootPosKey == b->getPosKey())
            {
                pvTable[i] = PVNode {moveGenerator->getBoard()->getPosKey(), move};
            }
            else
            {
                std::cerr << "Attempting to rewrite pv node for root pos key" << std::endl;
            }
            
        }
        //General case, update pv table
        else
        {
            pvTable[i] = PVNode {moveGenerator->getBoard()->getPosKey(), move};
        }
        
    }
    else
    {
        std::cerr << "storing null PV move" << std::endl;
        exit(1);
    }
}

void Searcher::storeKillerMove(int move, int depth)
{
    //Putting the current killer 1 into the killer 2 container
    killerMoves[depth + MAXDEPTH] = killerMoves[depth];
    //Storing the new killer move as the killer 1
    killerMoves[depth] = move;
}

void Searcher::storeHistoryMove(int move, int score)
{
    int pce = moveGenerator->getBoard()->getPce(fromR(move), fromC(move));
    
    if (pce == NOPIECE)
    {
        std::cerr << "something has gone horribly wrong" << std::endl;
        exit(1);
    }
    
    int index = (64 * pce) + (8 * toR(move)) + toC(move);
    historyMoves[index] = score;
}

int Searcher::getHistoryScore(int move)
{
    int pce = moveGenerator->getBoard()->getPce(fromR(move), fromC(move));
    
    if (pce == NOPIECE)
    {
        std::cerr << "something has gone horribly wrong" << std::endl;
        exit(1);
    }
    
    int index = (64 * pce) + (8 * toR(move)) + toC(move);
    return historyMoves[index];
}

int Searcher::getKiller1(int depth)
{
    return killerMoves[depth];
}

int Searcher::getKiller2(int depth)
{
    return killerMoves[depth + MAXDEPTH];
}

void Searcher::checkTime()
{
    if (timer.elapsed() > timeAllocated && searchDepth > minDepth)
    {
        stop = true;
    }
}
