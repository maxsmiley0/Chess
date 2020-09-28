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
    
    for (std::list<int>::iterator itr = captureList.begin(); itr != captureList.end(); itr++)
    {
        if (moveGenerator->makeMove(*itr))
        {
            movesMade++;
            int moveScore = -quiescenceSearch(-beta, -alpha);
            moveGenerator->takeBack();
            
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
    
    if (alpha != oldAlpha)
    {
        storePvMove(bestMove);
    }
    
    return alpha;
}

int Searcher::alphaBeta (int alpha, int beta, int depth)
{
    int movesMade = 0;
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    
    if (depth <= 0)
    {
        return quiescenceSearch(alpha, beta);
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
    
    //Probe PV table
    
    //Worry about move ordering later
    
    std::list<int> moveList = moveGenerator->generateMoves();
    moveList = orderedMoves(moveList);
    
    for (std::list<int>::iterator itr = moveList.begin(); itr != moveList.end(); itr++)
    {
        
        if (moveGenerator->makeMove(*itr))
        {
            movesMade++;
            int moveScore = -alphaBeta(-beta, -alpha, depth - 1);
            moveGenerator->takeBack();
            
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

void Searcher::reccomendMove(int depth)
{
    //Iterative deepening
    for (int i = 1; i <= depth; i++)
    {
        Timer t;
        t.start();
        stat.reset();
        
        alphaBeta(-INFINITY, INFINITY, i);
        
        std::cout << "Nodes Searched: " << stat.nodes << std::endl;
        std::cout << "Search Speed: " << (stat.nodes / t.elapsed()) << " kN/s" << std::endl;
        std::cout << "Move Ordering: " << (100 * stat.failHighFirst / stat.failHigh) << '%' <<  std::endl;
        printPvLine(depth);
    }
     /*
    
    stat.reset();
    Timer t;
    t.start();
    
    alphaBeta(-INFINITY, INFINITY, depth);
    
    std::cout << "Nodes Searched: " << stat.nodes << std::endl;
    std::cout << "Search Speed: " << (stat.nodes / t.elapsed()) << " kN/s" << std::endl;
    std::cout << "Move Ordering: " << (100 * stat.failHighFirst / stat.failHigh) << '%' <<  std::endl;
    printPvLine(depth);*/
}

Movegen* Searcher::getMoveGenerator()
{
    return moveGenerator;
}

std::list<int> Searcher::orderedMoves(std::list<int> moves)
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
            int weight = movePriority(*itr);
            bool inserted = false;
            //Loop through new list, insert when the weight is greater than next value, so we will have sorted this list from highest to lowest priority
            for (std::list<int>::iterator i = li.begin(); i != li.end(); i++)
            {
                if (weight > movePriority(*i))
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

int Searcher::movePriority(int move)
{
    //check if principal variation, will return value 300,000
    if (getPvMove() == move)
    {
        return 300000;
    }
    
    //MVVLVA, can return values from ~150k to ~250k
    int cap = captured(move);
    
    if (cap != NOPIECE)
    {
        int pce = moveGenerator->getBoard()->getPce(fromR(move), fromC(move));
        int weight = 150000;
        
        weight += abs(worth(cap)) * 10;   //primary decider is worth of captured piece
        weight += abs(worth(pce));    //secondary decider is worth of capturing piece
        
        return weight;
    }
    
    //killer?
    
    //search by history heuristic
    
    return 0;
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

void Searcher::storePvMove(int move)
{
    int i = moveGenerator->getBoard()->getPosKey() % TTABLEENTRIES;
    pvTable[i] = PVNode {moveGenerator->getBoard()->getPosKey(), move};
}
