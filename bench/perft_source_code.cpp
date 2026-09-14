#include "../source_code/movegen.h"
#include <chrono>
#include <iostream>
#include <list>

static long long perft(Movegen& gen, int depth) {
    if (depth == 0) return 1;

    long long nodes = 0;
    std::list<int> moves = gen.generateMoves();
    for (int move : moves) {
        if (gen.makeMove(move)) {
            nodes += perft(gen, depth - 1);
            gen.takeBack();
        }
    }
    return nodes;
}

int main(int argc, char* argv[]) {
    int depth = argc > 1 ? std::atoi(argv[1]) : 6;

    Movegen gen;
    gen.getBoard()->parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    auto start = std::chrono::steady_clock::now();
    long long nodes = perft(gen, depth);
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count();

    double seconds = static_cast<double>(micros) / 1e6;

    std::cout << "nodes   " << nodes << '\n'
              << "seconds " << seconds << '\n'
              << "nps     " << static_cast<long long>(nodes / seconds) << std::endl;
    return 0;
}
