#include "Piece.h"
#include "Board.h"
#include "Set.h"
#include "Auxiliary.h"

Piece::Piece(Board* b, Coord c, char col)
{
    mBoard = b;
    pos = c;
    color = col;
    moved = false;
    pinDir = 'N';
    numDefending = 0;
    numAttacking = 0;
    
    if (col != 'W' && col != 'B')
    {
        //guard against invalid colors
        cerr << "Invalid color assignment" << endl;
        exit(1);
    }
}

bool Piece::operator!=(const Piece& other)
{
    //When is a piece equal to another?
    //simply must have the same color and typr
    //the reason it doesn't need to have the same position is because we will be calling it in Board::operator==, which will check the squares entry by entry
    return (color != other.color || type() != other.type());
}

Piece::~Piece()
{
    //don't want to destroy mBoard! Because then there will be no shared board
}

Coord Piece::getPos() const
{
    return pos;
}

Board* Piece::getBoard() const
{
    return mBoard;
}

char Piece::getPinDir() const
{
    return pinDir;
}

char Piece::getColor() const
{
    return color;
}

char Piece::getOppositeColor() const
{
    if (color == 'W')
    {
        return 'B';
    }
    else if (color == 'B')
    {
        return 'W';
    }
    else
    {
        cerr << "Invalid color specifier in Piece::getOppositeColor()" << endl;
        exit(1);
    }
}

void Piece::setPinDir(char c)
{
    pinDir = c;
}

void Piece::setMoved(bool b)
{
    moved = b;
}

void Piece::setPos(Coord c)
{
    pos = c;
}

bool Piece::hasMoved() const
{
    return moved;
}

int Piece:: getNumDefending() const
{
    return numDefending;
}

void Piece::incrementNumDefending()
{
    numDefending++;
}

void Piece::setNumDefending(int i)
{
    numDefending = i;
}

int Piece:: getNumAttacking() const
{
    return numAttacking;
}

void Piece::incrementNumAttacking()
{
    numAttacking++;
}

void Piece::setNumAttacking(int i)
{
    numAttacking = i;
}


Piece* Piece::getWeakestDefender()
{
    //Assumes at least one defender
    Piece* p = defenders.front();
    
    for (list<Piece*>::iterator itr = defenders.begin(); itr != defenders.end(); itr++)
    {
        if ((*itr)->worth() < p->worth())
        {
            p = (*itr);
        }
    }
    return p;
}

Piece* Piece::getWeakestAttacker()
{
    //Assumes at least one attacker
    Piece* p = attackers.front();
    
    for (list<Piece*>::iterator itr = attackers.begin(); itr != attackers.end(); itr++)
    {
        if ((*itr)->worth() < p->worth())
        {
            p = (*itr);
        }
    }
    return p;
}

bool Piece::inCheck() const
{
    if (color == 'W')
    {
        for (list<Piece*>::iterator  itr = mBoard->blackPieces.begin(); itr != mBoard->blackPieces.end(); itr++)
        {
            if ((*itr)->pseudoLegalMoves().contains(getPos()))
            {
                return true;
            }
        }
    }
    else
    {
        for (list<Piece*>::iterator  itr = mBoard->whitePieces.begin(); itr != mBoard->whitePieces.end(); itr++)
        {
            if ((*itr)->pseudoLegalMoves().contains(getPos()))
            {
                return true;
            }
        }
    }
        return false;
}

bool Piece::pseudoLegalMove(Coord c) const
{
    //We may only care about pseudoLegalMoves if we're checking where the king goes
    if (c.getX() > 7 || c.getX() < 0 || c.getY() > 7 || c.getY() < 0)
    {
        return false;
    }
    return true;
}

bool Piece::legalMove(Coord c) const
{
    bool b = pseudoLegalMove(c);
    //Added check if moving in a pinned direction
    if (getPinDir() != 'N')
    {
        if (getPinDir() != defineDir(getPos(), c))
        {
            return false;
        }
    }
    /*
     So king can't capture a protected piece
     */
    if (b && type() == 'K' && mBoard->getPiece(c) != nullptr && mBoard->getPiece(c)->getColor() != color) //if same color
    {
        return true;
    }
    if (b && getBoard()->getPiece(c) != nullptr && getBoard()->getPiece(c)->getColor() == color) //if same color
    {
        return false;
    }
    
    /*
    Final Clause: King cannot be in check at the end of turn.
    We have already ruled out king moving into check and moving into discovered check
    Essentially, we now have to account for if the king is in check at the beginning of a round, he must either capture the offending piece, move the king, or interpose a piece.
    Case Not Attacked: don't need to do anything
    Case Attacked by 1 Piece: move king (so don't do anything to king's coords), interpose a piece (so rule out all legalMoves that aren't in the ray), or capture offending pice
    Case Attacked by 2+ pieces: can only escape this by moving king, so rule out any move except a king move
    */
    
    //list<Piece*> li = mBoard->canReachCoordPseudo(mBoard->getKing(getColor())->getPos(), getOppositeColor());
    
    //list<Piece*> li = mBoard->canReachKingPseudo(getColor());
    list<Piece*> li = mBoard->getKing(getColor())->attackers;
    
     if (li.size() == 1)
     {
         if (li.front()->type() == 'R' || li.front()->type() == 'B' || li.front()->type() == 'Q')
         {
            //either interpose a piece, move king, or capture piece
             char dir;
             
             if (type() == 'K' || c == li.front()->getPos() || getRay(li.front(), dir).contains(c))
             {
                 return b;
             }
             else
             {
                 return false;
             }
         }
         else
         {
             //case pawn or knight
             if (type() == 'K' || c == li.front()->getPos())
             {
                 return b;
             }
             else
             {
                 return false;
             }
         }
     }
     else if (li.size() > 1)
     {
         if (type() == 'K')
         {
             return b;
         }
         else
         {
             return false;
         }
    }
    return b;
}

double Piece::centerControl() const
{
    /*
     This will be called for Kings, Queens, and Rooks because we don't really care about their center control in the beginning
     */
    
    return 0.0;
}
