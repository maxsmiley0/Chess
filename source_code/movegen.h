//
//  movegen.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef movegen_h
#define movegen_h

#include "board.h"
#include <list>

class Movegen
{
public:
    Movegen();
    Movegen(const Movegen& other);          //copy constructor for dummy ponder board
    ~Movegen();
    
    //Move generation related
    std::list<int> generateMoves() const;   //returns a list of legal moves in a given position. only accounts for pseudolegal moves, without checking to see if king is in check at end
    std::list<int> generateCaptures() const;   //returns a list of legal captures in a given position. only accounts for pseudolegal captures, without checking to see if king is in check at end
    bool squareAttacked(int r, int c, int side) const;  //returns true if square is attacked, used for king checks and castling, if we specify side as WHITE, it will loop through black pieces
    
    //Updates all relevant key information for board
    bool makeMove(int move);    //Returns true if we can make such a move (and makes the move), false otherwise
    void takeBack();            //Takes back the previously made move
    int getMove(int sR, int sC, int eR, int eC, int promoted) const;   //generates a move given from, and to, assumes valid parameters
    
    Board* getBoard() const {return mBoard; } //trivial accessor
    
    //Perft functions, used to verify the integrity of the legal move generator
    void perft(int depth);
    void perftTest(int depth);
private:
    //Board from which moves and scores are generated
    Board* mBoard;
    
    int leafNodes;  //used in perft / perftTest
    
    //Debugging related
    void printMoves(std::list<int> moves); //prints moves to cout
    void printAttacked();                  //prints attacked squares
};

#endif /* movegen_h */
