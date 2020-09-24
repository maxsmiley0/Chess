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
    bool squareAttacked(int r, int c, int side, int move = 0);  //returns true if square is attacked, used for king checks and castling, if we specify side as WHITE, it will loop through black pieces
    void printAttacked();          //prints attacked squares for debugging purposes
    int getMove(int sR, int sC, int eR, int eC, int promoted);   //generates a move given from, and to, assumes valid parameters
    void printMoves(int ply);              //prints moves to cout
    Board& getBoard() {return mBoard; }
    
    //Returns true if we can make such a move, false otherwise
    //Updates all relevant key information for board
    bool makeMove(int move);
    void takeBack();    //Takes back the previously made move
    
    //Perft functions, used to verify the integrity of the legal move generator
    void perft(int depth);
    void perftTest(int depth);
    
private:
    //Board from which moves and scores are generated
    Board mBoard;
    
    //Indexed by MAXPOSITIONMOVES * search ply
    int moves[MAXPOSITIONMOVES * MAXDEPTH];     //after moves have been generated, append subcontainer with '0' so we know when we are finished
    int scores[MAXPOSITIONMOVES * MAXDEPTH];
    int leafNodes;  //perft related
};

#endif /* movegen_h */
