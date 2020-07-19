#include <iostream>

#include "Auxiliary.h"
#include "King.h"
#include "Board.h"
#include "Set.h"

using namespace std;

King::King(Board* b, Coord c, char col) : Piece(b, c, col)
{
    
}

King::~King()
{
    
}

bool King::canReachEnemyKing() const
{
    /*
     By rules of chess, King will never be next to enemy King at the start of a move
     */
    
    return false;
}

double King::worth() const
{
    return 0;
}

char King::type() const
{
    return 'K';
}

void King::updateLegalMoves()
{
    legalMoves.clear();
    //Regular Moves
    
    if ((legalMove(getPos() + Coord(1, 1)) && getBoard()->canReachCoordPseudo(getPos() + Coord(1, 1), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(1, 1));
    }
    if ((legalMove(getPos() + Coord(0, 1)) && getBoard()->canReachCoordPseudo(getPos() + Coord(0, 1), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(0, 1));
    }
    if ((legalMove(getPos() + Coord(-1, 1)) && getBoard()->canReachCoordPseudo(getPos() + Coord(-1, 1), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(-1, 1));
    }
    if ((legalMove(getPos() + Coord(-1, 0)) && getBoard()->canReachCoordPseudo(getPos() + Coord(-1, 0), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(-1, 0));
    }
    if ((legalMove(getPos() + Coord(-1, -1)) && getBoard()->canReachCoordPseudo(getPos() + Coord(-1, -1), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(-1, -1));
    }
    if ((legalMove(getPos() + Coord(0, -1)) && getBoard()->canReachCoordPseudo(getPos() + Coord(0, -1), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(0, -1));
    }
    if ((legalMove(getPos() + Coord(1, -1)) && getBoard()->canReachCoordPseudo(getPos() + Coord(1, -1), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(1, -1));
    }
    if ((legalMove(getPos() + Coord(1, 0)) && getBoard()->canReachCoordPseudo(getPos() + Coord(1, 0), getOppositeColor()).empty()))
    {
        legalMoves.push_back(getPos() + Coord(1, 0));
    }
    //Castling
    
    if (canCastle(getColor(), -1))
    {
        if (getColor() == 'W')
        {
            legalMoves.push_back(Coord(7, 2));
        }
        else if (getColor() == 'B')
        {
            legalMoves.push_back(Coord(0, 6));
        }
    }
    
    if (canCastle(getColor(), 1))
    {
        if (getColor() == 'W')
        {
            legalMoves.push_back(Coord(7, 6));
        }
        else if (getColor() == 'B')
        {
            legalMoves.push_back(Coord(0, 2));
        }
    }
}

bool King::canCastle(char color, int side) const
{
    if (hasMoved())
    {
        return false;
    }
    
    if (color == 'W')
    {
        if (side == 1)
        {
            /*
             If neither the king nor the rook have moved, and there are neither any friendy pieces obstructing the castling path nor enemy pieces threatening these squares, AND the king is not already in check, the king may castle.
             */
            if (getBoard()->getPiece(Coord(7, 7)) != nullptr &&!getBoard()->getPiece(Coord(7, 7))->hasMoved() && getBoard()->getPiece(getPos() + Coord(0, 1)) == nullptr && getBoard()->getPiece(getPos() + Coord(0, 2)) == nullptr && !inCheck() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, 1), 'B').empty() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, 2), 'B').empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (getBoard()->getPiece(Coord(7, 0)) != nullptr &&!getBoard()->getPiece(Coord(7, 0))->hasMoved() && getBoard()->getPiece(getPos() + Coord(0, -1)) == nullptr && getBoard()->getPiece(getPos() + Coord(0, -2)) == nullptr &&
                getBoard()->getPiece(getPos() + Coord(0, -3)) == nullptr &&
                !inCheck() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, -1), 'B').empty() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, -2), 'B').empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else if (color == 'B')
    {
        if (side == 1)
        {
            if (getBoard()->getPiece(Coord(0, 0)) != nullptr &&!getBoard()->getPiece(Coord(0, 0))->hasMoved() && getBoard()->getPiece(getPos() + Coord(0, -1)) == nullptr && getBoard()->getPiece(getPos() + Coord(0, -2)) == nullptr &&
                getBoard()->getPiece(getPos() + Coord(0, -3)) == nullptr &&
                !inCheck() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, -1), 'W').empty() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, -2), 'W').empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            if (getBoard()->getPiece(Coord(0, 7)) != nullptr && !getBoard()->getPiece(Coord(0, 7))->hasMoved() && getBoard()->getPiece(getPos() + Coord(0, 1)) == nullptr && getBoard()->getPiece(getPos() + Coord(0, 2)) == nullptr &&
                !inCheck() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, 1), 'W').empty() && getBoard()->canReachCoordPseudo(getPos() + Coord(0, 2), 'W').empty())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    else
    {
        cerr << "Invalid color specifier in King::canCastle" << endl;
        exit(1);
    }
}

Set King::pseudoLegalMoves() const
{
    Set set;
    
    //Regular Moves
    
    if (pseudoLegalMove(getPos() + Coord(1, 1)))
    {
        set.insert(getPos() + Coord(1, 1));
    }
    if (pseudoLegalMove(getPos() + Coord(0, 1)))
    {
        set.insert(getPos() + Coord(0, 1));
    }
    if (pseudoLegalMove(getPos() + Coord(-1, 1)))
    {
        set.insert(getPos() + Coord(-1, 1));
    }
    if (pseudoLegalMove(getPos() + Coord(-1, 0)))
    {
        set.insert(getPos() + Coord(-1, 0));
    }
    if (pseudoLegalMove(getPos() + Coord(-1, -1)))
    {
        set.insert(getPos() + Coord(-1, -1));
    }
    if (pseudoLegalMove(getPos() + Coord(0, -1)))
    {
        set.insert(getPos() + Coord(0, -1));
    }
    if (pseudoLegalMove(getPos() + Coord(1, -1)))
    {
        set.insert(getPos() + Coord(1, -1));
    }
    if (pseudoLegalMove(getPos() + Coord(1, 0)))
    {
        set.insert(getPos() + Coord(1, 0));
    }
    
    return set;
}

void King::update()
{
    setNumDefending(0);
    setNumAttacking(0);
    
    Piece* p;
    
    p = getBoard()->getPiece(getPos() + Coord(1, 0));
    if (pseudoLegalMove((getPos() + Coord(1, 0))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(1, 0)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(-1, 0));
    if (pseudoLegalMove((getPos() + Coord(-1, 0))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(-1, 0)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(0, 1));
    if (pseudoLegalMove((getPos() + Coord(0, 1))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(0, 1)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(0, -1));
    if (pseudoLegalMove((getPos() + Coord(0, -1))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(0, -1)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(1, 1));
    if (pseudoLegalMove((getPos() + Coord(1, 1))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(1, 1)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(-1, 1));
    if (pseudoLegalMove((getPos() + Coord(-1, 1))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(-1, 1)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(1, -1));
    if (pseudoLegalMove((getPos() + Coord(1, -1))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(1, -1)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
    
    p = getBoard()->getPiece(getPos() + Coord(-1, -1));
    if (pseudoLegalMove((getPos() + Coord(-1, -1))) && p != nullptr)
    {
        if (p->getColor() == getColor())
        {
            p->defenders.push_back(this);
            incrementNumDefending();
        }
        else
        {
            if (containsCoord(legalMoves, getPos() + Coord(-1, -1)))
            {
                p->attackers.push_back(this);
                incrementNumAttacking();
            }
        }
    }
}
