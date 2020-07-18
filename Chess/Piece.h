#ifndef Piece_h
#define Piece_h

#include "Coord.h"
#include <list>

class Set;
class Board;

class Piece
{
public:
    /*
     Defines a piece with a board pointer, a current position coord, and a color
     */
    Piece(Board* b, Coord c, char col);
    bool operator!=(const Piece& other);
    virtual ~Piece();
    
    /*
     Need to separate clauses so no cylical dependancies
     
     A move is legal IFF:
     It can be reached
     It can be reached without going in check
     */
    
    Coord getPos() const;
    char getColor() const;
    char getOppositeColor() const;
    Board* getBoard() const;
    char getPinDir() const;
    
    void setPinDir(char c);
    void setMoved(bool b);
    void setPos(Coord c);
    
    bool inCheck() const; //King overwrites this, crashes if called on non-king (in a given board)
    bool pseudoLegalMove(Coord c) const; 
    bool legalMove(Coord c) const; //returns if the move that would end up with the piece here is legal
    bool hasMoved() const; //returns if a piece has moved or not
    Piece* getWeakestDefender(); //returns the weakest defender to be used in Auxiliary::evalPiece
    Piece* getWeakestAttacker(); //returns the weakest attacker to be used in Auxiliary::evalPiece
    
    int getNumDefending() const; //only used by king in determining king ring safety levels
    void incrementNumDefending(); //only used by king in determining king ring safety levels
    void setNumDefending(int i);
    
    int getNumAttacking() const; //only used by king
    void incrementNumAttacking(); //only used by king
    void setNumAttacking(int i);
    
    virtual bool canReachEnemyKing() const = 0; //optimized version of pseudoLegalMoves() fit for finding the enemy king
    virtual double worth() const = 0;
    virtual char type() const = 0; //returns the name character e.g. "K" for king
    virtual Set pseudoLegalMoves() const = 0; //returns a set of pseudolegal moves from given pos
    virtual Set legalMoves() const = 0; //returns a set of legal moves from given pos
    virtual double centerControl() const; //returns a bonus for center control, convention is always positive so it will be up to Auxiliary::centerControl to adhere to sign conventions
    
    //Updates list of pieces it attacks and defends
    //We assume not in check
    virtual void update() = 0;
    
    //List of pieces of opposite color attacking this piece (legally)
    std::list<Piece*> attackers;
    //List of pieces of same color defending this piece (legally)
    std::list<Piece*> defenders;
    
private:
    
    Board* mBoard;
    Coord pos;
    char color;
    bool moved;
    char pinDir;
    int numDefending;
    int numAttacking;
};

#endif /* Piece_h */

//Some function needs to return a set of all legal moves, or a pointer to the first node
//use byte or short? since x, y are 0 to 7 basically
