//
//  Game.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 10/4/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "game.h"

Game::Game()
{
    mSearch = new Searcher();
    gameOver = false;
}

Game::~Game()
{
    delete mSearch;
}

void Game::play()
{
    //Resetting board to starting position
    getBoard()->parseFen(startFen);
    
    //Which side will the player be?
    while (true)
    {
        std::string side;
        std::cout << "Would you like to play white or black? (W / B)" << std::endl;
        std::cin >> side;
        
        if (side == "W" || side == "w")
        {
            playerColor = WHITE;
            break;
        }
        else if (side == "B" || side == "b")
        {
            playerColor = BLACK;
            break;
        }
        else
        {
            std::cout << "Invalid color" << std::endl;
        }
    }
    
    //Loop while game is not over
    while (!gameOver)
    {
        clearScreen();
        getBoard()->printBoard();
        
        std::string move;
        std::cin >> move;
        
        //vet moves somewhere in here
        
        int f1 = 7 + (int)'1' - (int)move[1];
        int f2 = (int)move[0] - (int)'a';
        int f3 = 7 + (int)'1' - (int)move[3];
        int f4 = (int)move[2] - (int)'a';
        
        int pce = NOPIECE;
        getMoveGenerator()->makeMove(getMoveGenerator()->getMove(f1, f2, f3, f4, pce));
        clearScreen();
        
        getBoard()->printBoard();
        getMoveGenerator()->makeMove(mSearch->reccomendMove());
    }
}
