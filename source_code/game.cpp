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

bool Game::vetMove(Movegen* mGen, int move)
{
    bool isPseudoLegal = false;
    std::list<int> moves = mGen->generateMoves();
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
    return mGen->makeMove(move);
}

void Game::runGame()
{
    //Loops until game is over
    while (!gameOver)
    {
        //If player's turn
        if (getBoard()->getSide() == playerColor)
        {
            clearScreen();
            getBoard()->printBoard(playerColor);
            
            if (!computerMode)
            {
                if (ponder)
                {
                    //Dummy board to input moves on while real board is pondering
                    Movegen* dummyInput = new Movegen(*getMoveGenerator());
                    int move;
                    
                    //Thread 1: Ponder
                    std::thread ponderThread([this]{getSearcher()->ponder();});
                    //Thread 2: Await for user input. When valid input is given, send a signal to our searcher to stop pondering, and return the move so we can make it on the "real board"
                    std::thread awaitIO([&, this]{move = processPlayerMove(dummyInput, mSearch);});
                    
                    //In this body, the engine is calculating while waiting on input
                    //Join the threads
                    awaitIO.join();
                    ponderThread.join();
                    
                    //Make the move
                    getMoveGenerator()->makeMove(move);
                    //Cleanup dummy board
                    delete dummyInput;
                }
                else
                {
                    processPlayerMove(getMoveGenerator());
                }
                
                checkGameStatus();
                if (gameOver) {clearScreen(); getBoard()->printBoard(playerColor); break;}
            }
            else
            {
                processComputerMove();
                
                checkGameStatus();
                if (gameOver) {clearScreen(); getBoard()->printBoard(playerColor); break;}
            }
        }
        else
        {
            clearScreen();
            //Computer makes move
            getBoard()->printBoard(playerColor);
            std::cout << "Computer is thinking..." << std::endl;
            
            int move = mSearch->reccomendMove();
            getMoveGenerator()->makeMove(move);
            
            //When a move has been calculated on this board, it is logged into the arena
            if (computerMode)
            {
                FILE* port;
                port = fopen(outSocket, "r+");
                write(fileno(port), &move, sizeof(move));
                fclose(port);
            }
            
            checkGameStatus();
            if (gameOver) {clearScreen(); getBoard()->printBoard(playerColor); break;}
        }
    }
}

int Game::processPlayerMove(Movegen* mGen, Searcher* ponderSearch)
{
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
    if (f3 == 0 && isPawn(mGen->getBoard()->getPce(f1, f2)))
    {
        pce = WQ;
    }
    else if (f3 == 7 && isPawn(mGen->getBoard()->getPce(f1, f2)))
    {
        pce = BQ;
    }
    
    //Construct the move key
    int move = mGen->getMove(f1, f2, f3, f4, pce);
    
    //Ensure valid input (legal move)
    if (!vetMove(mGen, move))
    {
        std::cout << "Illegal Move" << std::endl;
        return processPlayerMove(mGen, ponderSearch);
    }
    else
    {
        //Legal move case
        if (ponderSearch)
        {
            ponderSearch->sendSIGSTOP();
        }
        return move;
    }
}

void Game::processComputerMove()
{
    int move = 0;
    int cnt = 0;
    FILE* port;
    
    //Loop until the other computer has made a move
    while (move == 0)
    {
        //Since syscalls are taxing, only check every 0xFFFFF iterations
        if ((cnt & 0xFFFFF) == 0)
        {
            port = fopen(inSocket, "r+");
            read(fileno(port), &move, sizeof(move));
            fclose(port);
        }
        
        cnt++;
    }
    
    //Ensuring valid move from other computer
    if (!getMoveGenerator()->makeMove(move))
    {
        std::cerr << "Computer input is an invalid move" << std::endl;
        exit(1);
    }
    
    port = fopen(inSocket, "w"); //clears file
    fclose(port);
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
