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
    getBoard()->parseFen(startFen);     //Resetting board to starting position
    chooseSide();                       //Prompts the user to input a side
    runGame();                          //Loop while game is not over
    
    //Output who won
    int side = getBoard()->getSide();
    int kingR = getBoard()->getKingR(side);
    int kingC = getBoard()->getKingC(side);
    
    bool inCheck = getMoveGenerator()->squareAttacked(kingR, kingC, side);
    //If no legal moves and in check
    if (inCheck && getBoard()->numRep() < 2)
    {
        if (getBoard()->getSide() == WHITE)
        {
            std::cout << "Black wins by checkmate" << std::endl;
        }
        else
        {
            std::cout << "White wins by checkmate" << std::endl;
        }
    }
    else
    {
        if (getBoard()->numRep() >= 2)
        {
            std::cout << "Game Drawn: 3 fold repetition" << std::endl;
        }
        else
        {
            std::cout << "Game Drawn: stalemate" << std::endl;
        }
    }
}

void Game::chooseSide()
{
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
        else if (side == "#exit")
        {
            exit(1);
        }
        else
        {
            std::cout << "Invalid color" << std::endl;
        }
    }
}

bool Game::vetMove(int move)
{
    bool isPseudoLegal = false;
    std::list<int> moves = getMoveGenerator()->generateMoves();
    //Ensure the inputted move is at least a pseudolegal move
    for (std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        if (*itr == move)
        {
            isPseudoLegal = true;
            break;
        }
    }
    
    if (!isPseudoLegal)
    {
        return false;
    }
    //Otherwise, make the move
    return getMoveGenerator()->makeMove(move);
}

void Game::runGame()
{
    while (!gameOver)
    {
        if (getBoard()->getSide() == playerColor)
        {
            clearScreen();
            getBoard()->printBoard(playerColor);
            std::cout << "Enter a move: " << std::endl;
            std::string input;
            std::cin >> input;
            
            //Exiting program special command
            if (input == "#exit")
            {
                exit(1);
            }
            
            //Maps the move string into the four move coordinates
            int f1 = 7 + (int)'1' - (int)input[1];
            int f2 = (int)input[0] - (int)'a';
            int f3 = 7 + (int)'1' - (int)input[3];
            int f4 = (int)input[2] - (int)'a';
            
            int pce = NOPIECE;
            
            //Promotion cases
            if (f3 == 0 && isPawn(getBoard()->getPce(f1, f2)))
            {
                pce = WQ;
            }
            else if (f4 == 7 && isPawn(getBoard()->getPce(f1, f2)))
            {
                pce = BQ;
            }
            
            //Construct the move key
            int move = getMoveGenerator()->getMove(f1, f2, f3, f4, pce);
            
            //Ensure valid input (legal move)
            if (!vetMove(move))
            {
                std::cout << "Illegal Move" << std::endl;
                continue;
            }
            
            checkGameStatus();
            if (gameOver) {clearScreen(); getBoard()->printBoard(playerColor); break;}
        }
        else
        {
            clearScreen();
            //Computer makes move
            getBoard()->printBoard(playerColor);
            std::cout << "Computer is thinking..." << std::endl;
            getMoveGenerator()->makeMove(mSearch->reccomendMove());
            
            checkGameStatus();
            if (gameOver) {clearScreen(); getBoard()->printBoard(playerColor); break;}
        }
    }
}

void Game::checkGameStatus()
{
    //If no legal moves, or 3 fold repetition is true
    
    if (legalMoves(getMoveGenerator()->generateMoves()) <= 0 || getBoard()->numRep() >= 2)
    {
        gameOver = true;
    }
}

unsigned long Game::legalMoves(const std::list<int> moves)
{
    unsigned long size = moves.size();
    
    for (std::list<int>::const_iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        if (getMoveGenerator()->makeMove(*itr))
        {
            getMoveGenerator()->takeBack();
        }
        else
        {
            size--;
        }
    }
    
    return size;
}
