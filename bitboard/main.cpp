#include "board.h"

int main()
{
    Brd brd = parse_fen("rnb1kbnr/ppp1qppp/8/2ppP3/8/8/PPPP1PPP/RNBQKBNR w KQkq d6 0 1");
    std::list<int> moves = generate_moves(brd);
    print_moves(moves);
    make_move(brd, 5244124);
    print_board(brd);
}

/*
For moves:
//optimize pawn moves at end..?
//also keep array of # so don't have to loop over captured pieces!
make move
generate move
organize
*/