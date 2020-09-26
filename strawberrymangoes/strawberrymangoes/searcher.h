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

class Searcher
{
public:
    Searcher();
    ~Searcher();
    
    int alphaBeta(int alpha, int beta, int depth);      //returns alpha in centipawns
    void reccomendMove(int depth);   //computer plays the move recommended by alphabeta
    Movegen* getMoveGenerator();    //returns a pointer to the move generator member
    
private:
    Movegen* moveGenerator;
    int pvTable[MAXDEPTH];          //stores principal variation move at each depth
    int bestMove;
    int ply;
};

#endif /* searcher_hpp */
