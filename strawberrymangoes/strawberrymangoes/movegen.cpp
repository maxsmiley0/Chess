//
//  movegen.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "movegen.h"

Movegen::Movegen(Board& b)
{
    mBoard = b;
}

void Movegen::generateMoves(int ply)
{
    int i = ply * MAXPOSITIONMOVES;
    for (int i = 0; i < 500; i++)
    {
        getMove(1, 9, 2, 6, 3);
    }
}

int Movegen::getMove(int sR, int sC, int eR, int eC, int promoted)
{
    int moveKey = 0;
    //From square handling
    moveKey |= sR;          //3 bits allocated for 8 combinations for starting rank
    moveKey |= (sC << 3);   //3 bits allocated for 8 combinations for starting file
    //To square handling
    moveKey |= (eR << 6);   //3 bits allocated for 8 combinations for ending rank
    moveKey |= (eC << 9);   //3 bits allocated for 8 combinations for ending file
    //Capture handling
    int capturedPce = mBoard.getPce(eR, eC);    //captured piece
    moveKey |= (capturedPce << 12);     //4 bits allocated for 16 pieces (upper bound)
    //Is en passant capture
    bool pawnMove = isPawn(mBoard.getPce(sR, sC));
    if (pawnMove && eR == mBoard.getEnpasSquareR() && eC == mBoard.getEnpasSquareC())
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
    if (isKing(mBoard.getPce(sR, sC)) && (sC == 4 && (eC == 2 || eC == 6)))
    {
        moveKey |= (1 << 22);
    }
    return moveKey;
}

bool Movegen::squareAttacked(int r, int c)
{
    int lBound;
    int uBound;
    
    //Loop through opposite color, so we can see what ISNT legal for this side
    if (mBoard.getSide() == WHITE)
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
    for (int i = lBound; i <= uBound; i++)
    {
        //Looping through all pieces of this type
        for (int j = 0; j < mBoard.getPceNum(i); j++)
        {
            //r and c of the piece being examined
            int pceR = mBoard.getPceR(i, j);
            int pceC = mBoard.getPceC(i, j);
            
            switch (i)
            {
                //Separate cases for white and black pawns due to "directionality"
                case WP:
                    if (pceC != 0 && r == pceR - 1 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (pceC != 7 && r == pceR - 1 && c == pceC + 1)
                    {
                        return true;
                    }
                    break;
                case BP:
                    
                    if (pceC != 0 && r == pceR + 1 && c == pceC - 1)
                    {
                        return true;
                    }
                    if (pceC != 7 && r == pceR + 1 && c == pceC + 1)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                            else if (mBoard.getPce(indexR, indexC) != NOPIECE)
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
                    break;
                default:
                    std::cerr << "Error in Movegen::generateThreats()" << std::endl;
                    break;
            }
        }
    }
    
    return false;
}

void Movegen::printAttacked()
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            if (squareAttacked(r, c))
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
