//
//  Game.hpp
//  strawberrymangoes
//
//  Created by Max Smiley on 10/4/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include "searcher.h"

class Game
{
public:
    Game();
    ~Game();
    /*
    Description of play:
    First: sets the board to the start fen
    Second: prompts user for the side they want to play
    Third: loops until game ends, with each iteration prompting the user to input a move, while the computer outputs a move
    Fourth: prints the game result to cout
    */
    void play();
    
private:
    //Accessors
    Searcher* getSearcher() const {return mSearch; }
    Movegen* getMoveGenerator() const {return mSearch->getMoveGenerator(); }
    Board* getBoard() const {return mSearch->getBoard(); }
    
    //Components of the main play method
    void chooseSide();           //prompts the user to choose the side
    void runGame();              //the "back and forth" component of play
    
    //Processing moves
    int processPlayerMove(Movegen* mGen, Searcher* ponderSearch = nullptr);   //handles move input if not computer mode. If pondering, input the searcher used to ponder to send stop signal to, otherwise leave blank. Returns the move
    void processComputerMove(Movegen* mGen); //handles move input if computer mode, with respect to a given Move Generator
    
    //Writes and clears files. Methods are responsible for error checking
    void writeMove(int move);             //Always writes to outSocket
    void clearSocket(std::string socket);//Clears a socket (can clear any file)
    
    //Auxiliary runGame helper methods
    void checkGameStatus();     //checks if the game is over
    bool vetMove(Movegen* mGen, int move);     //returns true if move is allowed
    unsigned long legalMoves(const std::list<int> moves);  //return number of actual legal moves, discounting all pseudolegal moves that would result in king being in check
    
    //Members
    int playerColor;
    bool gameOver;
    const std::string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
    Searcher* mSearch;
};

#endif /* Game_h */
