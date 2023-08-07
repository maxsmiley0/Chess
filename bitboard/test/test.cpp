#include "../src/board.h"
#include "test.h"

TestFramework::TestFramework() {

}

int TestFramework::getMove(Brd& brd, int src_sq, int tar_sq) {
    MoveList move_list[1];
    generate_moves(brd, move_list);

    for (int move_cnt = 0; move_cnt < move_list->cnt; move_cnt++)
    {
        int move = move_list->move[move_cnt];

        int move_src_sq = get_move_source(move);
        int move_tar_sq  = get_move_target(move);
        
        if (src_sq == move_src_sq && tar_sq == move_tar_sq) {
            return move;
        }
    }

    return -1;
}

void TestFramework::parseMoves(Brd& brd, vector<pair<int, int>> moves) {
    for (int i = 0; i < moves.size(); i++) {
        int src_sq = moves[i].first;
        int tar_sq = moves[i].second;

        if (src_sq < a8 || src_sq > h1 || tar_sq < a8 || tar_sq > h1) {
            std::cout << "Invalid square in input" << std::endl;
            return;
        }
        
        int move = getMove(brd, src_sq, tar_sq);
        if (move == -1) {
            std::cout << "Illegal move input" << std::endl;
            return;
        }
        make_move(brd, move);
        std::cout << "Moved " << pce_char[get_move_piece(move)] << " from " << square_to_coordinates[src_sq] << " to " << square_to_coordinates[tar_sq] << std::endl;
        std::cout << "PK: " << brd.poskey << std::endl;
    }
}

void TestFramework::initCaptureTest(string fen) {
    captureBoard = parse_fen(fen);
    hint = get_starter_hint(captureBoard);
}

int TestFramework::nextCapture() {
    return generate_capture_incr(captureBoard, hint);
}