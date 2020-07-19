//
//  Queen.hpp
//  Chess
//
//  Created by Max Smiley on 6/29/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef Queen_h
#define Queen_h

#include "Piece.h"

class Board;

class Queen : public Piece
{
public:
    Queen(Board* b, Coord c, char col);
    virtual ~Queen();
    
    virtual bool canReachEnemyKing() const;
    virtual double worth() const;
    virtual char type() const;
    virtual void updateLegalMoves();
    virtual Set pseudoLegalMoves() const;
    virtual void update();
};

#endif /* Queen_h*/
