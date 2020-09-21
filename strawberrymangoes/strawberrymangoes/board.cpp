//
//  board.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "board.h"
#include "defs.h"

Board::Board()
{
    InitBoard();
    InitKeys();
}

int Board::getPosKey()
{
    return posKey;
}

void Board::addPiece(int r, int c, int pce)
{
    //XORing out the piece at (r, c)
    mBoard[r][c] = pce;
    posKey ^= pceKeys[r][c][pce];
}

void Board::removePiece(int r, int c)
{
    //XORing out the piece at (r, c)
    posKey ^= pceKeys[r][c][mBoard[r][c]];
    mBoard[r][c] = NOPIECE;
}

void Board::InitBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            mBoard[i][j] = NOPIECE;
        }
    }
}

void Board::InitKeys()
{
    posKey = 0;
    sideKey = RAND32();
    
    for (int i = 0; i < 4; i++)
    {
        castleKeys[i] = RAND32();
    }
    
    for (int i = 0; i < 64; i++)
    {
        enpasKey[i] = RAND32();
    }
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 12; k++)
            {
                pceKeys[i][j][k] = RAND32();
            }
        }
    }
}
