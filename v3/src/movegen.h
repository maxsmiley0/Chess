#pragma once

#include "position.h"
#include "types.h"

struct MoveList {
    Move moves[MAX_MOVES];
    int  size = 0;

    void add(Move m) { moves[size++] = m; }

    Move*       begin()       { return moves; }
    Move*       end()         { return moves + size; }
    const Move* begin() const { return moves; }
    const Move* end()   const { return moves + size; }
};

void generate_moves(const Position& pos, MoveList& ml);
void generate_captures(const Position& pos, MoveList& ml);

MoveList generate_moves(const Position& pos);
MoveList generate_captures(const Position& pos);
