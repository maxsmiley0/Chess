//
//  Game.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 10/4/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "game.h"

Game::Game(std::string initFen, int pColor)
{
    mSearch = new Searcher();
    gameOver = false;
    
    startFen = initFen;
    if (pColor != WHITE && pColor != BLACK && pColor != NEITHER)
    {
        std::cerr << "Invalid color in game initializer" << std::endl;
        exit(1);
    }
    else
    {
        playerColor = pColor;
    }
}

Game::~Game()
{
    delete mSearch;
}

void Game::play()
{
    getBoard()->parseFen(startFen);     //Resetting board to starting position
    if (playerColor == NEITHER)
    {
        chooseSide();                       //Prompts the user to input a side
    }
    runGame();                          //Loop while game is not over
    printResult();                      //Prints if white/black won/drew and the method
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
                if (ponderMode)
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
                processComputerMove(getMoveGenerator());
                
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
                writeMove(move);
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

void Game::processComputerMove(Movegen* mGen)
{
    std::string move = "";
    int cnt = 0;
    std::ifstream file;
    file.open(inSocket);
    
    //Does the file exist?
    if (file.fail())
    {
        std::cerr << "Socket '" << inSocket << "' failed to open" << std::endl;
        exit(1);
    }
    
    file.close();
    
    //Loop until the other computer has made a move
    while (move == "")
    {
        //Since syscalls are taxing, only check every 0xFFFFF iterations
        if ((cnt & 0xFFFFF) == 0)
        {
            //Only read if not empty
            file.open(inSocket);
            if (file.peek() != std::ifstream::traits_type::eof())
            {
                file >> move;
            }
            file.close();
        }
        
        cnt++;
    }
    
    //Ensuring valid move from other computer
    if (!vetMove(mGen, std::stoi(move)))
    {
        std::cerr << "Computer input is an invalid move" << std::endl;
        exit(1);
    }
    
    clearSocket(inSocket);
}

void Game::writeMove(int move)
{
    std::ofstream file;
    file.open(outSocket);
    //Error checking pointless as it will simply create the file
    file << move;
    
    file.close();
}

void Game::clearSocket(std::string socket)
{
    std::ofstream file;
    file.open(socket, std::ofstream::out | std::ofstream::trunc);
    
    //Does the file exist?
    if (file.fail())
    {
        std::cerr << "Socket '" << socket << "' failed to open" << std::endl;
        exit(1);
    }
    
    file.close();
}

void Game::checkGameStatus()
{
    //If no legal moves, or 3 fold repetition is true
    
    if (legalMoves(getMoveGenerator()->generateMoves()) <= 0 || getBoard()->numRep() >= 2 || (getBoard()->getHisPly() - getBoard()->getFiftyMove()) >= 100)
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

void Game::printResult()
{
    //Output who won
    int side = getBoard()->getSide();
    int kingR = getBoard()->getKingR(side);
    int kingC = getBoard()->getKingC(side);
    
    bool inCheck = getMoveGenerator()->squareAttacked(kingR, kingC, side);
    //If no legal moves and in check
    if (inCheck && getBoard()->numRep() < 2 && (getBoard()->getHisPly() - getBoard()->getFiftyMove()) < 100)
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
        else if ((getBoard()->getHisPly() - getBoard()->getFiftyMove()) < 100)
        {
            std::cout << "Game Drawn: 50 move violation" << std::endl;
        }
        else
        {
            std::cout << "Game Drawn: stalemate" << std::endl;
        }
    }
}
