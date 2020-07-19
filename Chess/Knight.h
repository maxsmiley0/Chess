#ifndef Knight_h
#define Knight_h

#include "Piece.h"

class Board;

class Knight : public Piece
{
public:
    Knight(Board* b, Coord c, char col);
    virtual ~Knight();
    
    virtual bool canReachEnemyKing() const;
    virtual double worth() const;
    virtual char type() const;
    virtual void updateLegalMoves();
    virtual Set pseudoLegalMoves() const;
    virtual double centerControl();
    
    virtual void update();
}; 

#endif /* Knight_h */
