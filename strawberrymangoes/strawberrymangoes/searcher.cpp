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
    int score = static_eval(moveGenerator->getBoard());
    bool gameOver = true;
    
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
            gameOver = false;
            int moveScore = -quiescenceSearch(-beta, -alpha);
            moveGenerator->takeBack();
            
            if (moveScore > alpha)
            {
                if (moveScore >= beta)
                {
                    return beta;
                }
                else
                {
                    alpha = moveScore;
                }
            }
        }
    }
    
    if (gameOver)
    {
        int side = moveGenerator->getBoard()->getSide();
        int thisSideKingR = moveGenerator->getBoard()->getKingR(side);
        int thisSideKingC = moveGenerator->getBoard()->getKingC(side);
        
        if (moveGenerator->squareAttacked(thisSideKingR, thisSideKingC, side))
        {
            return -INFINITY;
        }
        else
        {
            return 0;
        }
    }
    
    return alpha;
}

int Searcher::alphaBeta (int alpha, int beta, int depth)
{
    bool gameOver = true;       //will be set to false if any legal moves are generated
    int oldAlpha = alpha;
    int bestMove = NOMOVE;
    
    if (depth <= 0)
    {
        return quiescenceSearch(alpha, beta);
    }
    
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
            gameOver = false;
            int moveScore = -alphaBeta(-beta, -alpha, depth - 1);
            moveGenerator->takeBack();
            
            if (moveScore > alpha)
            {
                if (moveScore >= beta)
                {
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
    
    if (gameOver)
    {
        if (inCheck)
        {
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
    std::cout << alphaBeta(-INFINITY, INFINITY, depth) << std::endl;
    std::cout << bestMove << ' ' << printMove(bestMove) << std::endl;
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
