//
//  defs.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef defs_h
#define defs_h

#include <iostream>
#include <chrono>

#define OFFBOARD -1
#define WP 0
#define WN 1
#define WB 2
#define WR 3
#define WQ 4
#define WK 5
#define BP 6
#define BN 7
#define BB 8
#define BR 9
#define BQ 10
#define BK 11
#define NOPIECE 12

#define INVERSE 15  //inverts castling bits when XORed
#define WKCA 8
#define WQCA 4
#define BKCA 2
#define BQCA 1

#define WHITE 0
#define BLACK 1
#define NEITHER 2

#define MAXPOSITIONMOVES 256
#define MAXDEPTH 32

#define MAXGAMELENGTH 1024

#include <string>

static std::string PceChar = "PNBRQKpnbrqk.";
static std::string RankChar = "87654321";
static std::string FileChar = "abcdefgh";

int PceCol (int pce);   //Returns color, given a piece
int RAND32 ();          //Returns a random 32 bit integer

bool isPawn(int pce);   //returns true if pawn
bool isKing(int pce);   //returns true if king
bool doDiag(int pce);   //return true if bishop or queen
bool doVert(int pce);   //return trie if rook or queen

//Related to extracting information about the move key
int fromR(int move);
int fromC(int move);
int toR(int move);
int toC(int move);
int captured(int move);
int promoted(int move);
std::string printMove(int move);    //converts move e2e4 form

bool isEnpasMove(int move);
bool isPawnstartMove(int move);
bool isCastleMove(int move);

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

struct History
{
    int move;
    int pce;
    int castlePerm;
    int enpasSquareR;
    int enpasSquareC;
};

#endif /* defs_h */
