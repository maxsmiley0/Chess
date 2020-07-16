//
//  King.hpp
//  Chess
//
//  Created by Max Smiley on 5/10/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef King_h
#define King_h

#include "Piece.h"

class Board;

class King : public Piece
{
public:
    King(Board* b, Coord c, char col);
    virtual ~King();
    
    virtual bool canReachEnemyKing() const;
    virtual double worth() const;
    virtual char type() const;
    virtual Set legalMoves() const;
    virtual Set pseudoLegalMoves() const;
    virtual void update();
    
private:
    //Auxillary function to determine if king can castle or not
    //Input -1 for left, 1 for right, and char color
    bool canCastle(char color, int side) const;
};

#endif /* King_hpp */
