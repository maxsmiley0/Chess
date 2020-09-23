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
    int moveIndex = ply * MAXPOSITIONMOVES;
    int lBound;
    int uBound;
    
    //Loop through all pieces of the color whose turn it is
    if (mBoard.getSide() == WHITE)
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
        for (int j = 0; j < mBoard.getPceNum(i); j++)
        {
            //r and c of the piece being examined
            int r = mBoard.getPceR(i, j);
            int c = mBoard.getPceC(i, j);
            
            switch (i)
            {
                //Separate cases for white and black pawns due to "directionality"
                case WP:
                    //One square forward
                    if (mBoard.getPce(r - 1, c) == NOPIECE)
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moves[moveIndex] = getMove(r, c, r - 1, c, WN);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c, WB);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c, WR);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c, WQ);
                            moveIndex++;
                        }
                        //General case
                        else
                        {
                            moves[moveIndex] = getMove(r, c, r - 1, c, NOPIECE);
                            moveIndex++;
                        }
                        //Two squares forward
                        if (r == 6 && mBoard.getPce(r - 2, c) == NOPIECE)
                        {
                            moves[moveIndex] = getMove(r, c, r - 2, c, NOPIECE);
                            moveIndex++;
                        }
                    }
                    //Capture cases
                    if (c != 0 && mBoard.getPce(r - 1, c - 1) != NOPIECE && PceCol(mBoard.getPce(r - 1, c - 1)) != mBoard.getSide())
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moves[moveIndex] = getMove(r, c, r - 1, c - 1, WN);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c - 1, WB);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c - 1, WR);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c - 1, WQ);
                            moveIndex++;
                        }
                        //General case
                        else
                        {
                            moves[moveIndex] = getMove(r, c, r - 1, c - 1, NOPIECE);
                            moveIndex++;
                        }
                    }
                    if (c != 7 && mBoard.getPce(r - 1, c + 1) != NOPIECE && PceCol(mBoard.getPce(r - 1, c + 1)) != mBoard.getSide())
                    {
                        //Promotion case
                        if (r == 1)
                        {
                            moves[moveIndex] = getMove(r, c, r - 1, c + 1, WN);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c + 1, WB);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c + 1, WR);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r - 1, c + 1, WQ);
                            moveIndex++;
                        }
                        //General case
                        else
                        {
                            moves[moveIndex] = getMove(r, c, r - 1, c + 1, NOPIECE);
                            moveIndex++;
                        }
                        
                    }
                    //En passant case
                    if (c != 0 && r == 3 && mBoard.getEnpasSquareC() == c - 1 && mBoard.getEnpasSquareR() == r - 1)
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    if (c != 7 && r == 3 && mBoard.getEnpasSquareC() == c + 1 && mBoard.getEnpasSquareR() == r - 1)
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    break;
                case BP:
                    //One square forward
                    if (mBoard.getPce(r + 1, c) == NOPIECE)
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moves[moveIndex] = getMove(r, c, r + 1, c, BN);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c, BB);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c, BR);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c, BQ);
                            moveIndex++;
                        }
                        //General case
                        else
                        {
                            moves[moveIndex] = getMove(r, c, r + 1, c, NOPIECE);
                            moveIndex++;
                        }
                        //Two squares forward
                        if (r == 1 && mBoard.getPce(r + 2, c) == NOPIECE)
                        {
                            moves[moveIndex] = getMove(r, c, r + 2, c, NOPIECE);
                            moveIndex++;
                        }
                    }
                    //Capture cases
                    if (c != 0 && mBoard.getPce(r + 1, c - 1) != NOPIECE && PceCol(mBoard.getPce(r + 1, c - 1)) != mBoard.getSide())
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moves[moveIndex] = getMove(r, c, r + 1, c - 1, BN);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c - 1, BB);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c - 1, BR);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c - 1, BQ);
                            moveIndex++;
                        }
                        //General case
                        else
                        {
                            moves[moveIndex] = getMove(r, c, r + 1, c - 1, NOPIECE);
                            moveIndex++;
                        }
                    }
                    if (c != 7 && mBoard.getPce(r + 1, c + 1) != NOPIECE && PceCol(mBoard.getPce(r + 1, c + 1)) != mBoard.getSide())
                    {
                        //Promotion case
                        if (r == 6)
                        {
                            moves[moveIndex] = getMove(r, c, r + 1, c + 1, BN);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c + 1, BB);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c + 1, BR);
                            moveIndex++;
                            moves[moveIndex] = getMove(r, c, r + 1, c + 1, BQ);
                            moveIndex++;
                        }
                        //General case
                        else
                        {
                            moves[moveIndex] = getMove(r, c, r + 1, c + 1, NOPIECE);
                            moveIndex++;
                        }
                        
                    }
                    //En passant case
                    if (c != 0 && r == 4 && mBoard.getEnpasSquareC() == c - 1 && mBoard.getEnpasSquareR() == r + 1)
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    if (c != 7 && r == 4 && mBoard.getEnpasSquareC() == c + 1 && mBoard.getEnpasSquareR() == r + 1)
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    break;
                    
                case WN:
                case BN:
                    //8 squares that the knight might move to
                    //Can only move to a given square if empty or occupied by enemy pce
                    if (r >= 1 && c <= 5 && (mBoard.getPce(r - 1, c + 2) == NOPIECE || PceCol(mBoard.getPce(r - 1, c + 2)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c + 2, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 5 && c >= 1 && (mBoard.getPce(r + 2, c - 1) == NOPIECE || PceCol(mBoard.getPce(r + 2, c - 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 2, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r >= 2 && c <= 6 && (mBoard.getPce(r - 2, c + 1) == NOPIECE || PceCol(mBoard.getPce(r - 2, c + 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 2, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 6 && c >= 2 && (mBoard.getPce(r + 1, c - 2) == NOPIECE || PceCol(mBoard.getPce(r + 1, c - 2)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c - 2, NOPIECE);
                        moveIndex++;
                    }
                    if (r >= 1 && c >= 2 && (mBoard.getPce(r - 1, c - 2) == NOPIECE || PceCol(mBoard.getPce(r - 1, c - 2)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c - 2, NOPIECE);
                        moveIndex++;
                    }
                    if (r >= 2 && c >= 1 && (mBoard.getPce(r - 2, c - 1) == NOPIECE || PceCol(mBoard.getPce(r - 2, c - 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 2, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 5 && c <= 6 && (mBoard.getPce(r + 2, c + 1) == NOPIECE || PceCol(mBoard.getPce(r + 2, c + 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 2, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 6 && c <= 5 && (mBoard.getPce(r + 1, c + 2) == NOPIECE || PceCol(mBoard.getPce(r + 1, c + 2)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c + 2, NOPIECE);
                        moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                            if (mBoard.getPce(indexR, indexC) == NOPIECE)
                            {
                                moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                moveIndex++;
                            }
                            else
                            {
                                if (PceCol(mBoard.getPce(indexR, indexC)) != mBoard.getSide())
                                {
                                    moves[moveIndex] = getMove(r, c, indexR, indexC, NOPIECE);
                                    moveIndex++;
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
                    if (r >= 1 && c >= 1 && (mBoard.getPce(r - 1, c - 1) == NOPIECE || PceCol(mBoard.getPce(r - 1, c - 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r >= 1 && (mBoard.getPce(r - 1, c) == NOPIECE || PceCol(mBoard.getPce(r - 1, c)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c, NOPIECE);
                        moveIndex++;
                    }
                    if (r >= 1 && c <= 6 && (mBoard.getPce(r - 1, c + 1) == NOPIECE || PceCol(mBoard.getPce(r - 1, c + 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r - 1, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    if (c <= 6 && (mBoard.getPce(r, c + 1) == NOPIECE || PceCol(mBoard.getPce(r, c + 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 6 && c <= 6 && (mBoard.getPce(r + 1, c + 1) == NOPIECE || PceCol(mBoard.getPce(r + 1, c + 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c + 1, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 6 && (mBoard.getPce(r + 1, c) == NOPIECE || PceCol(mBoard.getPce(r + 1, c)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c, NOPIECE);
                        moveIndex++;
                    }
                    if (r <= 6 && c >= 1 && (mBoard.getPce(r + 1, c - 1) == NOPIECE || PceCol(mBoard.getPce(r + 1, c - 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r + 1, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    if (c >= 1 && (mBoard.getPce(r, c - 1) == NOPIECE || PceCol(mBoard.getPce(r, c - 1)) != mBoard.getSide()))
                    {
                        moves[moveIndex] = getMove(r, c, r, c - 1, NOPIECE);
                        moveIndex++;
                    }
                    
                    //Castling case
                    if (mBoard.hasKcPerm())
                    {
                        if (mBoard.getPce(r, c + 1) == NOPIECE && mBoard.getPce(r, c + 2) == NOPIECE && !squareAttacked(r, c) && !squareAttacked(r, c + 1))
                        {
                            moves[moveIndex] = getMove(r, c, r, c + 2, NOPIECE);
                            moveIndex++;
                        }
                    }
                    if (mBoard.hasQcPerm())
                    {
                        if (mBoard.getPce(r, c - 1) == NOPIECE && mBoard.getPce(r, c - 2) == NOPIECE && !squareAttacked(r, c) && !squareAttacked(r, c - 1))
                        {
                            moves[moveIndex] = getMove(r, c, r, c - 2, NOPIECE);
                            moveIndex++;
                        }
                    }
                    break;
                default:
                    std::cerr << "Error in Movegen::generateThreats()" << std::endl;
                    break;
            }
        }
    }
    moves[moveIndex] = 0;   //signalling end of move generation
}

void Movegen::printMoves(int ply)
{
    for (int i = ply * MAXPOSITIONMOVES; moves[i] != 0; i++)
    {
        int move = moves[i];
        
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
    if (pawnMove && eR == mBoard.getEnpasSquareR() && eC == mBoard.getEnpasSquareC() && (sR == 3 || sR == 4))
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

bool Movegen::makeMove(int move)
{
    mBoard.pushHistory(move);
    
    //Handling special cases first
    
    //Store the pce for later use
    int pce = mBoard.getPce(fromR(move), fromC(move));
    //Remove the piece we want to move
    mBoard.removePiece(fromR(move), fromC(move));
    //If we have captured some piece
    if (captured(move) != NOPIECE)
    {
        //Remove that piece
        mBoard.removePiece(toR(move), toC(move));
    }
    if (promoted(move) != NOPIECE)
    {
        mBoard.addPiece(toR(move), toC(move), promoted(move));
    }
    else
    {
        mBoard.addPiece(toR(move), toC(move), pce);
    }
    
    if (squareAttacked(mBoard.getKingR(mBoard.getSide()), mBoard.getKingC(mBoard.getSide())))
    {
        //need to undo all of that...
        return false;
    }
    //Move the piece to where it belongs
    return true;
}

bool takeMove()
{
    //What are steps?
    /*
    Have to undo the move, using history array
     */
    return false;
}