#include "board.h"

int main()
{
    Brd brd = parse_fen("rnbqkbnr/8/8/8/8/8/8/RNBQKBNR b KQkq - 0 1");
    Brd new_brd = parse_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    Brd new_brd_2 = parse_fen("rnb2bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    new_brd.last_state = &brd;
    new_brd_2.last_state = &new_brd;
    print_board(new_brd_2);
}

/*
For moves:
//optimize pawn moves at end..?
//also keep array of # so don't have to loop over captured pieces!
make move
generate move
organize
*/