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

int Searcher::alphaBeta (int alpha, int beta, int depth)
{
    bool gameOver = true;       //will be set to false if any legal moves are generated
    
    int oldAlpha = alpha;
    int bestMove = 0;
    
    if (depth == 0)
    {
        return static_eval(moveGenerator->getBoard());
    }
    
    //Probe PV table
    
    //Worry about move ordering later
    moveGenerator->generateMoves(depth);
    //Loop through the moves generated
    for (int i = depth * MAXPOSITIONMOVES; moveGenerator->getMove(i) != 0; i++)
    {
        if (moveGenerator->makeMove(moveGenerator->getMove(i)))
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
                    bestMove = moveGenerator->getMove(i);
                }
            }
        }
    }
    
    //worryabout mates later... wait we have to worry about them now...
    
    if (gameOver)
    {
        std::cerr << "this occurs?" << std::endl;
        return -1000;
    }
    
    if (alpha != oldAlpha)
    {
        pvTable[depth] = bestMove;
    }
    
    return alpha;
}

void Searcher::reccomendMove(int depth)
{
    std::cout << alphaBeta(-INFINITY, INFINITY, depth) << std::endl;
    
    for (int i = depth; i >= 1; i--)
    {
        std::cout << i << ' ' << printMove(pvTable[i]) << ' ' << pvTable[i] << std::endl;
    }
}

Movegen* Searcher::getMoveGenerator()
{
    return moveGenerator;
}

