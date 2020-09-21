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
}

void Board::InitBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            mBoard[i][j] = 0;
        }
    }
}
