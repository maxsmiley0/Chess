#include "../v3/src/bitboard.h"
#include "../v3/src/movegen.h"
#include "../v3/src/position.h"
#include "../v3/src/zobrist.h"
#include <chrono>
#include <cstdio>
#include <cstdlib>

static MoveList stack_lists[32];

static uint64_t perft_value_return(Position& pos, int depth) {
    if (depth == 0) return 1;
    uint64_t nodes = 0;
    MoveList moves = generate_moves(pos);
    for (Move m : moves) {
        pos.do_move(m);
        nodes += perft_value_return(pos, depth - 1);
        pos.undo_move(m);
    }
    return nodes;
}

static uint64_t perft_out_param(Position& pos, int depth) {
    if (depth == 0) return 1;
    MoveList& moves = stack_lists[depth];
    moves.size = 0;
    generate_moves(pos, moves);
    uint64_t nodes = 0;
    for (Move m : moves) {
        pos.do_move(m);
        nodes += perft_out_param(pos, depth - 1);
        pos.undo_move(m);
    }
    return nodes;
}

static uint64_t perft_bulk(Position& pos, int depth) {
    MoveList& moves = stack_lists[depth];
    moves.size = 0;
    generate_moves(pos, moves);
    if (depth == 1) return moves.size;
    uint64_t nodes = 0;
    for (Move m : moves) {
        pos.do_move(m);
        nodes += perft_bulk(pos, depth - 1);
        pos.undo_move(m);
    }
    return nodes;
}

static uint64_t gen_only(Position& pos, int depth) {
    MoveList& moves = stack_lists[depth];
    moves.size = 0;
    generate_moves(pos, moves);
    if (depth == 1) return moves.size;
    uint64_t nodes = 0;
    for (Move m : moves) {
        pos.do_move(m);
        nodes += gen_only(pos, depth - 1);
        pos.undo_move(m);
        MoveList& again = stack_lists[0];
        again.size = 0;
        generate_moves(pos, again);
        nodes += again.size == 0 ? 1 : 0;
    }
    return nodes;
}

template<typename Fn>
static void run(const char* label, Fn fn, int depth) {
    Position pos;
    pos.set(START_FEN);
    auto start = std::chrono::steady_clock::now();
    uint64_t nodes = fn(pos, depth);
    double seconds = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count() / 1e6;
    printf("%-22s nodes %12llu   %6.3fs   %8.1f Mnps\n",
           label, (unsigned long long)nodes, seconds, nodes / seconds / 1e6);
}

int main(int argc, char* argv[]) {
    Zobrist::init();
    init_bitboards();
    int depth = argc > 1 ? std::atoi(argv[1]) : 6;

    run("value-return", perft_value_return, depth);
    run("out-param", perft_out_param, depth);
    run("bulk-count", perft_bulk, depth);
    run("gen twice per node", gen_only, depth);
    return 0;
}
