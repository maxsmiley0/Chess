//
//  Pawn.hpp
//  Chess
//
//  Created by Max Smiley on 6/29/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef Pawn_h
#define Pawn_h

#include "Piece.h"

class Board;

class Pawn : public Piece
{
public:
    Pawn(Board* b, Coord c, char col);
    virtual ~Pawn();
    
    virtual bool canReachEnemyKing() const;
    virtual double worth() const;
    virtual char type() const;
    virtual void updateLegalMoves();
    virtual Set pseudoLegalMoves() const;
    virtual double centerControl() const;
    
    virtual void update();
}; 

#endif /* Pawn_h */
