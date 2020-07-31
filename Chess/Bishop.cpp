//
//  Bishop.cpp
//  Chess
//
//  Created by Max Smiley on 6/27/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include <iostream>

#include "Auxiliary.h"
#include "Bishop.h"
#include "Set.h"
#include "Board.h"

using namespace std;

Bishop::Bishop(Board* b, Coord c, char col) : Piece(b, c, col)
{
    
}

Bishop::~Bishop()
{
    
}

bool Bishop::canReachEnemyKing() const
{
    Coord c = getPos();
    Coord e = getBoard()->getKing(getOppositeColor())->getPos();
    Coord differential(e - c);
    //case y = -x diagonal
    if (differential.getX() == differential.getY())
    {
        //case DR vector
        if (c.getX() < e.getX())
        {
            int i = 1;
            while ((c + Coord(i, i)).getX() < e.getX())
            {
                if (getBoard()->getPiece(c + Coord(i, i)) != nullptr)
                {
                    return false;
                }
                i++;
            }
            return true;
        }
        //case UL vector
        else
        {
            int i = -1;
            while ((c + Coord(i, i)).getX() > e.getX())
            {
                if (getBoard()->getPiece(c + Coord(i, i)) != nullptr)
                {
                    return false;
                }
                i--;
            }
            return true;
        }
    }
    //case y = x diagonal
    else if (differential.getX() == -1*differential.getY())
    {
        //case UR vector
        if (c.getX() > e.getX())
        {
            int i = 1;
            while ((c + Coord(-i, i)).getX() > e.getX())
            {
                if (getBoard()->getPiece(c + Coord(-i, i)) != nullptr)
                {
                    return false;
                }
                i++;
            }
            return true;
        }
        //case DL vector
        else
        {
            int i = 1;
            while ((c + Coord(i, -i)).getX() < e.getX())
            {
                if (getBoard()->getPiece(c + Coord(i, -i)) != nullptr)
                {
                    return false;
                }
                i++;
            }
            return true;
        }
    }
    return false;
}

double Bishop::worth() const
{
    return 3.0;
}

char Bishop::type() const
{
    return 'B';
}

void Bishop::updateLegalMoves()
{
    legalMoves.clear();
    
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(i, i)))
        {
            legalMoves.push_back(getPos() + Coord(i, i));
        }
        if (getBoard()->getPiece(getPos() + Coord(i, i)) != nullptr)
        {
            break;
        }
    }
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(-i, i)))
        {
            legalMoves.push_back(getPos() + Coord(-i, i));
        }
        if (getBoard()->getPiece(getPos() + Coord(-i, i)) != nullptr)
        {
            break;
        }
    }
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(i, -i)))
        {
            legalMoves.push_back(getPos() + Coord(i, -i));
        }
        if (getBoard()->getPiece(getPos() + Coord(i, -i)) != nullptr)
        {
            break;
        }
    }
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(-i, -i)))
        {
            legalMoves.push_back(getPos() + Coord(-i, -i));
        }
        if (getBoard()->getPiece(getPos() + Coord(-i, -i)) != nullptr)
        {
            break;
        }
    }
}

Set Bishop::pseudoLegalMoves() const
{
    Set set;
    
    //set these to true when encountering an illegal pseudoLegal move so bishop can't move past
    bool urCheck = false;
    bool ulCheck = false;
    bool drCheck = false;
    bool dlCheck = false;
    
    
    //Set lMoves = getRay(this, dir);
    //return getRay(this, dir);
    
    
    for (int i = 1; i < 8; i++)
    {
        if (pseudoLegalMove(getPos() + Coord(i, i)) && !drCheck)
        {
            set.insert(getPos() + Coord(i, i));
            if (getBoard()->getPiece(getPos() + Coord(i, i)) != nullptr && getBoard()->getPiece(getPos() + Coord(i, i)) != getBoard()->getKing(getOppositeColor()))
            {
                drCheck = true;
            }
        }
        else
        {
            drCheck = true;
        }
        if (pseudoLegalMove(getPos() + Coord(-i, i)) && !urCheck)
        {
            set.insert(getPos() + Coord(-i, i));
            if (getBoard()->getPiece(getPos() + Coord(-i, i)) != nullptr && getBoard()->getPiece(getPos() + Coord(-i, i)) != getBoard()->getKing(getOppositeColor()))
            {
                urCheck = true;
            }
        }
        else
        {
            urCheck = true;
        }
        if (pseudoLegalMove(getPos() + Coord(i, -i)) && !dlCheck)
        {
            set.insert(getPos() + Coord(i, -i));
            if (getBoard()->getPiece(getPos() + Coord(i, -i)) != nullptr && getBoard()->getPiece(getPos() + Coord(i, -i)) != getBoard()->getKing(getOppositeColor()))
            {
                dlCheck = true;
            }
        }
        else
        {
            dlCheck = true;
        }
        if (pseudoLegalMove(getPos() + Coord(-i, -i)) && !ulCheck)
        {
            set.insert(getPos() + Coord(-i, -i));
            if (getBoard()->getPiece(getPos() + Coord(-i, -i)) != nullptr && getBoard()->getPiece(getPos() + Coord(-i, -i)) != getBoard()->getKing(getOppositeColor()))
            {
                ulCheck = true;
            }
        }
        else
        {
            ulCheck = true;
        }
    }
    return set;
}

double Bishop::centerControl() const
{
    /*
     For white, c4, b3, f4, g3 are all +.22
     Most other positions + fianchetto are worth .17
     
     Mirrored for black
     If in none of these positions return 0
     */
     
    if (getColor() == 'W')
    {
        if (getPos() == Coord(4, 5) || getPos() == Coord(4, 2) || getPos() == Coord(5, 6) || getPos() == Coord(5, 1))
        {
            return .22;
        }
        else if (getPos() == Coord(4, 0) || getPos() == Coord(3, 1) || getPos() == Coord(3, 6) || getPos() == Coord(4, 7) || getPos() == Coord(6, 6) || getPos() == Coord(6, 1) || getPos() == Coord(6, 3) || getPos() == Coord(6, 4))
        {
            return .17;
        }
    }
    else
    {
        if (getPos() == Coord(3, 5) || getPos() == Coord(3, 2) || getPos() == Coord(2, 6) || getPos() == Coord(2, 1))
        {
            return .22;
        }
        else if (getPos() == Coord(3, 0) || getPos() == Coord(4, 1) || getPos() == Coord(4, 6) || getPos() == Coord(3, 7) || getPos() == Coord(1, 6) || getPos() == Coord(1, 1) || getPos() == Coord(1, 3) || getPos() == Coord(1, 4))
        {
            return .17;
        }
    }
    
    return 0.0;
}

void Bishop::update()
{
    //4 cases: UL, UR, DL, DR
    /*
     UL / DR: either N or D pinned
     UR / DL: either N or d pinned
     search until we go out of bounds OR find a piece
     if found piece then update attack/defenders
     */
    
    //UL case -> -x, -y
    Piece* p;
    
    int i = 1;
    while (true)
    {
        if ((getPos() + Coord(-i, -i)).getX() < 0 || (getPos() + Coord(-i, -i)).getY() < 0)
        {
            break;
        }
        p = getBoard()->getPiece(getPos() + Coord(-i, -i));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'D')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'D' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        i++;
    }
    
    
    //UR case -> -x, +y
    int j = 1;
    while (true)
    {
        if ((getPos() + Coord(-j, j)).getX() < 0 || (getPos() + Coord(-j, j)).getY() > 7)
        {
            break;
        }
        p = getBoard()->getPiece(getPos() + Coord(-j, j));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'd')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'd' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        j++;
    }
    
    //DL case -> x, -y
    int k = 1;
    while (true)
    {
        if ((getPos() + Coord(k, -k)).getX() > 7 || (getPos() + Coord(k, -k)).getY() < 0)
        {
            break;
        }
        p = getBoard()->getPiece(getPos() + Coord(k, -k));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'd')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'd' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        k++;
    }
    
    //case DR -> x, y
    int l = 1;
    while (true)
    {
        if ((getPos() + Coord(l, l)).getX() > 7 || (getPos() + Coord(l, l)).getY() > 7)
        {
            break;
        }
        p = getBoard()->getPiece(getPos() + Coord(l, l));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'D')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'D' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        l++;
    }
}
