//
//  movegen.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef movegen_h
#define movegen_h

#include "defs.h"
#include "board.h"

class Movegen
{
public:
    Movegen(Board& b);
    void generateMoves(int ply);   //fills the moves and scores array with moves. only accounts for legal moves, without checking to see if king is in check at end
    bool squareAttacked(int r, int c);  //returns true if square is attacked, really only used for checking if king is in check
    void printAttacked();          //prints attacked squares for debugging purposes
    int getMove(int sR, int sC, int eR, int eC, int promoted);   //generates a move given from, and to, assumes valid parameters
    void printMoves(int ply);              //prints moves to cout
    
private:
    //Board from which moves and scores are generated
    Board mBoard;
    
    //Indexed by MAXPOSITIONMOVES * search ply
    int moves[MAXPOSITIONMOVES * MAXDEPTH];     //after moves have been generated, append subcontainer with '0' so we know when we are finished
    int scores[MAXPOSITIONMOVES * MAXDEPTH];
};

#endif /* movegen_h */
