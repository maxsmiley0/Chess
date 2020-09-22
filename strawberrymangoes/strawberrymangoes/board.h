//
//  board.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef board_h
#define board_h

#include <string>

class Board
{
public:
    Board();
    
    int getPosKey();
    void addPiece(int r, int c, int pce);     //adds a piece to the board, and updates the position key, assumes an empty spot, assumes pce is valid
    void removePiece(int r, int c);           //removes a piece from the board, updates the position key, assumes there is a piece in that square
    void parseFen(std::string fen);           //updates board according to fen, assumes valid fen
    int getPce(int r, int c);        //returns piece located at (r, c)
    int getPceR(int pce, int i);     //returns R coord on 'ith' piece (from 0 to pceNum)
    int getPceC(int pce, int i);     //returns C coord on 'ith' piece (from 0 to pceNum)
    int getPceNum(int pce); //returns # of pieces of a given type
    int getSide() {return side; }
    
    void printBoard();      //prints char representation to couts
    
private:
    //Initialization functions
    void ClearBoard();  //initializes all values to NOPIECE, all piece lists and nums to empty
    void InitKeys();    //initializes all keys to random 32 bit numbers
    
    //Related to position key
    int posKey;         //all active keys XORed together form a unique key
    int sideKey;        //key for if white is to move
    int castleKeys[4];  //4 keys for each castling side
    int enpasKey[8][8];   //only 16 possible squares, but we'll generate 64 keys for looping convenience
    int pceKeys[8][8][12];  //64 squares with 12 possible pieces
    
    //Board information
    int mBoard[8][8];   //stores pieces
    int side;           //side to move
    int pceNum[12];     //stores number of each type of piece
    int pListR[120];    //stores row of each piece, indexed by piece type
    int pListC[120];    //stores column of each piece, indexed by piece type
    int enpasSquareR;   //stores the en passant square row
    int enpasSquareC;   //stores the en passant square column
    int castlePerm;     //stores the castle perms
    //piece num, piece lists
    //Piece lists somewhere...
};

#endif /* board_h */
