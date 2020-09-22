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

#define WKCA 8
#define WQCA 4
#define BKCA 2
#define BQCA 1

#define WHITE 0
#define BLACK 1
#define NEITHER 2

#define MAXPOSITIONMOVES 256
#define MAXDEPTH 32

#define FLAGENPASSANT 0x40000
#define FLAGPAWNSTART 0x80000
#define FLAGCASTLE 0x1000000
#define FLAGCAPTURE 0x7C000
#define FLAGPROMOTION 0xF00000
#define NOFLAG 0

#include <string>

static std::string PceChar = "pnbrqkPNBRQK.";

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

#endif /* defs_h */
