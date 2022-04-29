#ifndef board_h
#define board_h

#include "defs.h"

struct Brd {
    //Pieces
    map pce[12];    //piece map, indexed by piece type
    map side[2];    //side map, indexed by side
    map occ;        //all occupancies

    //State
    int color;
    int enpas;
    int castleperms;

    //History
    Brd* last_state;             //pointer to last board state
    int his_ply;                 //stores what number ply of the game we are on
    int fifty_move;              //Last ply when moving pawn or capture or promotion
};

//Returns true if square is under attack
static inline bool is_sq_atk(Brd brd, int square, int color);

//Returns true if the side to move is in check
static inline bool in_check(Brd brd);

//Given a square return character of piece on it
char pce_on_square(int square, Brd brd);

//Given a fen string, returns a board loaded to that fen
Brd parse_fen(std::string fen);

//Given a board, returns a list of generated (pseudo-legal) moves in the position
std::list<int> generate_moves(Brd brd);

//Returns deep copy of board
Brd copy_board(Brd& brd);

//Takes back last move, assumes there is a move to take back
void take_back(Brd& brd);

//make move function! also pay attention to his_ply and fifty_move blah blah blah. hashes after perft driver?
//then work on improvement...
//Makes a move on a given board, returns true if move was made (legal)
bool make_move(Brd& brd, int move);

#endif