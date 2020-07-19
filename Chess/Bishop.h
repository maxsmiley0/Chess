//
//  Bishop.hpp
//  Chess
//
//  Created by Max Smiley on 6/27/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef Bishop_h
#define Bishop_h

#include "Piece.h"

class Board;

class Bishop : public Piece
{
public:
    Bishop(Board* b, Coord c, char col);
    virtual ~Bishop();
    
    virtual bool canReachEnemyKing() const;
    virtual double worth() const;
    virtual char type() const;
    virtual void updateLegalMoves();
    virtual Set pseudoLegalMoves() const;
    virtual double centerControl() const;
     
    virtual void update();
};

#endif /* Bishop_h */
