#include "../v3/src/bitboard.h"
#include "../v3/src/movegen.h"
#include "../v3/src/position.h"
#include "../v3/src/zobrist.h"
#include <chrono>
#include <cstdlib>
#include <iostream>

static uint64_t perft(Position& pos, int depth) {
    if (depth == 0) return 1;

    uint64_t nodes = 0;
    MoveList moves = generate_moves(pos);
    for (Move m : moves) {
        pos.do_move(m);
        nodes += perft(pos, depth - 1);
        pos.undo_move(m);
    }
    return nodes;
}

int main(int argc, char* argv[]) {
    Zobrist::init();
    init_bitboards();

    int depth = argc > 1 ? std::atoi(argv[1]) : 6;

    Position pos;
    pos.set("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto start = std::chrono::steady_clock::now();
    uint64_t nodes = perft(pos, depth);
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count();

    double seconds = static_cast<double>(micros) / 1e6;

    std::cout << "nodes   " << nodes << '\n'
              << "seconds " << seconds << '\n'
              << "nps     " << static_cast<uint64_t>(nodes / seconds) << std::endl;
    return 0;
}
