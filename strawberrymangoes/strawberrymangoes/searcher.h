//
//  searcher.hpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/25/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef searcher_hpp
#define searcher_hpp

#include "movegen.h"
#include <list>

class Searcher
{
public:
    Searcher();
    ~Searcher();
    
    int alphaBeta(int alpha, int beta, int depth);      //returns alpha in centipawns
    int quiescenceSearch(int alpha, int beta);          //called at nodes of alphaBeta
    void reccomendMove(int depth);   //computer plays the move recommended by alphabeta
    Movegen* getMoveGenerator();    //returns a pointer to the move generator member
    
    void orderMoves(std::list<int>& moves);    //takes in an unordered list of legal moves, orders them
    
    void printPvLine(int depth);    //after recommending a move, print the pv line
    void storePvMove(int move);     //stores pv move for a particular board
    int getPvMove();                //returns PV move for a given position
    
private:
    Movegen* moveGenerator;
    std::list<PVNode> pvTable[TTABLEENTRIES];     //stores principal variation moves indexed by position key modulo TTABLEENTRIES
    
    int bestMove;
    int ply;
    Stats stat;
};

#endif /* searcher_hpp */
