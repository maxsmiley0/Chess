#ifndef Rook_h
#define Rook_h

#include "Piece.h"

class Board;

class Rook : public Piece
{
public:
    Rook(Board* b, Coord c, char col);
    virtual ~Rook();
    
    virtual bool canReachEnemyKing() const;
    virtual double worth() const;
    virtual char type() const;
    virtual void updateLegalMoves();
    virtual Set pseudoLegalMoves() const;
    virtual void update();
};

#endif /* Rook_h */
