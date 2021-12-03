//
//  defs.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "defs.h"
#include "board.h"

#include <climits> 

//Returns true if a piece is a pawn
bool isPawn(int pce)
{
    return (pce == WP || pce == BP);
}

//Returns true if a piece is a king
bool isKing(int pce)
{
    return (pce == WK || pce == BK);
}

//Returns true if a piece is a bishop or a queen
bool doDiag(int pce)
{
    return (pce == WB || pce == BB || pce == WQ || pce == BQ);
}

//Returns true if a piece is a rook or a queen
bool doVert(int pce)
{
    return (pce == WR || pce == BR || pce == WQ || pce == BQ);
}

//Returns color of a piece
int PceCol (int pce)
{
    if (pce >= 0 && pce <= 5)
    {
        return WHITE;
    }
    else if (pce >= 6 && pce <= 11)
    {
        return BLACK;
    }
    else
    {
        std::cerr << "Invalid piece color" << std::endl;
        exit(1);
    }
}

//Extracts information from a move key
int fromR(int move)
{
    return (move & 0x7);
}

int fromC(int move)
{
    return ((move & 0x38) >> 3);
}

int toR(int move)
{
    return ((move & 0x1C0) >> 6);
}

int toC(int move)
{
    return ((move & 0xE00) >> 9);
}

int captured(int move)
{
    return ((move & 0xF000) >> 12);
}

int promoted(int move)
{
    return ((move & 0x3C0000) >> 18);
}

bool isEnpasMove(int move)
{
    return ((move & 0x10000) >> 16);
}

bool isPawnstartMove(int move)
{
    return ((move & 0x20000) >> 17);
}
bool isCastleMove(int move)
{
    return ((move & 0x400000) >> 22);
}

//Converts move key into readable form
std::string printMove(int move)
{
    std::string str = "";
    str += FileChar[fromC(move)];
    str += RankChar[fromR(move)];
    str += FileChar[toC(move)];
    str += RankChar[toR(move)];
    
    return (move == NOMOVE ? "NOMOVE" : str);
}

//Returns worth of a piece
int worth(int pce)
{
    switch (pce)
    {
        case WP:
            return 100;
        case WN:
            return 325;
        case WB:
            return 325;
        case WR:
            return 550;
        case WQ:
            return 1000;
        case WK:
            return 50000;
        case BP:
            return -100;
        case BN:
            return -325;
        case BB:
            return -325;
        case BR:
            return -550;
        case BQ:
            return -1000;
        case BK:
            return -50000;
        default:
            std::cerr << "Trying to find worth of an undefined pce" << std::endl;
            exit(1);
    }
}

//  PESTO PST's
int mg_pawn_table[8][8] = {
    {  0,   0,   0,   0,   0,   0,  0,   0},
    { 98, 134,  61,  95,  68, 126, 34, -11},
    { -6,   7,  26,  31,  65,  56, 25, -20},
    {-14,  13,   6,  21,  23,  12, 17, -23},
    {-27,  -2,  -5,  12,  17,   6, 10, -25},
    {-26,  -4,  -4, -10,   3,   3, 33, -12},
    {-35,  -1, -20, -23, -15,  24, 38, -22},
    {  0,   0,   0,   0,   0,   0,  0,   0}
};

int eg_pawn_table[8][8] = {
    {  0,   0,   0,   0,   0,   0,   0,   0},
    {178, 173, 158, 134, 147, 132, 165, 187},
    { 94, 100,  85,  67,  56,  53,  82,  84},
    { 32,  24,  13,   5,  -2,   4,  17,  17},
    { 13,   9,  -3,  -7,  -7,  -8,   3,  -1},
    {  4,   7,  -6,   1,   0,  -5,  -1,  -8},
    { 13,   8,   8,  10,  13,   0,   2,  -7},
    {  0,   0,   0,   0,   0,   0,   0,   0}
};

int mg_knight_table[8][8] = {
    {-167, -89, -34, -49,  61, -97, -15, -107},
    { -73, -41,  72,  36,  23,  62,   7,  -17},
    { -47,  60,  37,  65,  84, 129,  73,   44},
    {  -9,  17,  19,  53,  37,  69,  18,   22},
    { -13,   4,  16,  13,  28,  19,  21,   -8},
    { -23,  -9,  12,  10,  19,  17,  25,  -16},
    { -29, -53, -12,  -3,  -1,  18, -14,  -19},
    {-105, -21, -58, -33, -17, -28, -19,  -23}
};

int eg_knight_table[8][8] = {
    {-58, -38, -13, -28, -31, -27, -63, -99},
    {-25,  -8, -25,  -2,  -9, -25, -24, -52},
    {-24, -20,  10,   9,  -1,  -9, -19, -41},
    {-17,   3,  22,  22,  22,  11,   8, -18},
    {-18,  -6,  16,  25,  16,  17,   4, -18},
    {-23,  -3,  -1,  15,  10,  -3, -20, -22},
    {-42, -20, -10,  -5,  -2, -20, -23, -44},
    {-29, -51, -23, -15, -22, -18, -50, -64}
};

int mg_bishop_table[8][8] = {
    {-29,   4, -82, -37, -25, -42,   7,  -8},
    {-26,  16, -18, -13,  30,  59,  18, -47},
    {-16,  37,  43,  40,  35,  50,  37,  -2},
    { -4,   5,  19,  50,  37,  37,   7,  -2},
    { -6,  13,  13,  26,  34,  12,  10,   4},
    {  0,  15,  15,  15,  14,  27,  18,  10},
    {  4,  15,  16,   0,   7,  21,  33,   1},
    {-33,  -3, -14, -21, -13, -12, -39, -21}
};

int eg_bishop_table[8][8] = {
    {-14, -21, -11,  -8, -7,  -9, -17, -24},
    { -8,  -4,   7, -12, -3, -13,  -4, -14},
    {  2,  -8,   0,  -1, -2,   6,   0,   4},
    { -3,   9,  12,   9, 14,  10,   3,   2},
    { -6,   3,  13,  19,  7,  10,  -3,  -9},
    {-12,  -3,   8,  10, 13,   3,  -7, -15},
    {-14, -18,  -7,  -1,  4,  -9, -15, -27},
    {-23,  -9, -23,  -5, -9, -16,  -5, -17}
};

int mg_rook_table[8][8] = {
    { 32,  42,  32,  51, 63,  9,  31,  43},
    { 27,  32,  58,  62, 80, 67,  26,  44},
    { -5,  19,  26,  36, 17, 45,  61,  16},
    {-24, -11,   7,  26, 24, 35,  -8, -20},
    {-36, -26, -12,  -1,  9, -7,   6, -23},
    {-45, -25, -16, -17,  3,  0,  -5, -33},
    {-44, -16, -20,  -9, -1, 11,  -6, -71},
    {-19, -13,   1,  17, 16,  7, -37, -26}
};

int eg_rook_table[8][8] = {
    {13, 10, 18, 15, 12,  12,   8,   5},
    {11, 13, 13, 11, -3,   3,   8,   3},
    { 7,  7,  7,  5,  4,  -3,  -5,  -3},
    { 4,  3, 13,  1,  2,   1,  -1,   2},
    { 3,  5,  8,  4, -5,  -6,  -8, -11},
    {-4,  0, -5, -1, -7, -12,  -8, -16},
    {-6, -6,  0,  2, -9,  -9, -11,  -3},
    {-9,  2,  3, -1, -5, -13,   4, -20}
};

int mg_queen_table[8][8] = {
    {-28,   0,  29,  12,  59,  44,  43,  45},
    {-24, -39,  -5,   1, -16,  57,  28,  54},
    {-13, -17,   7,   8,  29,  56,  47,  57},
    {-27, -27, -16, -16,  -1,  17,  -2,   1},
    { -9, -26,  -9, -10,  -2,  -4,   3,  -3},
    {-14,   2, -11,  -2,  -5,   2,  14,   5},
    {-35,  -8,  11,   2,   8,  15,  -3,   1},
    { -1, -18,  -9,  10, -15, -25, -31, -50}
};

int eg_queen_table[8][8] = {
    { -9,  22,  22,  27,  27,  19,  10,  20},
    {-17,  20,  32,  41,  58,  25,  30,   0},
    {-20,   6,   9,  49,  47,  35,  19,   9},
    {  3,  22,  24,  45,  57,  40,  57,  36},
    {-18,  28,  19,  47,  31,  34,  39,  23},
    {-16, -27,  15,   6,   9,  17,  10,   5},
    {-22, -23, -30, -16, -16, -23, -36, -32},
    {-33, -28, -22, -43,  -5, -32, -20, -41}
};

int mg_king_table[8][8] = {
    {-65,  23,  16, -15, -56, -34,   2,  13},
    { 29,  -1, -20,  -7,  -8,  -4, -38, -29},
    { -9,  24,   2, -16, -20,   6,  22, -22},
    {-17, -20, -12, -27, -30, -25, -14, -36},
    {-49,  -1, -27, -39, -46, -44, -33, -51},
    {-14, -14, -22, -46, -44, -30, -15, -27},
    {  1,   7,  -8, -64, -43, -16,   9,   8},
    {-15,  36,  12, -54,   8, -28,  24,  14}
};

int eg_king_table[8][8] = {
    {-74, -35, -18, -18, -11,  15,   4, -17},
    {-12,  17,  14,  17,  17,  38,  23,  11},
    { 10,  17,  23,  15,  20,  45,  44,  13},
    { -8,  22,  24,  27,  26,  33,  26,   3},
    {-18,  -4,  21,  24,  27,  23,   9, -11},
    {-19,  -3,  11,  21,  23,  16,   7,  -9},
    {-27, -11,   4,  13,  14,   4,  -5, -17},
    {-53, -34, -21, -11, -28, -14, -24, -43}
};

//Middlegame worth
#define PM 82
#define NM 337
#define BM 365
#define RM 477
#define QM 1025

//Endgame worth
#define PE 94
#define NE 281
#define BE 297
#define RE 512
#define QE 936

//Gamephase worth
#define PG 0
#define NG 1
#define BG 1
#define RG 2
#define QG 4

//Static evaluation, to be called at the leaf nodes
const int static_eval(const Board& b)
{
    int mgScore = 0;
    int egScore = 0;
    int gamePhase = 0;
    
    //Looping through all white pawns
    for(int i = 0; i < b.getPceNum(WP); i++)
    {
        mgScore += PM;
        egScore += PE;
        mgScore += mg_pawn_table[b.getPceR(WP, i)][b.getPceC(WP, i)];
        egScore += eg_pawn_table[b.getPceR(WP, i)][b.getPceC(WP, i)];
        gamePhase += PG;
    }
    //Looping through all white knights
    for(int i = 0; i < b.getPceNum(WN); i++)
    {
        mgScore += NM;
        egScore += NE;
        mgScore += mg_knight_table[b.getPceR(WN, i)][b.getPceC(WN, i)];
        egScore += eg_knight_table[b.getPceR(WN, i)][b.getPceC(WN, i)];
        gamePhase += NG;
    }
    //Looping through all white bishops
    for(int i = 0; i < b.getPceNum(WB); i++)
    {
        mgScore += BM;
        egScore += BE;
        mgScore += mg_bishop_table[b.getPceR(WB, i)][b.getPceC(WB, i)];
        egScore += eg_bishop_table[b.getPceR(WB, i)][b.getPceC(WB, i)];
        gamePhase += BG;
    }
    //Looping through all white rooks
    for(int i = 0; i < b.getPceNum(WR); i++)
    {
        mgScore += RM;
        egScore += RE;

        int rR = b.getPceR(WR, i);
        int rC = b.getPceC(WR, i);
        mgScore += mg_rook_table[rR][rC];
        egScore += eg_rook_table[rR][rC];
        gamePhase += RG;

        mgScore += b.sqExposedVert(rR, rC) * 3;   //bonus for (semi)open file
    }

    //Looping through all white queens
    for(int i = 0; i < b.getPceNum(WQ); i++)
    {
        mgScore += QM;
        egScore += QE;
        mgScore += mg_queen_table[b.getPceR(WQ, i)][b.getPceC(WQ, i)];
        egScore += eg_queen_table[b.getPceR(WQ, i)][b.getPceC(WQ, i)];
        gamePhase += QG;
    }

    //King PST
    int kR = b.getKingR(WHITE);
    int kC = b.getKingC(WHITE);
    mgScore += mg_king_table[kR][kC];
    egScore += eg_king_table[kR][kC];
    mgScore -= b.sqExposed(kR, kC) * 3;       //king safety penalty
    
    //Looping through all black pawns
    for(int i = 0; i < b.getPceNum(BP); i++)
    {
        mgScore -= PM;
        egScore -= PE;
        mgScore -= mg_pawn_table[7 - b.getPceR(BP, i)][b.getPceC(BP, i)];
        egScore -= eg_pawn_table[7 - b.getPceR(BP, i)][b.getPceC(BP, i)];
        gamePhase += PG;
    }
    //Looping through all black knights
    for(int i = 0; i < b.getPceNum(BN); i++)
    {
        mgScore -= NM;
        egScore -= NE;
        mgScore -= mg_knight_table[7 - b.getPceR(BN, i)][b.getPceC(BN, i)];
        egScore -= eg_knight_table[7 - b.getPceR(BN, i)][b.getPceC(BN, i)];
        gamePhase += NG;
    }
    //Looping through all black bishops
    for(int i = 0; i < b.getPceNum(BB); i++)
    {
        mgScore -= BM;
        egScore -= BE;
        mgScore -= mg_bishop_table[7 - b.getPceR(BB, i)][b.getPceC(BB, i)];
        egScore -= eg_bishop_table[7 - b.getPceR(BB, i)][b.getPceC(BB, i)];
        gamePhase += BG;
    }
    //Looping through all black rooks
    for(int i = 0; i < b.getPceNum(BR); i++)
    {
        mgScore -= RM;
        egScore -= RE;

        int rR = b.getPceR(BR, i);
        int rC = b.getPceC(BR, i);
        mgScore -= mg_rook_table[7 - rR][rC];
        egScore -= eg_rook_table[7 - rR][rC];
        gamePhase += RG;

        mgScore -= b.sqExposedVert(rR, rC) * 3;   //bonus for (semi)open file
    }

    //Looping through all black queens
    for(int i = 0; i < b.getPceNum(BQ); i++)
    {
        mgScore -= QM;
        egScore -= QE;
        mgScore -= mg_queen_table[7 - b.getPceR(BQ, i)][b.getPceC(BQ, i)];
        egScore -= eg_queen_table[7 - b.getPceR(BQ, i)][b.getPceC(BQ, i)];
        gamePhase += QG;
    }
    
    //King PST
    kR = b.getKingR(BLACK);
    kC = b.getKingC(BLACK);
    mgScore -= mg_king_table[7 - kR][kC];
    egScore -= eg_king_table[7 - kR][kC];
    mgScore += b.sqExposed(kR, kC) * 3;       //king safety penalty

    //Tapered eval
    int mgPhase = gamePhase;
    if (mgPhase > 24) mgPhase = 24; //In case of early promotion
    int egPhase = 24 - mgPhase;
    int score = (mgScore * mgPhase + egScore * egPhase) / 24;
    
    return (b.getSide() == WHITE) ? (score) : (-score);
}

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <termios.h>

static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal escape sequence:  ESC [

void clearScreen()
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        std::cout << std::endl;
     else
        std::cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << std::flush;
}


//Used for RAND32
static std::random_device rd;
static std::default_random_engine generator(rd());
//Returns random 32 bit number
int RAND32()
{
    std::uniform_int_distribution<> distro(0, INT_MAX);
    return distro(generator);
}

