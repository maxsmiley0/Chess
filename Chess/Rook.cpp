#include <iostream>

#include "Rook.h"
#include "Set.h"
#include "Board.h"

using namespace std;

Rook::Rook(Board* b, Coord c, char col) : Piece(b, c, col)
{
    
}

Rook::~Rook()
{
    
}

bool Rook::canReachEnemyKing() const
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
    
    return false;
}

double Rook::worth() const
{
    return 5.0;
}

char Rook::type() const
{
    return 'R';
}

void Rook::updateLegalMoves()
{
    legalMoves.clear();
    
    //set these to true when encountering an illegal move so rook can't move past
    bool rCheck = false;
    bool uCheck = false;
    bool lCheck = false;
    bool dCheck = false;
    
    //responsible for adding these moves to legalMoves
    
    for (int i = 1; i < 8; i++)
    {
        if (legalMove(getPos() + Coord(i, 0)) && !dCheck)
        {
            legalMoves.push_back(getPos() + Coord(i, 0));
        }
        if (legalMove(getPos() + Coord(-i, 0)) && !uCheck)
        {
            legalMoves.push_back(getPos() + Coord(-i, 0));
        }
        if (legalMove(getPos() + Coord(0, i)) && !rCheck)
        {
            legalMoves.push_back(getPos() + Coord(0, i));
        }
        if (legalMove(getPos() + Coord(0, -i)) && !lCheck)
        {
            legalMoves.push_back(getPos() + Coord(0, -i));
        }
        
        //If we hit a piece, stop checking
        
        if (getBoard()->getPiece(getPos() + Coord(i, 0)) != nullptr)
        {
            dCheck = true;
        }
        if (getBoard()->getPiece(getPos() + Coord(-i, 0)) != nullptr)
        {
            uCheck = true;
        }
        if (getBoard()->getPiece(getPos() + Coord(0, i)) != nullptr)
        {
            rCheck = true;
        }
        if (getBoard()->getPiece(getPos() + Coord(0, -i)) != nullptr)
        {
            lCheck = true;
        }
    }
}

Set Rook::pseudoLegalMoves() const
{
    Set set;
    
    //set these to true when encountering a pseudoLegal move so rook can't move past
    //designing this so it pierces through the enemy king so king can't move in that direction and survive (he could if it otherwise stopped there)
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
    
    return set;
}

void Rook::update()
{
    //4 cases: UP, DOWN, LEFT, RIGHT
    /*
     UP / DOWN: either N or V pinned
     LEFT / RIGHT: either N or H pinned
     search until we go out of bounds OR find a piece
     if found piece then update attack/defenders
     */
    int i = 1;
    while (true)
    {
        if ((getPos() + Coord(i, 0)).getX() > 7)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(i, 0));
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
        i++;
    }
    
    int j = 1;
    while (true)
    {
        if ((getPos() + Coord(-j, 0)).getX() < 0)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(-j, 0));
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
        j++;
    }
    
    int k = 1;
    while (true)
    {
        if ((getPos() + Coord(0, k)).getY() > 7)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(0, k));
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
        k++;
    }
    
    int l = 1;
    while (true)
    {
        if ((getPos() + Coord(0, -l)).getY() < 0)
        {
            break;
        }
        Piece* p = getBoard()->getPiece(getPos() + Coord(0, -l));
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
        l++;
    }
}
