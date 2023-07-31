#include "board.h"

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

    Brd brd_a = parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Brd brd_b = parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R b KQkq -");

    std::cout << (brd_a.poskey ^ brd_b.poskey) << std::endl;

    /*
    Moving a piece
    Destroying castling rights
    Enpas rights
    Side rights

    Testing protocol:
    initialize two boards, and use make_move on the other until it equals the same to show expected
    */
}