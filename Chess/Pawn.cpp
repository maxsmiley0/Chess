//
//  Pawn.cpp
//  Chess
//
//  Created by Max Smiley on 6/29/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "Pawn.h"

#include <iostream>

#include "Pawn.h"
#include "Set.h"
#include "Board.h"

using namespace std;

Pawn::Pawn(Board* b, Coord c, char col) : Piece(b, c, col)
{
    
}

Pawn::~Pawn()
{
    
}

bool Pawn::canReachEnemyKing() const
{
    int dir;
    
    if (getColor() == 'W')
    {
        dir = -1;
    }
    else if (getColor() == 'B')
    {
        dir = 1;
    }
    else
    {
        cerr << "Invalid color specifier" << endl;
        exit(1);
    }
    
    Piece* p1 = getBoard()->getPiece(getPos() + Coord(dir, 1));
    Piece* p2 = getBoard()->getPiece(getPos() + Coord(dir, -1));
    
    if ((p1 != nullptr && p1->type() == 'K' && p1->getColor() != getColor()) || (p2 != nullptr && p2->type() == 'K' && p2->getColor() != getColor()))
    {
        return true;
    }
    
    //else
    return false;
}

double Pawn::worth() const
{
    return 1.0;
}

char Pawn::type() const
{
    return 'P';
}

void Pawn::updateLegalMoves()
{
    legalMoves.clear();
    /*
     gives "directionality" to a pawn - -1 for white (going up the board), +1 for black (going down the board)
     */
    int dir;
    
    if (getColor() == 'W')
    {
        dir = -1;
    } 
    else if (getColor() == 'B')
    {
        dir = 1;
    }
    else
    {
        cerr << "Invalid color specifier" << endl;
        exit(1);
    }
    
    //Regular forward case
    if (legalMove(getPos() + Coord(dir, 0)))
    {
        //Pawn can only move if there is no piece there
        if (getBoard()->getPiece(getPos() + Coord(dir, 0)) == nullptr)
        {
            legalMoves.push_back(getPos() + Coord(dir, 0));
        }
    }
    /*
     First move case: May move forward two spaces
     */
    if (!hasMoved() && legalMove(getPos() + Coord(2*dir, 0)))
    {
        //Pawn can only move forward two if not obstructed in any way
        if (getBoard()->getPiece(getPos() + Coord(dir, 0)) == nullptr && getBoard()->getPiece(getPos() + Coord(2*dir, 0)) == nullptr)
        {
            legalMoves.push_back(getPos() + Coord(2*dir, 0));
        }
    }
    
    /*
     Capture case: if the diagonals are a legal move AND there is a piece there AND the color tag is of opposite color, then it can be inserted into the legal move set
     */
    
    if (legalMove(getPos() + Coord(dir, 1)) && getBoard()->getPiece(getPos() + Coord(dir, 1)) != nullptr && getBoard()->getPiece(getPos() + Coord(dir, 1))->getColor() != getColor())
    {
        legalMoves.push_back(getPos() + Coord(dir, 1));
    }
    if (legalMove(getPos() + Coord(dir, -1)) && getBoard()->getPiece(getPos() + Coord(dir, -1)) != nullptr && getBoard()->getPiece(getPos() + Coord(dir, -1))->getColor() != getColor())
    {
        legalMoves.push_back(getPos() + Coord(dir, -1));
    }
}

Set Pawn::pseudoLegalMoves() const
{
    /*
     gives "directionality" to a pawn - -1 for white (going up the board), +1 for black (going down the board)
     */
    int dir;
    
    if (getColor() == 'W')
    {
        dir = -1;
    }
    else if (getColor() == 'B')
    {
        dir = 1;
    }
    else
    {
        cerr << "Invalid color specifier in Pawn::pseudoLegalMoves()" << endl;
        exit(1);
    }
    
    Set set;
    
    /*
     Since pseudolegal moves are used to determine kings possibly walking into check, we only care about its capturing squares
     */
    
    set.insert(getPos() + Coord(dir, 1));
    set.insert(getPos() + Coord(dir, -1));
    
    return set;
}

double Pawn::centerControl() const
{
    if (getColor() == 'W')
    {
        /*
         Giving a bonus if the pawns are in the central squares
         Giving a smaller bonus if the pawns are in squares that control the center
         */
        if (getPos() == Coord(4,3) || getPos() == Coord(4, 4))
        {
            return .4;
        }
        else if (getPos() == Coord(4, 2) || getPos() == Coord(5, 2) || getPos() == Coord(5, 3) || getPos() == Coord(5, 4) || getPos() == Coord(5, 5) || getPos() == Coord(4, 5))
        {
            return 0.12;
        }
    }
    else
    {
        if (getPos() == Coord(3,3) || getPos() == Coord(3, 4))
        {
            return .4;
        }
        else if (getPos() == Coord(3, 2) || getPos() == Coord(2, 2) || getPos() == Coord(2, 3) || getPos() == Coord(2, 4) || getPos() == Coord(2, 5) || getPos() == Coord(3, 5))
        {
            return 0.12;
        }
    }
    return 0.0;
}

void Pawn::update()
{
    Piece* p;
    
    if (getColor() == 'W')
    {
        //if not pinned or pinned UL
        p = getBoard()->getPiece(getPos() + Coord(-1, -1));
        if (p != nullptr)
        {
            if (p->getColor() != getColor())
            {
                //assuming it is pinned in these direction, OR threatening the enemy king
                if (getPinDir() == 'N' || getPinDir() == 'D' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            else
            {
                //does not apply for the defending king case
                if (getPinDir() == 'N' || getPinDir() == 'D')
                {
                    p->defenders.push_back(this);
                }
            }
        }
        //if not pinned or pinned UR
        p = getBoard()->getPiece(getPos() + Coord(-1, 1));
        if (p != nullptr)
        {
            if (p->getColor() != getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'd' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'd')
                {
                    p->defenders.push_back(this);
                }
            }
        }
    }
    else
    {
        //if not pinned or pinned DL
        p = getBoard()->getPiece(getPos() + Coord(1, -1));
        if (p != nullptr)
        {
            if (p->getColor() != getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'd' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'd')
                {
                    p->defenders.push_back(this);
                }
            }
        }
        //if not pinned or pinned DR
        p = getBoard()->getPiece(getPos() + Coord(1, 1));
        if (p != nullptr)
        {
            if (p->getColor() != getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'D' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'D')
                {
                    p->defenders.push_back(this);
                }
            }
        }
    }
}

