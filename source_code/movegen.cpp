//
//  movegen.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "movegen.h"

Movegen::Movegen()
{
    mBoard = new Board();
    leafNodes = 0;
    
    perftTest(6);
}

Movegen::Movegen(const Movegen& other)
{
    this->mBoard = new Board(*other.getBoard());
}

Movegen::~Movegen()
{
    delete mBoard;
}

std::list<int> Movegen::generateMoves() const
{
    std::list<int> moveList;
    
    int lBound;
    int uBound;
    
    //Loop through all pieces of the color whose turn it is
    if (mBoard->getSide() == WHITE)
    {
        lBound = WP;
        uBound = WK;
    }
    else
    {
        lBound = BP;
        uBound = BK;
    }
    
    //Looping through all pieces of the side
    for (int i = lBound; i <= uBound; i++)
    {
        //Looping through all pieces of this type
        for (int j = 0; j < mBoard->getPceNum(i); j++)
        {
            //r and c of the piece being examined
            int r = mBoard->getPceR(i, j);
            int c = mBoard->getPceC(i, j);
            
            switch (i)
            {
                //Separate cases for white and black pawns due to "directionality"
                case WP:
                    //One square forward
                    if (mBoard->getPce(r - 1, c) == NOPIECE)
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moveList.push_back(getMove(r, c, r - 1, c, WN));
                            moveList.push_back(getMove(r, c, r - 1, c, WB));
                            moveList.push_back(getMove(r, c, r - 1, c, WR));
                            moveList.push_back(getMove(r, c, r - 1, c, WQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r - 1, c, NOPIECE));
                        }
                        //Two squares forward
                        if (r == 6 && mBoard->getPce(r - 2, c) == NOPIECE)
                        {
                            moveList.push_back(getMove(r, c, r - 2, c, NOPIECE));
                        }
                    }
                    //Capture cases
                    if (c != 0 && mBoard->getPce(r - 1, c - 1) != NOPIECE && PceCol(mBoard->getPce(r - 1, c - 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WN));
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WB));
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WR));
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r - 1, c - 1, NOPIECE));
                        }
                    }
                    if (c != 7 && mBoard->getPce(r - 1, c + 1) != NOPIECE && PceCol(mBoard->getPce(r - 1, c + 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WN));
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WB));
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WR));
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r - 1, c + 1, NOPIECE));
                        }
                        
                    }
                    //En passant case
                    if (c != 0 && r == 3 && mBoard->getEnpasSquareC() == c - 1 && mBoard->getEnpasSquareR() == r - 1)
                    {
                        moveList.push_back(getMove(r, c, r - 1, c - 1, NOPIECE));
                    }
                    if (c != 7 && r == 3 && mBoard->getEnpasSquareC() == c + 1 && mBoard->getEnpasSquareR() == r - 1)
                    {
                        moveList.push_back(getMove(r, c, r - 1, c + 1, NOPIECE));
                    }
                    break;
                case BP:
                    //One square forward
                    if (mBoard->getPce(r + 1, c) == NOPIECE)
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moveList.push_back(getMove(r, c, r + 1, c, BN));
                            moveList.push_back(getMove(r, c, r + 1, c, BB));
                            moveList.push_back(getMove(r, c, r + 1, c, BR));
                            moveList.push_back(getMove(r, c, r + 1, c, BQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r + 1, c, NOPIECE));
                        }
                        //Two squares forward
                        if (r == 1 && mBoard->getPce(r + 2, c) == NOPIECE)
                        {
                            moveList.push_back(getMove(r, c, r + 2, c, NOPIECE));
                        }
                    }
                    //Capture cases
                    if (c != 0 && mBoard->getPce(r + 1, c - 1) != NOPIECE && PceCol(mBoard->getPce(r + 1, c - 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BN));
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BB));
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BR));
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r + 1, c - 1, NOPIECE));
                        }
                    }
                    if (c != 7 && mBoard->getPce(r + 1, c + 1) != NOPIECE && PceCol(mBoard->getPce(r + 1, c + 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BN));
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BB));
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BR));
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r + 1, c + 1, NOPIECE));
                        }
                        
                    }
                    //En passant case
                    if (c != 0 && r == 4 && mBoard->getEnpasSquareC() == c - 1 && mBoard->getEnpasSquareR() == r + 1)
                    {
                        moveList.push_back(getMove(r, c, r + 1, c - 1, NOPIECE));
                    }
                    if (c != 7 && r == 4 && mBoard->getEnpasSquareC() == c + 1 && mBoard->getEnpasSquareR() == r + 1)
                    {
                        moveList.push_back(getMove(r, c, r + 1, c + 1, NOPIECE));
                    }
                    break;
                    
                case WN:
                case BN:
                    //8 squares that the knight might move to
                    //Can only move to a given square if empty or occupied by enemy pce
                    if (r >= 1 && c <= 5 && (mBoard->getPce(r - 1, c + 2) == NOPIECE || PceCol(mBoard->getPce(r - 1, c + 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c + 2, NOPIECE));
                    }
                    if (r <= 5 && c >= 1 && (mBoard->getPce(r + 2, c - 1) == NOPIECE || PceCol(mBoard->getPce(r + 2, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 2, c - 1, NOPIECE));
                    }
                    if (r >= 2 && c <= 6 && (mBoard->getPce(r - 2, c + 1) == NOPIECE || PceCol(mBoard->getPce(r - 2, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 2, c + 1, NOPIECE));
                    }
                    if (r <= 6 && c >= 2 && (mBoard->getPce(r + 1, c - 2) == NOPIECE || PceCol(mBoard->getPce(r + 1, c - 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c - 2, NOPIECE));
                    }
                    if (r >= 1 && c >= 2 && (mBoard->getPce(r - 1, c - 2) == NOPIECE || PceCol(mBoard->getPce(r - 1, c - 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c - 2, NOPIECE));
                    }
                    if (r >= 2 && c >= 1 && (mBoard->getPce(r - 2, c - 1) == NOPIECE || PceCol(mBoard->getPce(r - 2, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 2, c - 1, NOPIECE));
                    }
                    if (r <= 5 && c <= 6 && (mBoard->getPce(r + 2, c + 1) == NOPIECE || PceCol(mBoard->getPce(r + 2, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 2, c + 1, NOPIECE));
                    }
                    if (r <= 6 && c <= 5 && (mBoard->getPce(r + 1, c + 2) == NOPIECE || PceCol(mBoard->getPce(r + 1, c + 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c + 2, NOPIECE));
                    }
                    break;
                case WB:
                case BB:
                case WR:
                case BR:
                case WQ:
                case BQ:
                {
                    if (doDiag(i))
                    {
                        //Loop in 4 directions until we hit a piece or go offboard
                        int indexR = r + 1;
                        int indexC = c + 1;
                        //down right direction
                        while (indexR <= 7 && indexC <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR++;
                            indexC++;
                        }
                        //down left direction
                        indexR = r - 1;
                        indexC = c + 1;
                        
                        while (indexR >= 0 && indexC <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR--;
                            indexC++;
                        }
                        //up right direction
                        indexR = r + 1;
                        indexC = c - 1;
                        
                        while (indexR <= 7 && indexC >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR++;
                            indexC--;
                        }
                        //up left direction
                        indexR = r - 1;
                        indexC = c - 1;
                        
                        while (indexR >= 0 && indexC >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR--;
                            indexC--;
                        }
                    }
                    if (doVert(i))
                    {
                        //Loop in 4 directions until we hit a piece or go offboard
                        int indexR = r;
                        int indexC = c + 1;
                        //right direction
                        while (indexC <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexC++;
                        }
                        
                        indexR = r;
                        indexC = c - 1;
                        //left direction
                        while (indexC >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexC--;
                        }
                        
                        indexR = r - 1;
                        indexC = c;
                        //up direction
                        while (indexR >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR--;
                        }
                        
                        indexR = r + 1;
                        indexC = c;
                        //down direction
                        while (indexR <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) == NOPIECE)
                            {
                                moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                            }
                            else
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR++;
                        }
                    }
                    break;
                }
                case WK:
                case BK:
                    //8 squares that the king might move to
                    //Regular king move case
                    if (r >= 1 && c >= 1 && (mBoard->getPce(r - 1, c - 1) == NOPIECE || PceCol(mBoard->getPce(r - 1, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c - 1, NOPIECE));
                    }
                    if (r >= 1 && (mBoard->getPce(r - 1, c) == NOPIECE || PceCol(mBoard->getPce(r - 1, c)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c, NOPIECE));
                    }
                    if (r >= 1 && c <= 6 && (mBoard->getPce(r - 1, c + 1) == NOPIECE || PceCol(mBoard->getPce(r - 1, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c + 1, NOPIECE));
                    }
                    if (c <= 6 && (mBoard->getPce(r, c + 1) == NOPIECE || PceCol(mBoard->getPce(r, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r, c + 1, NOPIECE));
                    }
                    if (r <= 6 && c <= 6 && (mBoard->getPce(r + 1, c + 1) == NOPIECE || PceCol(mBoard->getPce(r + 1, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c + 1, NOPIECE));
                    }
                    if (r <= 6 && (mBoard->getPce(r + 1, c) == NOPIECE || PceCol(mBoard->getPce(r + 1, c)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c, NOPIECE));
                    }
                    if (r <= 6 && c >= 1 && (mBoard->getPce(r + 1, c - 1) == NOPIECE || PceCol(mBoard->getPce(r + 1, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c - 1, NOPIECE));
                    }
                    if (c >= 1 && (mBoard->getPce(r, c - 1) == NOPIECE || PceCol(mBoard->getPce(r, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r, c - 1, NOPIECE));
                    }
                    
                    //Castling case
                    if (mBoard->hasKcPerm())
                    {
                        if (mBoard->getPce(r, c + 1) == NOPIECE && mBoard->getPce(r, c + 2) == NOPIECE && !squareAttacked(r, c, PceCol(i)) && !squareAttacked(r, c + 1, PceCol(i)))
                        {
                            moveList.push_back(getMove(r, c, r, c + 2, NOPIECE));
                        }
                    }
                    if (mBoard->hasQcPerm())
                    {
                        if (mBoard->getPce(r, c - 1) == NOPIECE && mBoard->getPce(r, c - 2) == NOPIECE && mBoard->getPce(r, c - 3) == NOPIECE && !squareAttacked(r, c, PceCol(i)) && !squareAttacked(r, c - 1, PceCol(i)))
                        {
                            moveList.push_back(getMove(r, c, r, c - 2, NOPIECE));
                        }
                    }
                    break;
                default:
                    std::cerr << "Error in Movegen::generateMoves()" << std::endl;
                    break;
            }
        }
    }
    
    return moveList;
}

std::list<int> Movegen::generateCaptures() const
{
    std::list<int> moveList;
    
    int lBound;
    int uBound;
    
    //Loop through all pieces of the color whose turn it is
    if (mBoard->getSide() == WHITE)
    {
        lBound = WP;
        uBound = WK;
    }
    else
    {
        lBound = BP;
        uBound = BK;
    }
    
    //Looping through all pieces of the side
    for (int i = lBound; i <= uBound; i++)
    {
        //Looping through all pieces of this type
        for (int j = 0; j < mBoard->getPceNum(i); j++)
        {
            //r and c of the piece being examined
            int r = mBoard->getPceR(i, j);
            int c = mBoard->getPceC(i, j);
            
            switch (i)
            {
                //Separate cases for white and black pawns due to "directionality"
                case WP:
                    //Capture cases
                    if (c != 0 && mBoard->getPce(r - 1, c - 1) != NOPIECE && PceCol(mBoard->getPce(r - 1, c - 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WN));
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WB));
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WR));
                            moveList.push_back(getMove(r, c, r - 1, c - 1, WQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r - 1, c - 1, NOPIECE));
                        }
                    }
                    if (c != 7 && mBoard->getPce(r - 1, c + 1) != NOPIECE && PceCol(mBoard->getPce(r - 1, c + 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WN));
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WB));
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WR));
                            moveList.push_back(getMove(r, c, r - 1, c + 1, WQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r - 1, c + 1, NOPIECE));
                        }
                        
                    }
                    //En passant case
                    if (c != 0 && r == 3 && mBoard->getEnpasSquareC() == c - 1 && mBoard->getEnpasSquareR() == r - 1)
                    {
                        moveList.push_back(getMove(r, c, r - 1, c - 1, NOPIECE));
                    }
                    if (c != 7 && r == 3 && mBoard->getEnpasSquareC() == c + 1 && mBoard->getEnpasSquareR() == r - 1)
                    {
                        moveList.push_back(getMove(r, c, r - 1, c + 1, NOPIECE));
                    }
                    break;
                case BP:
                    //Capture cases
                    if (c != 0 && mBoard->getPce(r + 1, c - 1) != NOPIECE && PceCol(mBoard->getPce(r + 1, c - 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BN));
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BB));
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BR));
                            moveList.push_back(getMove(r, c, r + 1, c - 1, BQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r + 1, c - 1, NOPIECE));
                        }
                    }
                    if (c != 7 && mBoard->getPce(r + 1, c + 1) != NOPIECE && PceCol(mBoard->getPce(r + 1, c + 1)) != mBoard->getSide())
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BN));
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BB));
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BR));
                            moveList.push_back(getMove(r, c, r + 1, c + 1, BQ));
                        }
                        //General case
                        else
                        {
                            moveList.push_back(getMove(r, c, r + 1, c + 1, NOPIECE));
                        }
                        
                    }
                    //En passant case
                    if (c != 0 && r == 4 && mBoard->getEnpasSquareC() == c - 1 && mBoard->getEnpasSquareR() == r + 1)
                    {
                        moveList.push_back(getMove(r, c, r + 1, c - 1, NOPIECE));
                    }
                    if (c != 7 && r == 4 && mBoard->getEnpasSquareC() == c + 1 && mBoard->getEnpasSquareR() == r + 1)
                    {
                        moveList.push_back(getMove(r, c, r + 1, c + 1, NOPIECE));
                    }
                    break;
                    
                case WN:
                case BN:
                    //8 squares that the knight might move to
                    //Can only move to a given square if empty or occupied by enemy pce
                    if (r >= 1 && c <= 5 && (mBoard->getPce(r - 1, c + 2) != NOPIECE && PceCol(mBoard->getPce(r - 1, c + 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c + 2, NOPIECE));
                    }
                    if (r <= 5 && c >= 1 && (mBoard->getPce(r + 2, c - 1) != NOPIECE && PceCol(mBoard->getPce(r + 2, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 2, c - 1, NOPIECE));
                    }
                    if (r >= 2 && c <= 6 && (mBoard->getPce(r - 2, c + 1) != NOPIECE && PceCol(mBoard->getPce(r - 2, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 2, c + 1, NOPIECE));
                    }
                    if (r <= 6 && c >= 2 && (mBoard->getPce(r + 1, c - 2) != NOPIECE && PceCol(mBoard->getPce(r + 1, c - 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c - 2, NOPIECE));
                    }
                    if (r >= 1 && c >= 2 && (mBoard->getPce(r - 1, c - 2) != NOPIECE && PceCol(mBoard->getPce(r - 1, c - 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c - 2, NOPIECE));
                    }
                    if (r >= 2 && c >= 1 && (mBoard->getPce(r - 2, c - 1) != NOPIECE && PceCol(mBoard->getPce(r - 2, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 2, c - 1, NOPIECE));
                    }
                    if (r <= 5 && c <= 6 && (mBoard->getPce(r + 2, c + 1) != NOPIECE && PceCol(mBoard->getPce(r + 2, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 2, c + 1, NOPIECE));
                    }
                    if (r <= 6 && c <= 5 && (mBoard->getPce(r + 1, c + 2) != NOPIECE && PceCol(mBoard->getPce(r + 1, c + 2)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c + 2, NOPIECE));
                    }
                    break;
                case WB:
                case BB:
                case WR:
                case BR:
                case WQ:
                case BQ:
                {
                    if (doDiag(i))
                    {
                        //Loop in 4 directions until we hit a piece or go offboard
                        int indexR = r + 1;
                        int indexC = c + 1;
                        //down right direction
                        while (indexR <= 7 && indexC <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR++;
                            indexC++;
                        }
                        //down left direction
                        indexR = r - 1;
                        indexC = c + 1;
                        
                        while (indexR >= 0 && indexC <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR--;
                            indexC++;
                        }
                        //up right direction
                        indexR = r + 1;
                        indexC = c - 1;
                        
                        while (indexR <= 7 && indexC >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR++;
                            indexC--;
                        }
                        //up left direction
                        indexR = r - 1;
                        indexC = c - 1;
                        
                        while (indexR >= 0 && indexC >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR--;
                            indexC--;
                        }
                    }
                    if (doVert(i))
                    {
                        //Loop in 4 directions until we hit a piece or go offboard
                        int indexR = r;
                        int indexC = c + 1;
                        //right direction
                        while (indexC <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexC++;
                        }
                        
                        indexR = r;
                        indexC = c - 1;
                        //left direction
                        while (indexC >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexC--;
                        }
                        
                        indexR = r - 1;
                        indexC = c;
                        //up direction
                        while (indexR >= 0)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR--;
                        }
                        
                        indexR = r + 1;
                        indexC = c;
                        //down direction
                        while (indexR <= 7)
                        {
                            if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                if (PceCol(mBoard->getPce(indexR, indexC)) != mBoard->getSide())
                                {
                                    moveList.push_back(getMove(r, c, indexR, indexC, NOPIECE));
                                }
                                break;
                            }
                            indexR++;
                        }
                    }
                    break;
                }
                case WK:
                case BK:
                    //8 squares that the king might move to
                    //Regular king move case
                    if (r >= 1 && c >= 1 && (mBoard->getPce(r - 1, c - 1) != NOPIECE && PceCol(mBoard->getPce(r - 1, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c - 1, NOPIECE));
                    }
                    if (r >= 1 && (mBoard->getPce(r - 1, c) != NOPIECE && PceCol(mBoard->getPce(r - 1, c)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c, NOPIECE));
                    }
                    if (r >= 1 && c <= 6 && (mBoard->getPce(r - 1, c + 1) != NOPIECE && PceCol(mBoard->getPce(r - 1, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r - 1, c + 1, NOPIECE));
                    }
                    if (c <= 6 && (mBoard->getPce(r, c + 1) != NOPIECE && PceCol(mBoard->getPce(r, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r, c + 1, NOPIECE));
                    }
                    if (r <= 6 && c <= 6 && (mBoard->getPce(r + 1, c + 1) != NOPIECE && PceCol(mBoard->getPce(r + 1, c + 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c + 1, NOPIECE));
                    }
                    if (r <= 6 && (mBoard->getPce(r + 1, c) != NOPIECE && PceCol(mBoard->getPce(r + 1, c)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c, NOPIECE));
                    }
                    if (r <= 6 && c >= 1 && (mBoard->getPce(r + 1, c - 1) != NOPIECE && PceCol(mBoard->getPce(r + 1, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r + 1, c - 1, NOPIECE));
                    }
                    if (c >= 1 && (mBoard->getPce(r, c - 1) != NOPIECE && PceCol(mBoard->getPce(r, c - 1)) != mBoard->getSide()))
                    {
                        moveList.push_back(getMove(r, c, r, c - 1, NOPIECE));
                    }
                    break;
                default:
                    std::cerr << "Error in Movegen::generateCaptures()" << std::endl;
                    break;
            }
        }
    }
    return moveList;
}

bool Movegen::squareAttacked(int r, int c, int side) const
{
    int lBound;
    int uBound;
    
    //Loop through opposite color, so we can see what ISNT legal for this side
    if (side == WHITE)
    {
        lBound = BP;
        uBound = BK;
    }
    else
    {
        lBound = WP;
        uBound = WK;
    }
    
    //Looping through all pieces of the opposite side
    for (int i = uBound; i >= lBound; i--)
    {
        //Looping through all pieces of this type
        for (int j = 0; j < mBoard->getPceNum(i); j++)
        {
            //r and c of the piece being examined
            int pceR = mBoard->getPceR(i, j);
            int pceC = mBoard->getPceC(i, j);
            
            switch (i)
            {
                //Separate cases for white and black pawns due to "directionality"
                case WP:
                    if (r == pceR - 1 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (r == pceR - 1 && c == pceC + 1)
                    {
                        return true;
                    }
                    break;
                case BP:
                    
                    if (r == pceR + 1 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (r == pceR + 1 && c == pceC + 1)
                    {
                        return true;
                    }
                    break;
                case WN:
                case BN:
                    //8 squares that the knight might move to
                    if (pceR >= 1 && pceC <= 5 && r == pceR - 1 && c == pceC + 2)
                    {
                        return true;
                    }
                    if (pceR <= 5 && pceC >= 1 && r == pceR + 2 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (pceR >= 2 && pceC <= 6 && r == pceR - 2 && c == pceC + 1)
                    {
                        return true;
                    }
                    if (pceR <= 6 && pceC >= 2 && r == pceR + 1 && c == pceC - 2)
                    {
                        return true;
                    }
                    if (pceR >= 1 && pceC >= 2 && r == pceR - 1 && c == pceC - 2)
                    {
                        return true;
                    }
                    if (pceR >= 2 && pceC >= 1 && r == pceR - 2 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (pceR <= 5 && pceC <= 6 && r == pceR + 2 && c == pceC + 1)
                    {
                        return true;
                    }
                    if (pceR <= 6 && pceC <= 5 && r == pceR + 1 && c == pceC + 2)
                    {
                        return true;
                    }
                    break;
                case WB:
                case BB:
                case WR:
                case BR:
                case WQ:
                case BQ:
                {
                    if (doDiag(i))
                    {
                        //Loop in 4 directions until we hit a piece or go offboard
                        int indexR = pceR + 1;
                        int indexC = pceC + 1;
                        
                        while (indexR <= 7 && indexC <= 7)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexR++;
                            indexC++;
                        }
                        
                        indexR = pceR + 1;
                        indexC = pceC - 1;
                    
                        while (indexR <= 7 && indexC >= 0)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexR++;
                            indexC--;
                        }
                        
                        indexR = pceR - 1;
                        indexC = pceC + 1;
                        
                        while (indexR >= 0 && indexC <= 7)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexR--;
                            indexC++;
                        }
                        
                        indexR = pceR - 1;
                        indexC = pceC - 1;
                        
                        while (indexR >= 0 && indexC >= 0)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexR--;
                            indexC--;
                        }
                    }
                    if (doVert(i))
                    {
                        //Loop in 4 directions until we hit a piece or go offboard
                        int indexR = pceR;
                        int indexC = pceC + 1;
                        
                        while (indexC <= 7)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexC++;
                        }
                        
                        indexR = pceR;
                        indexC = pceC - 1;
                    
                        while (indexC >= 0)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexC--;
                        }
                        
                        indexR = pceR - 1;
                        indexC = pceC;
                        
                        while (indexR >= 0)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexR--;
                        }
                        
                        indexR = pceR + 1;
                        indexC = pceC;
                        
                        while (indexR <= 7)
                        {
                            if (r == indexR && c == indexC)
                            {
                                return true;
                            }
                            else if (mBoard->getPce(indexR, indexC) != NOPIECE)
                            {
                                break;
                            }
                            indexR++;
                        }
                    }
                    break;
                }
                case WK:
                case BK:
                    //8 squares that the king might move to
                    if (pceR >= 1 && pceC >= 1 && r == pceR - 1 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (pceR >= 1 && r == pceR - 1 && c == pceC)
                    {
                        return true;
                    }
                    if (pceR >= 1 && pceC <= 6 && r == pceR - 1 && c == pceC + 1)
                    {
                        return true;
                    }
                    if (pceC <= 6 && r == pceR && c == pceC + 1)
                    {
                        return true;
                    }
                    if (pceR <= 6 && pceC <= 6 && r == pceR + 1 && c == pceC + 1)
                    {
                        return true;
                    }
                    if (pceR <= 6 && r == pceR + 1 && c == pceC)
                    {
                        return true;
                    }
                    if (pceR <= 6 && pceC >= 1 && r == pceR + 1 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (pceC >= 1 && r == pceR && c == pceC - 1)
                    {
                        return true;
                    }
                    break;
                default:
                    std::cerr << "Error in Movegen::generateThreats()" << std::endl;
                    break;
            }
        }
    }
    
    return false;
}

bool Movegen::makeMove(int move)
{
    mBoard->pushHistory(move);
    
    int side = mBoard->getSide();
    int fR = fromR(move);
    int fC = fromC(move);
    int tR = toR(move);
    int tC = toC(move);
    
    int pce = mBoard->getPce(fR, fC);        //piece being moved
    
    //Handling special cases first
    //En passant case
    if (isEnpasMove(move))
    {
        //Delete the pawn behind the finish square
        if (side == WHITE)
        {
            mBoard->removePiece(tR + 1, tC);
        }
        else
        {
            mBoard->removePiece(tR - 1, tC);
        }
    }
    //Castling case
    else if (isCastleMove(move))
    {
        //Need to move the rooks
        if (side == WHITE)
        {
            if (tC == 2)    //WQCA
            {
                mBoard->removePiece(7, 0);
                mBoard->addPiece(7, 3, WR);
            }
            else            //WKCA
            {
                mBoard->removePiece(7, 7);
                mBoard->addPiece(7, 5, WR);
            }
        }
        else
        {
            if (tC == 2)    //BQCA
            {
                mBoard->removePiece(0, 0);
                mBoard->addPiece(0, 3, BR);
            }
            else            //BKCA
            {
                mBoard->removePiece(0, 7);
                mBoard->addPiece(0, 5, BR);
            }
        }
    }
    
    mBoard->hashOutEp();     //hashes out ep square, if such a square exists
    //Updating castling perms
    //If king moves, loses all castling perms for that side
    if (isKing(pce))
    {
        if (mBoard->getSide() == WHITE)
        {
            mBoard->hashOutCastle(WKCA);
            mBoard->hashOutCastle(WQCA);
        }
        else
        {
            mBoard->hashOutCastle(BKCA);
            mBoard->hashOutCastle(BQCA);
        }
    }
    
    //If any piece moves from the initial rook square, or to that square, that side will lose one of its castle perms
    if ((fR == 0 && fC == 0) || (tR == 0 && tC == 0))
    {
        mBoard->hashOutCastle(BQCA);
    }
    if ((fR == 0 && fC == 7) || (tR == 0 && tC == 7))
    {
        if (move == 3195343)
        {
            std::cerr << "hereewe" << std::endl;
        }
        
        mBoard->hashOutCastle(BKCA);
    }
    if ((fR == 7 && fC == 0) || (tR == 7 && tC == 0))
    {
        mBoard->hashOutCastle(WQCA);
    }
    if ((fR == 7 && fC == 7) || (tR == 7 && tC == 7))
    {
        mBoard->hashOutCastle(WKCA);
    }
    //Resetting enpas square if it was a starting pawn move
    if (isPawnstartMove(move))
    {
        if (side == WHITE)
        {
            mBoard->hashInEp(tR + 1, tC);
        }
        else
        {
            mBoard->hashInEp(tR - 1, tC);
        }
    }
    //Remove the piece we want to move
    mBoard->removePiece(fromR(move), fromC(move));
    //If we have captured some piece
    if (captured(move) != NOPIECE)
    {
        //Remove that piece
        mBoard->removePiece(toR(move), toC(move));
    }
    if (promoted(move) != NOPIECE)
    {
        mBoard->addPiece(toR(move), toC(move), promoted(move));
    }
    else
    {
        mBoard->addPiece(toR(move), toC(move), pce);
    }
    
    mBoard->changeSide();
    
    if (squareAttacked(mBoard->getKingR(side), mBoard->getKingC(side), side))
    {
        takeBack();
        return false;
    }
    //Legal move
    return true;
}

void Movegen::takeBack()
{
    History lastState = mBoard->getLastState();
    int move = lastState.move;
    
    int fR = fromR(move);
    int fC = fromC(move);
    int tR = toR(move);
    int tC = toC(move);
    
    int pce = lastState.pce;    //stores the MOVED piece
    if (pce == NOPIECE)
    {
        mBoard->printBoard();
        std::cerr << fR << ' ' << fC << std::endl;
        std::cerr << tR << ' ' << tC << std::endl;
        std::cerr << "something terrible has happened" << std::endl;
        exit(1);
    }
    mBoard->changeSide();
    
    //Handling special cases
    //En passant case
    if (isEnpasMove(move))
    {
        //Add the pawn behind the finish square
        if (mBoard->getSide() == WHITE)
        {
            mBoard->addPiece(tR + 1, tC, BP);
        }
        else
        {
            mBoard->addPiece(tR - 1, tC, WP);
        }
    }
    //Castling case
    else if (isCastleMove(move))
    {
        //Need to move the rooks
        if (mBoard->getSide() == WHITE)
        {
            if (tC == 2)    //WQCA
            {
                mBoard->removePiece(7, 3);
                mBoard->addPiece(7, 0, WR);
            }
            else            //WKCA
            {
                mBoard->removePiece(7, 5);
                mBoard->addPiece(7, 7, WR);
            }
        }
        else
        {
            if (tC == 2)    //BQCA
            {
                mBoard->removePiece(0, 3);
                mBoard->addPiece(0, 0, BR);
            }
            else            //BKCA
            {
                mBoard->removePiece(0, 5);
                mBoard->addPiece(0, 7, BR);
            }
        }
    }
    //Hashing in en passant / castling perms to the key
    mBoard->hashOutEp();
    if (lastState.enpasSquareR != OFFBOARD)
    {
        mBoard->hashInEp(lastState.enpasSquareR, lastState.enpasSquareC);
    }
    if ((lastState.castlePerm & WKCA) != 0) //if wkca last move
    {
        mBoard->hashInCastle(WKCA);
    }
    if ((lastState.castlePerm & WQCA) != 0) //if wqca last move
    {
        mBoard->hashInCastle(WQCA);
    }
    if ((lastState.castlePerm & BKCA) != 0) //if bkca last move
    {
        mBoard->hashInCastle(BKCA);
    }
    if ((lastState.castlePerm & BQCA) != 0) //if bqca last move
    {
        mBoard->hashInCastle(BQCA);
    }
    
    //Promotion Check
    if (promoted(move) != NOPIECE)
    {
        if (PceCol(pce) == WHITE)
        {
            mBoard->removePiece(tR, tC);
            mBoard->addPiece(tR, tC, WP);
        }
        else
        {
            mBoard->removePiece(tR, tC);
            mBoard->addPiece(tR, tC, BP);
        }
    }
    
    //Move piece back to original square
    mBoard->removePiece(tR, tC);
    
    if (promoted(move) == NOPIECE)
    {
        mBoard->addPiece(fR, fC, pce);
    }
    else
    {
        (PceCol(pce) == WHITE) ? (mBoard->addPiece(fR, fC, WP)) : (mBoard->addPiece(fR, fC, BP));
    }
    
    //Add captured piece, if any
    if (captured(move) != NOPIECE)
    {
        mBoard->addPiece(tR, tC, captured(move));
    }
    
    mBoard->popHistory();
}

int Movegen::getMove(int sR, int sC, int eR, int eC, int promoted) const
{
    int moveKey = 0;
    //From square handling
    moveKey |= sR;          //3 bits allocated for 8 combinations for starting rank
    moveKey |= (sC << 3);   //3 bits allocated for 8 combinations for starting file
    //To square handling
    moveKey |= (eR << 6);   //3 bits allocated for 8 combinations for ending rank
    moveKey |= (eC << 9);   //3 bits allocated for 8 combinations for ending file
    //Capture handling
    int capturedPce = mBoard->getPce(eR, eC);    //captured piece
    moveKey |= (capturedPce << 12);     //4 bits allocated for 16 pieces (upper bound)
    //Is en passant capture
    bool pawnMove = isPawn(mBoard->getPce(sR, sC));
    if (pawnMove && eR == mBoard->getEnpasSquareR() && eC == mBoard->getEnpasSquareC() && (sR == 3 || sR == 4))
    {
        moveKey |= (1 << 16);   //1 bit allocated for true or false
    }
    //Is pawn start move
    if (pawnMove && abs(eR - sR) == 2)
    {
        moveKey |= (1 << 17);   //1 bit allocated for true or false
    }
    //Promoted piece
    moveKey |= (promoted << 18);    //4 bits allocated for piece
    //Is a castling move
    if (isKing(mBoard->getPce(sR, sC)) && (sC == 4 && (eC == 2 || eC == 6)))
    {
        moveKey |= (1 << 22);
    }
    return moveKey;
}

//Perft testing functions, used to verify the integrity of the legal move generator
void Movegen::perft(int depth)
{
    if (depth == 0)
    {
        leafNodes++;
    }
    else
    {
        std::list<int> moves = generateMoves();
        
        for (std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
        {
            if (makeMove(*itr))
            {
                perft(depth - 1);
                takeBack();
            }
        }
    }
}

void Movegen::perftTest(int depth)
{
    int totalNodes = 0;
    leafNodes = 0;
    std::list<int> moves = generateMoves();
    int moveNum = 0;
    for (std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        if (makeMove(*itr))
        {
            moveNum++;
            int cumnodes = leafNodes;
            perft(depth - 1);
            takeBack();
            totalNodes += (leafNodes - cumnodes);
            std::cout << "Move " << moveNum << ' ' << printMove(*itr) << ' ' << *itr << std::endl << (leafNodes - cumnodes) << std::endl;
        }
    }
}

void Movegen::printMoves(std::list<int> moves)
{
    for (std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        int move = *itr;
        
        std::cout << "Move: " << move << std::endl;
        std::cout << "From: " << fromR(move) << ' ' << fromC(move) << std::endl;
        std::cout << "TO: " << toR(move) << ' ' << toC(move) << std::endl;
        std::cout << "Cap: " << PceChar[captured(move)] << std::endl;
        std::cout << "Prom: " << PceChar[promoted(move)] << std::endl;
        std::cout << "isep: " << isEnpasMove(move) << std::endl;
        std::cout << "ispa: " << isPawnstartMove(move) << std::endl;
        std::cout << "isca: " << isCastleMove(move) << std::endl;
        std::cout << std::endl;
    }
}

void Movegen::printAttacked()
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (squareAttacked(r, c, mBoard->getSide()))
            {
                std::cout << "X   ";
            }
            else
            {
                std::cout << ".   ";
            }
        }
        std::cout << std::endl << std::endl;
    }
}
