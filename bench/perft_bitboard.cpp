#include "../bitboard/src/board.h"
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
    init_keys();

    int depth = argc > 1 ? std::atoi(argv[1]) : 6;

    Brd brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto start = std::chrono::steady_clock::now();
    long long nodes = perft_driver(brd, depth);
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count();

    double seconds = static_cast<double>(micros) / 1e6;

    std::cout << "nodes   " << nodes << '\n'
              << "seconds " << seconds << '\n'
              << "nps     " << static_cast<long long>(nodes / seconds) << std::endl;
    return 0;
}
