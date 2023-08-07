#include "../src/board.h"
#include "test.h"
#include <cassert>
#include <iostream>

using namespace std;

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

    //given a fen, I want to return a capture stream of 
    TestFramework test;
    //test.initCaptureTest("k7/8/8/8/8/8/8/K7 w - - 0 1");
    //cout << test.nextCapture() << endl;

    /*test.initCaptureTest("k7/8/8/8/8/8/q7/K7 w - - 0 1");
    print_move(test.nextCapture());
    print_move(test.nextCapture());

    test.initCaptureTest("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    print_move(test.nextCapture());

    test.initCaptureTest("rnbqkbnr/pppp2pp/8/4pp2/4PP2/8/PPPP2PP/RNBQKBNR w KQkq - 0 1");
    while (true) {
        int move = test.nextCapture();
        if (move == -1) break;
        print_move(move);
    }

    test.initCaptureTest("rnbqkbnr/pppp2pp/8/4pp2/4PP2/8/PPPP2PP/RNBQKBNR b KQkq - 0 1");
    while (true) {
        int move = test.nextCapture();
        if (move == -1) break;
        print_move(move);
    }*/
    /*test.initCaptureTest("rnbqkbnr/8/1p6/2pp1pp1/3PpP1p/2p1P1P1/PPP4P/RNBQKBNR w KQkq - 0 1");
    while (true) {
        int move = test.nextCapture();
        if (move == -1) break;
        print_move(move);
    }*/
    //crazy position!
    /*test.initCaptureTest("r1b3n1/1B3Q2/1p1k4/2pp1pp1/2qPpPnp/N1p1PbP1/PPPB1rRP/R3K1N1 w - - 0 1");
    while (true) {
        int move = test.nextCapture();
        if (move == -1) break;
        print_move(move);
    }*/
    test.initCaptureTest("r1b3n1/1B3Q2/1p1k4/2pp1pp1/2qPpPnp/N1p1PbP1/PPPB1rRP/R3K1N1 b - - 0 1");
    while (true) {
        int move = test.nextCapture();
        if (move == -1) break;
        print_move(move);
    }
    //TODO TMRW: handle enpas and promos to make a function that compares the valididty of generate_capture_incr with the list from generated_moves or make generate_captures
    //How much does QS actually save?
    /*validate_perft();
    init_speed_test();
    validate_poskey();
    */
    return 0;
}