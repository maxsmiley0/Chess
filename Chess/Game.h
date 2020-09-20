#ifndef Game_h
#define Game_h

#include <string>
#include "Tuple.h"

class Board;

class Game
{
public:
    //Housekeeping
    Game(Board* b);                 //Constructor given a board
    ~Game();                        //Destructor (deletes board)
    
    //Move Input
    bool isValid(string s);         //Returns true if a syntactically valid input
    Coord convert(char c1, char c2);//Converts input string into movePiece instructions
    
    //Game related functions
    void play();                    //Loops until game over
    bool gameOver();                //Returns true if no legal moves for a given sign
    
private:
    //Game related members
    Board* mBoard;                  //Pointer to the game board
    char playerColor;               //Color of the human player
    
    //Developer statistics
    /*
     A list of a tuple of type double and int. Typically used in conjunction with the timer class, to push_back information regarding the time an algorithm takes to execute, to be displayed after the game has ended
     */
    list<Tuple> data;
    //A list of the player moves, effectively a PGN generator
    list<string> moves;
    int difficulty; //number of plys the computer will search
};

#endif /* Game_h */
