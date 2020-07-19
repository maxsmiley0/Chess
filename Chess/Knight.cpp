#include <iostream>

#include <cmath>
#include "Knight.h"
#include "Set.h"
#include "Board.h"

using namespace std;

Knight::Knight(Board* b, Coord c, char col) : Piece(b, c, col)
{
    
}

Knight::~Knight()
{
    
}

bool Knight::canReachEnemyKing() const
{
    Coord e = getBoard()->getKing(getOppositeColor())->getPos();
    
    if (abs((e - getPos()).getX()) > 2)
    {
        return false;
    }
    
    if (e == getPos() + Coord(2, 1))
    {
        return true;
    }
    if (e == getPos() + Coord(-2, 1))
    {
        return true;
    }
    if (e == getPos() + Coord(2, -1))
    {
        return true;
    }
    if (e == getPos() + Coord(-2, -1))
    {
        return true;
    }
    if (e == getPos() + Coord(1, 2))
    {
        return true;
    }
    if (e == getPos() + Coord(-1, 2))
    {
        return true;
    }
    if (e == getPos() + Coord(1, -2))
    {
        return true;
    }
    if (e == getPos() + Coord(-1, -2))
    {
        return true;
    }
    
    return false;
}

double Knight::worth() const
{
    return 3.0;
}

char Knight::type() const
{
    return 'N';
}

void Knight::updateLegalMoves()
{
    legalMoves.clear();
    
    if (legalMove(getPos() + Coord(2, 1)))
    {
        legalMoves.push_back(getPos() + Coord(2, 1));
    }
    if (legalMove(getPos() + Coord(2, -1)))
    {
        legalMoves.push_back(getPos() + Coord(2, -1));
    }
    if (legalMove(getPos() + Coord(-2, 1)))
    {
        legalMoves.push_back(getPos() + Coord(-2, 1));
    }
    if (legalMove(getPos() + Coord(-2, -1)))
    {
        legalMoves.push_back(getPos() + Coord(-2, -1));
    }
    if (legalMove(getPos() + Coord(1, 2)))
    {
        legalMoves.push_back(getPos() + Coord(1, 2));
    }
    if (legalMove(getPos() + Coord(1, -2)))
    {
        legalMoves.push_back(getPos() + Coord(1, -2));
    }
    if (legalMove(getPos() + Coord(-1, 2)))
    {
        legalMoves.push_back(getPos() + Coord(-1, 2));
    }
    if (legalMove(getPos() + Coord(-1, -2)))
    {
        legalMoves.push_back(getPos() + Coord(-1, -2));
    }
}

Set Knight::pseudoLegalMoves() const
{
    Set set;
    
    if (pseudoLegalMove(getPos() + Coord(2, 1)))
    {
        set.insert(getPos() + Coord(2, 1));
    }
    if (pseudoLegalMove(getPos() + Coord(2, -1)))
    {
        set.insert(getPos() + Coord(2, -1));
    }
    if (pseudoLegalMove(getPos() + Coord(-2, 1)))
    {
        set.insert(getPos() + Coord(-2, 1));
    }
    if (pseudoLegalMove(getPos() + Coord(-2, -1)))
    {
        set.insert(getPos() + Coord(-2, -1));
    }
    if (pseudoLegalMove(getPos() + Coord(1, 2)))
    {
        set.insert(getPos() + Coord(1, 2));
    }
    if (pseudoLegalMove(getPos() + Coord(1, -2)))
    {
        set.insert(getPos() + Coord(1, -2));
    }
    if (pseudoLegalMove(getPos() + Coord(-1, 2)))
    {
        set.insert(getPos() + Coord(-1, 2));
    }
    if (pseudoLegalMove(getPos() + Coord(-1, -2)))
    {
        set.insert(getPos() + Coord(-1, -2));
    }
    
    return set;
}

double Knight::centerControl()
{
    /*
     Penalty if knight is on the rim...
     */
    
    if (getColor() == 'W')
    {
        if (getPos() == Coord(5, 2) || getPos() == Coord(5, 6))
        {
            return .23;
        }
        else if (getPos() == Coord(6, 3) || getPos() == Coord(6, 4))
        {
            return .14;
        }
        else if (getPos() == Coord(5, 0) || getPos() == Coord(5, 7))
        {
            return -.23;
        }
    }
    else
    {
        if (getPos() == Coord(2, 2) || getPos() == Coord(2, 6))
        {
            return .23;
        }
        else if (getPos() == Coord(1, 3) || getPos() == Coord(1, 4))
        {
            return .14;
        }
        else if (getPos() == Coord(2, 0) || getPos() == Coord(2, 7))
        {
            return -.23;
        }
    }

    return 0.0;
}

void Knight::update()
{
    Piece* p;
    
    p = getBoard()->getPiece(getPos() + Coord(2, 1));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(-2, 1));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(2, -1));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(-2, -1));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(1, 2));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(1, -2));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(-1, 2));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
    p = getBoard()->getPiece(getPos() + Coord(-1, -2));
    if (p != nullptr)
    {
        if (getColor() == p->getColor())
        {
            if (getPinDir() == 'N')
            {
                p->defenders.push_back(this);
            }
        }
        else
        {
            if (getPinDir() == 'N' || p->type() == 'K')
            {
                p->attackers.push_back(this);
            }
        }
    }
}
