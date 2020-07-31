#ifndef Board_h
#define Board_h

#include <iostream>

#include <list>
#include <vector>

using namespace std;

class Coord;
class Piece;

class Board
{
public:
    //housekeeping
    Board();
    Board(const Board& other);
    //essentially copy constructor given a 2D array
    Board(vector<vector<string>> b);
    bool operator==(const Board& other);
    ~Board();
    
    //functional
    Piece* getPiece(Coord c) const;
    Piece* getKing(char color);
    char getTurn();
    void nextTurn();
    void addPiece(Piece* p);
    void movePiece(Piece* p, Coord c);
    //Moveraw moves pieces without checking if legal move, so faster
    //We do this to calculate hypothetical moves which we already know are legal
    void moveRaw(Piece* p, Coord c);
    void updateLegalMoves(char color);
    list<Piece*> canReachKingPseudo(char color);
    
    unsigned long hashmap(); //returns a hashkey
    
    //game auxillary
    //returns a list of pointers to pieces that can reach a given coordinate of a given color
    //e.g. which black pieces can reach e4?
    bool boolCanReachCoordPseudo(Coord c, char color);
    list<Piece*> canReachCoordPseudo(Coord c, char color);
    //updates pin direction tags to each piece of a given color
    void update(); //updates all piece's attackers and defenders
    void updatePinDir(char color);
    void print();
    
    /*
     Only doing this because I couldn't find out how to return a list... the obvious way wasn't returning the correct list, broke when I tried to dereference the iterator
     */
    
    list<Piece*> whitePieces;
    list<Piece*> blackPieces;
    
private:
    Piece* mBoard[8][8];
    char turn;
};

#endif /* Board_h */

 
