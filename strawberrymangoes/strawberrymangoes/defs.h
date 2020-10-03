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
class Board;

//Pieces
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

//Castling Perms
#define INVERSE 15  //inverts castling bits when XORed
#define WKCA 8
#define WQCA 4
#define BKCA 2
#define BQCA 1

//Sides
#define WHITE 0
#define BLACK 1
#define NEITHER 2

//Misc
#define NOMOVE 0
#define MAXPOSITIONMOVES 256
#define MINDEPTH 5
#define MAXDEPTH 32
#define MAXGAMELENGTH 1024
#define TTABLEENTRIES 100000

#define MATE 90000
#define INFINITY 100000

#include <string>

//For purposes of statistic countings
static int totalNodes = 0;
static double totalDepth = 0;

//A quick shortcut, piece representations indexed by piece type
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
bool isEnpasMove(int move);
bool isPawnstartMove(int move);
bool isCastleMove(int move);
std::string printMove(int move);    //converts move e2e4 form

int worth(int pce);     //returns worth, give piece

//static evaluation, to be called at the leaf nodes of the game tree
int static_eval(Board* b);

//Timer class for statistics purposes
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

//For the history table
struct History
{
    int posKey;
    int move;
    int pce;
    int castlePerm;
    int enpasSquareR;
    int enpasSquareC;
};

//For the searcher class
struct PVNode
{
    int posKey;
    int move;
};

//Statistics collection for move ordering and nodes visited info
struct Stats
{
    void reset()
    {
        nodes = 0;
        failHigh = 0;
        failHighFirst = 0;
    }
    
    int nodes;
    int failHigh;
    int failHighFirst;
};
#endif /* defs_h */
