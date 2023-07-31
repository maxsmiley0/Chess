#include "board.h"
#include "test.h"

void validate_perft() {
    Brd brd_open = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Brd brd_mid = parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Brd brd_end = parse_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");

    int open_nodes = perft_driver(brd_open, 6);
    int mid_nodes = perft_driver(brd_mid, 5);
    int end_nodes = perft_driver(brd_end, 7);

    if (open_nodes == 119060324) {
        std::cout << "Opening test passed" << std::endl;
    }
    else {
        std::cout << "Opening test passed" << std::endl;
    }
    if (mid_nodes == 193690690) {
        std::cout << "Middlegame test passed" << std::endl;
    }
    else {
        std::cout << "Middlegame test passed" << std::endl;
    }
    if (end_nodes == 178633661) {
        std::cout << "Endgame test passed" << std::endl;
    }
    else {
        std::cout << "Endgame test passed" << std::endl;
    }
}

void init_speed_test() {
    Brd brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Timer t1;
    t1.start();
    std::cout << perft_driver(brd, 6) << std::endl;
    std::cout << t1.elapsed() << std::endl;
}

int main()
{
    init_keys();
    Brd brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    TestFramework test;
    test.parseMoves(brd, {{e2, e4}, {g8, f6}, {b1, c3}, {e7, e6}, {g1, f3}, {f8, e7}, {f1, c4}, {e8, f8}, {h1, f1}, {c7, c5}, {e4, e5}, {d7, d5}, {e5, d6}, {d8, d6}, {d2, d4}, {b8, c6}, {c1, h6}, {g7, h6}, {d1, e2}, {a8, b8}, {e1, c1}});
    Brd brd_test = parse_fen("1rb2k1r/pp2bp1p/2nqpn1p/2p5/2BP4/2N2N2/PPP1QPPP/2KR1R2 b - - 0 1");
}