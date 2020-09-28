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
    //does not benefit much from ordering quiescent moves...
    
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
    //Loop through the moves generated
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
   //Presearch, clear tables
    
    for (int i = 1; i <= depth; i++)
    {
        stat.reset();
        Timer t;
        t.start();
        
        alphaBeta(-INFINITY, INFINITY, i);
        
        std::cout << "Nodes Searched: " << stat.nodes << std::endl;
        std::cout << "Search Speed: " << (stat.nodes / t.elapsed()) << " kN/s" << std::endl;
        std::cout << "Move Ordering: " << (100 * stat.failHighFirst / stat.failHigh) << '%' <<  std::endl;
        printPvLine(i);
    }
}

Movegen* Searcher::getMoveGenerator()
{
    return moveGenerator;
}

std::list<int> Searcher::orderedMoves(std::list<int> moves)
{
    std::list<int> li;
    //order moves
    for(std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        if (getPvMove() == *itr)
        {
            li.push_front(*itr);
        }
        else
        {
            li.push_back(*itr);
        }
        
        /*
        if (movePriority(*itr) == 1000)
        {
            std::cout << "pv node!" << std::endl;
            std::cout << printMove(*itr) << std::endl;
            getMoveGenerator()->getBoard()->printBoard();
        }*/
    }
    
    
    
    
    //assigning score stage
    
    //rearranging stage
    
    //some function to assign score
    return li;
}

int Searcher::movePriority(int move)
{
    //check if principal variation
    if (getPvMove() == move)
    {
        return 1000;
    }
    
    //mvvlva if capture
    
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
    for (std::list<PVNode>::iterator itr = pvTable[i].begin(); itr != pvTable[i].end(); itr++)
    {
        if (itr->posKey == moveGenerator->getBoard()->getPosKey())
        {
            return itr->move;
        }
    }
    
    return NOMOVE;
}

void Searcher::storePvMove(int move)
{
    int i = moveGenerator->getBoard()->getPosKey() % TTABLEENTRIES;
    pvTable[i].push_back(PVNode {moveGenerator->getBoard()->getPosKey(), move});
}
