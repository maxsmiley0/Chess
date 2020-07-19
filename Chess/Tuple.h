//
//  Tuple.h
//  Chess
//
//  Created by Max Smiley on 7/11/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef Tuple_h
#define Tuple_h

#include "Piece.h"
#include "Board.h"

struct TuplePC
{
    Piece* p = nullptr;
    Coord c;
    double eval;
};

struct TupleBE
{
    Board* b;
    double eval;
};

#endif /* Tuple_h */
