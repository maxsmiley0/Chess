#pragma once

#include "movegen.h"
#include "position.h"
#include "types.h"
#include <atomic>
#include <string>

constexpr int DEFAULT_MOVETIME_MS = 3000;

struct SearchLimits {
    int      depth     = MAX_PLY;
    int      movetime  = 0;
    int      wtime     = 0;
    int      btime     = 0;
    int      winc      = 0;
    int      binc      = 0;
    uint64_t max_nodes = 0;
    bool     infinite  = false;
};

struct Searcher {
    void search(Position& pos, const SearchLimits& limits);

    void request_stop() { stop_requested.store(true, std::memory_order_relaxed); }
    void clear_stop()   { stop_requested.store(false, std::memory_order_relaxed); }

    void set_multipv(int value) {
        multipv = value < 1 ? 1 : (value > MAX_MOVES ? MAX_MOVES : value);
    }

private:
    Score negamax(Position& pos, Score alpha, Score beta, int depth, int ply);
    Score quiesce(Position& pos, Score alpha, Score beta, int ply);

    int  move_score(const Position& pos, Move m) const;
    void order_moves(const Position& pos, MoveList& ml) const;

    void    init_timer(const Position& pos, const SearchLimits& limits);
    bool    check_time();
    int64_t elapsed_ms() const;

    std::string format_pv(Move first) const;
    void        report(int depth, int line, Score score, Move first) const;

    std::atomic<bool> stop_requested{false};

    bool     stopped    = false;
    int64_t  start_ms   = 0;
    int64_t  alloc_ms   = 0;
    uint64_t nodes      = 0;
    uint64_t node_limit = 0;
    int      multipv    = 1;

    Move root_best_move = MOVE_NONE;

    Move pv_table[MAX_PLY + 1][MAX_PLY + 1];
    int  pv_length[MAX_PLY + 1];
};
