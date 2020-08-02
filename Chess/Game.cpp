//
//  Game.cpp
//  Chess
//
//  Created by Max Smiley on 5/9/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include <string>
#include <iostream>

#include "Auxiliary.h"
#include "Board.h"
#include "Coord.h"
#include "Game.h"
#include "Piece.h"
#include "Set.h"

using namespace std;

Game::Game(Board* b)
{
    mBoard = b;
}

Game::~Game()
{
    delete mBoard;
    
    cout << endl << endl;
    /*
    for (list<Tuple>::iterator itr = data.begin(); itr != data.end(); itr++)
    {
        cout << "(" << (*itr).value << "," << (*itr).time << ")" << endl;
    }*/
    for (list<Tuple>::iterator itr = data.begin(); itr != data.end(); itr++)
    {
        //Displaying time from data
        cout << "," << (*itr).time << endl;
    }
    for (list<string>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        //Displaying moves at the end
        cout << (*itr) << " ";
    }
}

bool Game::isValid(string s)
{
    //To be valid: A string must have length 4. The first and third characters are letters a-h, the second and fourth characters are numbers 1-8
    if ((s.size() != 4))
    {
        return false;
    }
    else if (s[0] != 'a' && s[0] != 'b' && s[0] != 'c' && s[0] != 'd' && s[0] != 'e' && s[0] != 'f' && s[0] != 'g' && s[0] != 'h')
    {
        return false;
    }
    else if (s[1] != 0 && s[1] != 1 && s[1] != 2 && s[1] != 3 && s[1] != 4 && s[1] != 5 && s[1] != 6 && s[1] != 7 && s[1] != 8)
    {
        return false;
    }
    else if (s[2] != 'a' && s[2] != 'b' && s[2] != 'c' && s[2] != 'd' && s[2] != 'e' && s[2] != 'f' && s[2] != 'g' && s[2] != 'h')
    {
        return false;
    }
    else if (s[3] != 1 && s[3] != 2 && s[3] != 3 && s[3] != 4 && s[3] != 5 && s[3] != 6 && s[3] != 7 && s[3] != 8)
    {
        return false;
    }
    
    return true;
}

//Maps userInput to what the computer can understand
//E.g. e2e4 -> 6444
Coord Game::convert(char c1, char c2)
{
    int x, y;
    switch (c1)
    {
        case 'a':
            y = 0;
            break;
        case 'b':
            y = 1;
            break;
        case 'c':
            y = 2;
            break;
        case 'd':
            y = 3;
            break;
        case 'e':
            y = 4;
            break;
        case 'f':
            y = 5;
            break;
        case 'g':
            y = 6;
            break;
        case 'h':
            y = 7;
            break;
        default:
            y = -1;
            break;
    }
    switch (c2)
    {
        case '1':
            x = 7;
            break;
        case '2':
            x = 6;
            break;
        case '3':
            x = 5;
            break;
        case '4':
            x = 4;
            break;
        case '5':
            x = 3;
            break;
        case '6':
            x = 2;
            break;
        case '7':
            x = 1;
            break;
        case '8':
            x = 0;
            break;
        default:
            x = -1;
            break;
    }
    return Coord(x, y);
}

void Game::play()
{
    while (playerColor != 'W' && playerColor != 'B')
    {
        //Loops until player enters 'W' or 'B'
        cout << "Which side would you like to play? W/B" << endl;
        cin >> playerColor;
    }
    
    while (!gameOver())
    {
        
        mBoard->print();                      //prints out board
        
        if (mBoard->getTurn() == playerColor) //if turn is player turn
        {
            string userInput;
            Coord c1, c2;
            
            //Here, we loop until the player has entered a syntactically correct, legal move
            while (true)
            {
                cout << "Enter a move for " << mBoard->getTurn() << endl;
                cin >> userInput;
                
                if (userInput == "exit")
                {
                    //backdoor method of ending the game
                    break;
                }
                if (!isValid(userInput))
                {
                    c1 = convert(userInput[0], userInput[1]);
                    c2 = convert(userInput[2], userInput[3]);
                }
                if (!isValid(userInput) && mBoard->getPiece(c1) != nullptr && containsCoord(mBoard->getPiece(c1)->legalMoves, c2) && mBoard->getPiece(c1)->getColor() == mBoard->getTurn())
                {
                    moves.push_back(userInput);
                    break;
                }
            }
            if (userInput == "exit")
            {
                //backdoor method of ending the game
                break;
            }
            //At this point, the input is guaranteed to be syntactically correct
            mBoard->movePiece(mBoard->getPiece(c1), c2);    //move piece
            mBoard->nextTurn();                             //execute next turn
        }
        else                                  //if computer turn
        {
            Tuple t;
            Timer t1;  //timing how long it takes the computer to find a move
            t1.start();
            
            //Returning a tuple of type struct with pointer to piece and coord
            TuplePC tuple(reccomendMove(mBoard, mBoard->getTurn(), 3, -999, 999));
            mBoard->movePiece(tuple.p, tuple.c);
            mBoard->nextTurn();
            
            t.time = t1.elapsed();
            data.push_back(t);
        }
    }
    
    //Once the game has ended, display the final position
    mBoard->print();
    
    if (!mBoard->getKing('W')->attackers.empty() && !mBoard->getKing('B')->attackers.empty())
    {
        cout << "Stalemate!" << endl;
    }
    else if (!mBoard->getKing(playerColor)->attackers.empty())
    {
        cout << "You lose :(" << endl;
    }
    else
    {
        cout << "You win!" << endl;
    }
}

bool Game::gameOver()
{
    //gameOver is initially true, and if any legal moves exist, then we know the game is not actually over
    bool gameOver = true;
    
    if (mBoard->getTurn() == 'W')
    {
        for (list<Piece*>::iterator itr = mBoard->whitePieces.begin(); itr != mBoard->whitePieces.end(); itr++)
        {
            if (!(*itr)->legalMoves.empty())
            {
                gameOver = false;
                break;
            }
        }
    }
    else
    {
        for (list<Piece*>::iterator itr = mBoard->blackPieces.begin(); itr != mBoard->blackPieces.end(); itr++)
        {
            if (!(*itr)->legalMoves.empty())
            {
                gameOver = false;
                break;
            }
        }
    }
    
    return gameOver;
}
