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
#include "defs.h"

class Board
{
public:
    Board();
    
    //Modifies board material information
    void parseFen(std::string fen);           //updates board according to fen, assumes valid fen
    void addPiece(int r, int c, int pce);     //adds a piece to the board, and updates the position key, assumes an empty spot, assumes pce is valid
    void removePiece(int r, int c);           //removes a piece from the board, updates the position key, assumes there is a piece in that square
    void pushHistory(int move);   //updates history with new move
    void popHistory();            //gets rid of latest move
    
    //Accessors
    int getPce(int r, int c) const;        //returns piece located at (r, c)
    int getPceNum(int pce) const;          //returns # of pieces of a given type
    int getPceR(int pce, int i) const;     //returns R coord on 'ith' piece (from 0 to pceNum)
    int getPceC(int pce, int i) const;     //returns C coord on 'ith' piece (from 0 to pceNum)
    int getKingR(int color) const;         //returns r coord of king of a given color
    int getKingC(int color) const;         //returns c coord of king of a given color
    int getSide() const {return side; }
    int getPosKey() const {return posKey;} //Returns position key
    int getEnpasSquareR() const {return enpasSquareR; }
    int getEnpasSquareC() const {return enpasSquareC; }
    int getHisPly() const {return hisPly; }
    int getMaterial() const {return material;} //returns material differential of board
    int numRep() const;                        //returns number of repetitions
    int sqExposed(int r, int c) const;          //If square were a queen, returns how many legal moves that queen would have. Basic metric for king safety. Does not count horizontal squares
    int sqExposedVert(int r, int c) const;      //Returns how many forward / backwards square exist. Used for placing rooks on (semi)open files
    bool hasKcPerm() const;   //returns if the current side still has kingside castling perms
    bool hasQcPerm() const;   //returns if the current side still has queenside castling perms
    int getFiftyMove() const; //returns last ply when a pawn was moved, or a capture / promotion occured
    void updateFiftyMove();   //updates fiftyMove ply to hisPly
    History getLastState() const;             //returns latest move in history table
    
    //Modifies en passant, castling, and side information
    void hashInCastle(int castlePerm);   //hashes in a given castle perm
    void hashOutCastle(int castlePerm);  //hashes out a given castle perm
    void hashInEp(int r, int c);         //hashes in an ep
    void hashOutEp();                    //hashes out ep, if enpas square exists
    void changeSide();                   //changes side and hashes the pos key
    
    void printBoard(int side = WHITE);   //prints char representation to couts
    
private:
    //For purpose of debugging, so we can see where the pieces are according to their piece list
    void printPieces(int pce);
    
    //Initialization functions
    void ClearBoard();  //initializes all values to NOPIECE, all piece lists and nums to empty
    void InitKeys();    //initializes all keys to random 32 bit numbers
    
    //Related to position key
    int posKey;         //all active keys XORed together form a unique key
    int sideKey;        //key for if white is to move
    int castleKeys[4];  //4 keys for each castling side
    int enpasKey[8][8];   //only 16 possible squares, but we'll generate 64 keys for looping convenience
    int pceKeys[8][8][12];  //64 squares with 12 possible pieces
    int castlePerm;     //stores the castle perms
    
    //Board information
    int mBoard[8][8];   //stores pieces
    int side;           //side to move
    int pceNum[12];     //stores number of each type of piece
    int pListR[120];    //stores row of each piece, indexed by piece type
    int pListC[120];    //stores column of each piece, indexed by piece type
    int enpasSquareR;   //stores the en passant square row
    int enpasSquareC;   //stores the en passant square column
    int material;       //stores material differential of the board
    
    //History
    History history[MAXGAMELENGTH]; //stores all moves made
    int hisPly;                 //stores what number ply of the game we are on
    int fiftyMove;              //Last ply when moving pawn or capture or promotion
};

#endif /* board_h */
