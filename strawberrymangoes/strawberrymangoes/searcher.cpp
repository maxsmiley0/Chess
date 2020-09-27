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
    InitPvTable();
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
            // + ply so mate in 2 is better than mate in 1!
            return -MATE;
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
    stat.reset();
    
    Timer t;
    t.start();
    
    std::cout << alphaBeta(-INFINITY, INFINITY, depth) << std::endl;
    
    std::cout << "Nodes Searched: " << stat.nodes << std::endl;
    std::cout << "Search Speed: " << (stat.nodes / t.elapsed()) << " kN/s" << std::endl;
    std::cout << "Move Ordering: " << (100 * stat.failHighFirst / stat.failHigh) << '%' <<  std::endl;
    
    printPvLine(depth);
}

Movegen* Searcher::getMoveGenerator()
{
    return moveGenerator;
}

void Searcher::InitPvTable()
{
    for (int i = 0; i < TTABLEENTRIES; i++)
    {
        pvTable[i] = PVNode {0, 0};
    }
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
        //Compare stored move to this move
        return pvTable[i].move;
    }
    else
    {
        return NOMOVE;
    }
}

void Searcher::storePvMove(int move)
{
    int i = moveGenerator->getBoard()->getPosKey() % TTABLEENTRIES;
    pvTable[i].move = move;
    pvTable[i].posKey = moveGenerator->getBoard()->getPosKey();
}
