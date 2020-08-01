#ifndef auxiliary_h
#define auxiliary_h

#include <list>
#include "Tuple.h"

class Board;
class Piece;
class Set;
class Coord;

using namespace std;

/*
 Searching-related functions
 */
//Searches "depth" nodes into the game tree, using alpha-beta pruning
TuplePC reccomendMove(Board* b, char turn, int depth, double alpha, double beta);
//Static eval called at the leaf nodes of reccomendMove
double eval(Board* b, char color);

//Eval for a given gamestage
double evalOpening(Board* b, char color);
double evalMiddleGame(Board* b, char color);
double evalEndGame(Board* b, char color);

/*
 Auxiliary functions used in eval
 */
double evalPst(Board* b);   //positional bonuses assigned using a piece square table
double evalPstEg(Board* b); //endgame piece square table
//penalty for unprotected attacked pieces, or for pieces attacked by lesser pieces
double evalPiece(Board* b, char color);
//penalty for doubled / isolated pawns
double evalPawn(Board* b, char color);
//penalty for moving queen / rook out before castling or middlegame
double majorPiecePenalty(Board* b, char color);

//Ray-related functions

//Returns a ray to the king, modifies dir to represent ray direction (not inclusive)
const Set getRay(const Piece* p, char& dir);
//Defines a direction for a given ray
const char defineDir(const Coord c1, const Coord c2);
//returns true if there exists an unperturbed ray from p's location to c

//Misc

//returns true if a list contains a coord
bool containsCoord(list<Coord> li, Coord c);
//Returns the piece pinning piece p
Piece* getPinned(const Piece* p, char& dir);

#endif /* auxiliary_h */

//Timer class, to be used in measuring time of certain functions to see what can be improved upon

#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};
