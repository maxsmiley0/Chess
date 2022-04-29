#include "Board.h"
#include <iostream>
#include <fstream>

/*
                           castling   move     in      in
                              right update     binary  decimal
 king & rooks didn't move:     1111 & 1111  =  1111    15
        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13
     
         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7
*/

// castling rights update constants
const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

int main()
{
    Brd brd = parse_fen("rnbqk1nr/pppppppp/8/8/8/8/PPPPb1PP/R3K2R w KQkq - 0 1");
    print_board(brd);
    generate_moves(brd);
}

/*
For moves:
make move
generate move
organize
*/