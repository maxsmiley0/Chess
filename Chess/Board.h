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
    //Housekeeping
    Board();                                                //Constructor
    Board(vector<vector<string>> b);                        //Copy constructor from 2D vector
    Board(const Board& other);                              //Copy constructor from reference
    ~Board();                                               //Destructor (deletes all pieces)
    bool operator==(const Board& other);
    
    //Accessors
    Piece* getPiece(Coord c) const;                //Returns pointer to piece at coord c
    Piece* getKing(char color);                    //Returns pointer to king of given color
    char getTurn();
    list<Piece*> canReachKingPseudo(char color);   //Returns pieces that can reach king
    list<Piece*> canReachCoordPseudo(Coord c, char color);  //Same^ but for a given coord
    
    //Mutators
    void nextTurn();                    //Changes turn
    void addPiece(Piece* p);            //Adds piece
    //Moves piece, and updates all pieces' legal moves, attackers, defenders and pin direction
    void movePiece(Piece* p, Coord c);
    
    //Updating
    void update();                      //updates all pieces' attackers and defenders
    void updatePinDir(char color);      //updates all pieces' pin directions
    void updateLegalMoves();            //updates all pieces' legal moves
    
    //Functional
    bool boolCanReachCoordPseudo(Coord c, char color); //returns true if any piece can reach a given coordinate
    void print();   //prints a representation of the board to cout
   
    list<Piece*> whitePieces;   //list of white pieces
    list<Piece*> blackPieces;   //list of black pieces
    //Note*: These members are public to avoid writing methods to push_back and access elements from these lists
    
private:
    Piece* mBoard[8][8];        //Board representation as pointers to pieces
                                //Same pointers to pieces as in the list white/blackPieces
    char turn;                  //'W' or 'B' for white or black
};

#endif /* Board_h */

 
