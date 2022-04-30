#include "board.h"

int main()
{
    Brd brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Timer t1;
    t1.start();
    std::cout << perft_driver(brd, 6) << std::endl;
    std::cout << t1.elapsed() << std::endl;
}

/*
For moves:
//optimize pawn moves at end..?
//also keep array of # so don't have to loop over captured pieces!
make move
generate move
organize
*/