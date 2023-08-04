#ifndef board_h
#define board_h

#include "defs.h"

struct MoveList {
    int move[256];
    int cnt;
};

struct Brd {
    //Pieces
    bitbrd pce[12];    //piece bitbrd, indexed by piece type
    bitbrd side[2];    //side bitbrd, indexed by side
    bitbrd occ;        //all occupancies

    //State
    int color;
    int enpas;
    int castleperms;
    int fifty_move;              //Last ply when moving pawn or capture or promotion
    int poskey;
};

//Initialization
Brd parse_fen(std::string fen);  //Given a fen string, returns a board loaded to that fen
void init_keys();                //Key related functions for Zobrist hashing

//Querying board state
static inline bool is_sq_atk(const Brd& brd, int square, int color);    //Returns true if square is under attack
static inline bool in_check(const Brd& brd);                            //Returns true if the side to move is in check
char pce_on_square(int square, const Brd& brd);                         //Given a square return character of piece on it
int static_eval(const Brd& brd);                                        //Centipawn score of NNUE eval

//Move generation
static inline void add_move(MoveList* move_list, int move);
void generate_moves(const Brd& brd, MoveList* move_list);               //Given a board, returns a list of generated (pseudo-legal) moves in the position

//Board operations 
bool make_move(Brd& brd, int move);                                     //Makes a move on a given board, returns true if move was made (legal)
int perft_driver(Brd& brd, int depth);                                  //Returns the leaf nodes after a given depth of a position
static inline Brd copy_board(const Brd& brd);                           //Returns a deep copy of the board struct

/*
   ***   INTERFACE TO INTERACT WITH BRD   ***   
Use these functions, as they will ensure that the bitboards and keys are properly set
*/

static inline void add_pce(Brd& brd, int pce, int sq);                  //Sets the bit in the according b.pce[] and updates the poskey
static inline void remove_pce(Brd& brd, int pce, int sq);               //Removes the bit in the according b.pce[] given known pce and updates the poskey
static inline void hashInCastle(Brd& brd, int sq);                      //Adds the castleperms and poskey (does not move any pieces)
static inline void hashOutCastle(Brd& brd, int src_sq, int tar_sq);     //Updates the castleperms and poskey (does not move any pieces)
static inline void hashInEp(Brd& brd, int sq);                          //Sets the ep, and updates position key, assuming no ep
static inline void hashOutEp(Brd& brd);                                 //Resets the ep square to 0, and updates position key
static inline void changeSide(Brd& brd);                                //Changes the side and updates the poskey

#endif