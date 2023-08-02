#include "../src/board.h"
#include "test.h"
#include <cassert>

void validate_perft() {
    Brd brd_open = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Brd brd_mid = parse_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -");
    Brd brd_end = parse_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ");

    int open_nodes = perft_driver(brd_open, 6);
    int mid_nodes = perft_driver(brd_mid, 5);
    int end_nodes = perft_driver(brd_end, 7);

    assert(open_nodes == 119060324);
    assert(mid_nodes == 193690690);
    assert(end_nodes == 178633661);
}

void init_speed_test() {
    Brd brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Timer t1;
    t1.start();
    std::cout << perft_driver(brd, 6) << std::endl;
    std::cout << t1.elapsed() << std::endl;
}

void validate_poskey() {
    TestFramework test;

    Brd brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");            
    test.parseMoves(brd, {{e2, e4}});

    Brd cmp_brd = parse_fen("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");     //Initial position - e4
    assert(brd.poskey == cmp_brd.poskey);

    test.parseMoves(brd, {{g8, f6}, {b1, c3}, {e7, e6}, {g1, f3}, {f8, e7}, {f1, c4}});         //Opening sequence of moves
    cmp_brd = parse_fen("rnbqk2r/ppppbppp/4pn2/8/2B1P3/2N2N2/PPPP1PPP/R1BQK2R b KQkq - 0 1");
    assert(brd.poskey == cmp_brd.poskey);
    
    test.parseMoves(brd, {{e8, f8}});
    cmp_brd = parse_fen("rnbq1k1r/ppppbppp/4pn2/8/2B1P3/2N2N2/PPPP1PPP/R1BQK2R w KQ - 0 1");    //Moving black king properly hashes out the castle permissions
    assert(brd.poskey == cmp_brd.poskey);

    test.parseMoves(brd, {{h1, f1}});
    cmp_brd = parse_fen("rnbq1k1r/ppppbppp/4pn2/8/2B1P3/2N2N2/PPPP1PPP/R1BQKR2 b Q - 0 1");     //Moving white rook properly hashes out the castle permissions
    assert(brd.poskey == cmp_brd.poskey);

    test.parseMoves(brd, {{c7, c5}, {e4, e5}, {d7, d5}, {e5, d6}});
    cmp_brd = parse_fen("rnbq1k1r/pp2bppp/3Ppn2/2p5/2B5/2N2N2/PPPP1PPP/R1BQKR2 b Q - 0 1");     //En passant capture
    assert(brd.poskey == cmp_brd.poskey);

    test.parseMoves(brd, {{d8, d6}, {d2, d4}, {b8, c6}, {c1, h6}, {g7, h6}, {d1, e2}, {a8, b8}});   //Sequence of movesup to next significant test
    cmp_brd = parse_fen("1rb2k1r/pp2bp1p/2nqpn1p/2p5/2BP4/2N2N2/PPP1QPPP/R3KR2 w Q - 0 1");
    assert(brd.poskey == cmp_brd.poskey);

    test.parseMoves(brd, {{e1, c1}});                                                           //White queenside castling
    cmp_brd = parse_fen("1rb2k1r/pp2bp1p/2nqpn1p/2p5/2BP4/2N2N2/PPP1QPPP/2KR1R2 b - - 0 1");
    assert(brd.poskey == cmp_brd.poskey);
}

int main() {
    init_keys();

    validate_perft();
    init_speed_test();
    validate_poskey();
    
    return 0;
}