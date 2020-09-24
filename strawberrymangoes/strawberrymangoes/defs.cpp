//
//  defs.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "defs.h"
#include <random>
#include <utility>

bool isPawn(int pce)
{
    return (pce == WP || pce == BP);
}

bool isKing(int pce)
{
    return (pce == WK || pce == BK);
}

bool doDiag(int pce)
{
    return (pce == WB || pce == BB || pce == WQ || pce == BQ);
}

bool doVert(int pce)
{
    return (pce == WR || pce == BR || pce == WQ || pce == BQ);
}

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

std::string printMove(int move)
{
    std::string str = "";
    str += FileChar[fromC(move)];
    str += RankChar[fromR(move)];
    str += FileChar[toC(move)];
    str += RankChar[toR(move)];
    
    return str;
}

//Used for RAND32
static std::random_device rd;
static std::default_random_engine generator(rd());

int RAND32()
{
    std::uniform_int_distribution<> distro(0, INT_MAX);
    return distro(generator);
}

