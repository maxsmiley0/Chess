#pragma once

#include "movegen.h"
#include "position.h"
#include "types.h"

struct SearchLimits {
    int  depth    = MAX_PLY;
    int  movetime = 0;
    int  wtime    = 0;
    int  btime    = 0;
    int  winc     = 0;
    int  binc     = 0;
    bool infinite = false;
};

struct Searcher {
    void search(Position& pos, const SearchLimits& limits);
    void stop() { stopped = true; }
    bool is_stopped() const { return stopped; }

private:
    Score negamax(Position& pos, Score alpha, Score beta, int depth, int ply);
    Score quiesce(Position& pos, Score alpha, Score beta, int ply);

    int  move_score(const Position& pos, Move m, Move first_move) const;
    void order_moves(const Position& pos, MoveList& ml, Move first_move) const;

    void init_timer(const Position& pos, const SearchLimits& limits);
    bool check_time();

    bool     stopped   = false;
    int64_t  start_ms  = 0;
    int64_t  alloc_ms  = 0;
    uint64_t nodes     = 0;

    Move     root_best_move = MOVE_NONE;
    Score    root_best_score = 0;
};
