//
//  Queen.cpp
//  Chess
//
//  Created by Max Smiley on 6/29/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include <iostream>

#include "Queen.h"
#include "Set.h"
#include "Board.h"

using namespace std;

Queen::Queen(Board* b, Coord c, char col) : Piece(b, c, col)
{
    
}

Queen::~Queen()
{
    
}

bool Queen::canReachEnemyKing() const
{
    Coord c = getPos();
    Coord e = getBoard()->getKing(getOppositeColor())->getPos();
    
    if (c.getX() == e.getX())
    {
        //case horizontal
        if (c.getY() < e.getY())
        {
            int i = 1;
            while ((c + Coord(0, i)).getY() < e.getY())
            {
                if (getBoard()->getPiece(c + Coord(0, i)) != nullptr)
                {
                    return false;
                }
                i++;
            }
            return true;
        }
        else
        {
            int i = -1;
            while ((c + Coord(0, i)).getY() > e.getY())
            {
                if (getBoard()->getPiece(c + Coord(0, i)) != nullptr)
                {
                    return false;
                }
                i--;
            }
            return true;
        }
    }
    else if (c.getY() == e.getY())
    {
        //case vertical
        if (c.getX() < e.getX())
        {
            int i = 1;
            while ((c + Coord(i, 0)).getX() < e.getX())
            {
                if (getBoard()->getPiece(c + Coord(i, 0)) != nullptr)
                {
                    return false;
                }
                i++;
            }
            return true;
        }
        else
        {
            int i = -1;
            while ((c + Coord(i, 0)).getX() > e.getX())
            {
                if (getBoard()->getPiece(c + Coord(i, 0)) != nullptr)
                {
                    return false;
                }
                i--;
            }
            return true;
        }
    }
    
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

double Queen::worth() const
{
    return 9.0;
}

char Queen::type() const
{
    return 'Q';
}

void Queen::updateLegalMoves()
{
    legalMoves.clear();
    
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(0, i)))
        {
            legalMoves.push_back(getPos() + Coord(0, i));
        }
        if (getBoard()->getPiece(getPos() + Coord(0, i)) != nullptr)
        {
            break;
        }
    }
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(0, -i)))
        {
            legalMoves.push_back(getPos() + Coord(0, -i));
        }
        if (getBoard()->getPiece(getPos() + Coord(0, -i)) != nullptr)
        {
            break;
        }
    }
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(i, 0)))
        {
            legalMoves.push_back(getPos() + Coord(i, 0));
        }
        if (getBoard()->getPiece(getPos() + Coord(i, 0)) != nullptr)
        {
            break;
        }
    }
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(-i, 0)))
        {
            legalMoves.push_back(getPos() + Coord(-i, 0));
        }
        if (getBoard()->getPiece(getPos() + Coord(-i, 0)) != nullptr)
        {
            break;
        }
    }
    
    //bishop-related functionality
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

Set Queen::pseudoLegalMoves() const
{
    Set set;
    
    //"rook-like" portion of moves
    //set these to true when encountering an ilpseudoLegal move so queen can't move past
    bool rCheck = false;
    bool uCheck = false;
    bool lCheck = false;
    bool dCheck = false;
    
    for (int i = 1; i < 8; i++)
    {
        if (pseudoLegalMove(getPos() + Coord(i, 0)) && !dCheck)
        {
            set.insert(getPos() + Coord(i, 0));
            if (getBoard()->getPiece(getPos() + Coord(i, 0)) != nullptr && getBoard()->getPiece(getPos() + Coord(i, 0)) != getBoard()->getKing(getOppositeColor()))
            {
                dCheck = true;
            }
        }
        else
        {
            dCheck = true;
        }
        if (pseudoLegalMove(getPos() + Coord(-i, 0)) && !uCheck)
        {
            set.insert(getPos() + Coord(-i, 0));
            if (getBoard()->getPiece(getPos() + Coord(-i, 0)) != nullptr && getBoard()->getPiece(getPos() + Coord(-i, 0)) != getBoard()->getKing(getOppositeColor()))
            {
                uCheck = true;
            }
        }
        else
        {
            uCheck = true;
        }
        if (pseudoLegalMove(getPos() + Coord(0, i)) && !rCheck)
        {
            set.insert(getPos() + Coord(0, i));
            if (getBoard()->getPiece(getPos() + Coord(0, i)) != nullptr && getBoard()->getPiece(getPos() + Coord(0, i)) != getBoard()->getKing(getOppositeColor()))
            {
                rCheck = true;
            }
        }
        else
        {
            rCheck = true;
        }
        if (pseudoLegalMove(getPos() + Coord(0, -i)) && !lCheck)
        {
            set.insert(getPos() + Coord(0, -i));
            if (getBoard()->getPiece(getPos() + Coord(0, -i)) != nullptr && getBoard()->getPiece(getPos() + Coord(0, -i)) != getBoard()->getKing(getOppositeColor()))
            {
                lCheck = true;
            }
        }
        else
        {
            lCheck = true;
        }
    }
    
    //bishop-related functionality
    //set these to true when encountering an ilpseudoLegal move so queen can't move past
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

void Queen::update()
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
    
    //4 cases: UP, DOWN, LEFT, RIGHT
    /*
     UP / DOWN: either N or V pinned
     LEFT / RIGHT: either N or H pinned
     search until we go out of bounds OR find a piece
     if found piece then update attack/defenders
     */
    int ii = 1;
    while (true)
    {
        if ((getPos() + Coord(ii, 0)).getX() > 7)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(ii, 0));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'V')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'V' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        ii++;
    }
    
    int jj = 1;
    while (true)
    {
        if ((getPos() + Coord(-jj, 0)).getX() < 0)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(-jj, 0));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'V')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'V' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        jj++;
    }
    
    int kk = 1;
    while (true)
    {
        if ((getPos() + Coord(0, kk)).getY() > 7)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(0, kk));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'H')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'H' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        kk++;
    }
    
    int ll = 1;
    while (true)
    {
        if ((getPos() + Coord(0, -ll)).getY() < 0)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(0, -ll));
        if (p != nullptr)
        {
            if (getColor() == p->getColor())
            {
                if (getPinDir() == 'N' || getPinDir() == 'H')
                {
                    p->defenders.push_back(this);
                }
            }
            else
            {
                if (getPinDir() == 'N' || getPinDir() == 'H' || p->type() == 'K')
                {
                    p->attackers.push_back(this);
                }
            }
            break;
        }
        ll++;
    }
}
