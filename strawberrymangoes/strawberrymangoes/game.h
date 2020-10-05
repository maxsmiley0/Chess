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
    
    void play();
    /*
     loops while game over, cin
     some kind of vet function
     */
    //reset game
    
    Searcher* getSearcher() const {return mSearch; }
    Movegen* getMoveGenerator() const {return mSearch->getMoveGenerator(); }
    Board* getBoard() const {return mSearch->getBoard(); }
    //accessors for movegen, searcher, board
    
private:
    
    void chooseSide();          //
    void runGame();             //
    void checkGameStatus();         
    bool vetMove(int move);     //returns true if move is allowed
    
    unsigned long legalMoves(const std::list<int> moves);  //
    
    Searcher* mSearch;
    bool gameOver;
    const std::string startFen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -";
    int playerColor;
    
    //Checks when game is over
    //Start fen
    //vet fnct
    //searcher
    //player side
};

#endif /* Game_h */
