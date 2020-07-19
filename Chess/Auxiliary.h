#ifndef auxiliary_h
#define auxiliary_h

#include <list>
#include "Tuple.h"
class Board;
class Piece;
class Set;
class Coord;

/*
 //////////////
 THE MAIN BRAIN - evaluate function
 //////////////
 Will be a function of a pointer to a board, and a char color (whose turn it is)
 Checkmate is weighted as 200
 Sum up all values of white pieces and subtract by black pieces
 Pawn score: doubled, isolated, and blocked pawns
 Mobility score: simply number of all legal moves
 
 For this to work, we need to detect if pieces are protected
 */

TuplePC reccomendMove(Board* b, char turn, int depth, double alpha, double beta);

double eval(Board* b, char color);

//eval for a given gamestage
double evalOpening(Board* b, char color);
double evalMiddleGame(Board* b, char color);
double evalEndGame(Board* b, char color);
//returns the sum of all PST bonuses
double evalPst(Board* b);
//returns the sum of all PST bonuses for the endgame
double evalPstEg(Board* b);

//auxiliary eval functions
double evalPiece(Board* b, char color);
double evalPawn(Board* b, char color);
double evalPassedPawn(Board* b, char color);
double centerBonus(Board* b, char color);
double majorPiecePenalty(Board* b, char color);
//To be used in opening / middlegame
double evalKingSafety(Board* b, char color);
//To be used in the endgame
double evalKingActivity(Board* b, char color);
 
/*
functional
*/
const bool isAttacked(const Piece* p);
const bool isPinned(const Piece* p);

//Recommends a piece to go to a certain coord for a given board, turn, and # of half steps ahead
 
/*
 Ray functions
 */
//returns true if a list contains a coord
bool containsCoord(list<Coord> li, Coord c);
//Returns a ray to the king, modifies dir to represent ray direction (not inclusive)
const Set getRay(const Piece* p, char& dir);
//Returns a ray to the coord c
const Set getGeneralRay(const Piece* p, Coord c);
//Returns the piece this piece pins (or nullptr if none)
Piece* getPinned(const Piece* p, char& dir);
/*
 Defines a direction V for vertical, H for horizontal, D for UL vector, d for UR vector, and N for no direction. To be used in Piece::legalMove
 */
const char defineDir(const Coord c1, const Coord c2);


#endif /* auxiliary_h */

/*
for (list<Piece*>::iterator itr = mBoard->blackPieces.begin(); itr != mBoard->blackPieces.end(); itr++)
{
    if ((*itr)->type() == 'Q')
    {
        getRay(*itr).dump();
    }
}
*/

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

//LinReg on legal moves vs time
