/*
Initialize testing session
Init begin_board
Init end_board
Given a series of moves, execute them
Algebraic convert machine readable to human readable;
*/

#ifndef TEST_H
#define TEST_H

class Brd;

#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

class TestFramework {
public:
    TestFramework();
    void parseMoves(Brd& brd, vector<pair<int, int>> moves);
    void initCaptureTest(string fen);
    int nextCapture();
private:
    MoveHint hint;
    Brd captureBoard;
    int getMove(Brd& brd, int src_sq, int tar_sq);
};

#endif