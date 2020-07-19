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
#include "Game.h"
#include "Coord.h"
#include "Piece.h"
#include "Set.h"
#include "Tuple.h"

int allLegalMoves(Board* b, char color);
void pseudoLegalMovesBishop(Board* b);

using namespace std;

struct TuplePC;

Game::Game(Board* b)
{
    mBoard = b;
}

Game::~Game()
{
    delete mBoard;
    
    cout << endl << endl;
    cout << "Tuple regression (moves, alg time)" << endl;
    /*
    for (list<Tuple>::iterator itr = data.begin(); itr != data.end(); itr++)
    {
        cout << "(" << (*itr).value << "," << (*itr).time << ")" << endl;
    }*/
    for (list<Tuple>::iterator itr = data.begin(); itr != data.end(); itr++)
    {
        cout << "," << (*itr).time << endl;
    }
}

Board* Game::getBoard() const
{
    return mBoard;
}

//loops forever for now for testing purposes
void Game::play()
{
    while (playerColor != 'W' && playerColor != 'B')
    {
        cout << "Which side would you like to play? W/B" << endl;
        cin >> playerColor;
    }
    
    while (!gameOver())
    {
        Timer hashTimer;
        hashTimer.start();
        clearHash(mBoard->getTurn());
        cout << "Hash timer: " << hashTimer.elapsed() << endl;
        //displays board at beginning of turn
        mBoard->print();
        Timer t1;
        cout << mBoard->hashmap() << endl;
        cout << t1.elapsed() << endl;
        /*
         rudimentary way of getting beginning and end coord
         */
        if (mBoard->getTurn() == playerColor)
        {
            string userInput;
            Coord c1, c2;
            
            while (true)
            {
                cout << "Enter a move for " << getBoard()->getTurn() << endl;
                cin >> userInput;
                
                if (userInput == "exit")
                {
                    //backdoor ending game
                    break;
                }
                if (!isValid(userInput))
                {
                    c1 = convert(userInput[0], userInput[1]);
                    c2 = convert(userInput[2], userInput[3]);
                }
                if (!isValid(userInput) && mBoard->getPiece(c1) != nullptr && containsCoord(mBoard->getPiece(c1)->legalMoves, c2) && mBoard->getPiece(c1)->getColor() == mBoard->getTurn())
                {
                    break;
                }
            }
            
            /*
             In order for a move to be legal, it must be a "legal move" in that it can be moved there by a piece (not obstructed, etc)
             It must also lead to a "legal state"
             legalMove is absorbed in movePiece, maybe change?
             */
            if (mBoard->getPiece(c1) != nullptr && mBoard->getPiece(c1)->getColor() == getBoard()->getTurn())
            {
                mBoard->movePiece(mBoard->getPiece(c1), c2);
                mBoard->nextTurn();
            }
            else
            {
                cerr << "Bad input" << endl;
                break;
            }
        }
        else
        {
            Tuple t;
            Timer t1;
            t1.start();
            
            TuplePC tuple(reccomendMove(mBoard, mBoard->getTurn(), 4, -999, 999));
            mBoard->movePiece(tuple.p, tuple.c);
            mBoard->nextTurn();
            
            t.time = t1.elapsed();
            data.push_back(t);
        } 
    }
    
    mBoard->print();
    
    if (!getBoard()->getKing(playerColor)->attackers.empty())
    {
        cout << "You lose :(" << endl;
    }
    else
    {
        cout << "You win!" << endl;
    }
    
}

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

bool Game::gameOver()
{
    bool gameOverForWhite = true;
    bool gameOverForBlack = true;
    
    for (list<Piece*>::iterator itr = mBoard->whitePieces.begin(); itr != mBoard->whitePieces.end(); itr++)
    {
        if (!(*itr)->legalMoves.empty())
        {
            gameOverForWhite = false;
            break;
        }
    }
    
    for (list<Piece*>::iterator itr = mBoard->blackPieces.begin(); itr != mBoard->blackPieces.end(); itr++)
    {
        if (!(*itr)->legalMoves.empty())
        {
            gameOverForBlack = false;
            break;
        }
    }
    
    return (gameOverForWhite || gameOverForBlack);
}

bool Game::isValid(string s)
{
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


/*
for (list<Piece*>::iterator itr = mBoard->whitePieces.begin(); itr != mBoard->whitePieces.end(); itr++)
{
    for (list<Piece*>::iterator itr2 = (*itr)->attackers.begin(); itr2 != (*itr)->attackers.end(); itr2++)
    {
        cout << (*itr)->getColor() << (*itr)->type() << " at " << (*itr)->getPos().getX() << " " << (*itr)->getPos().getY() << " attacked by " << (*itr2)->getColor() << (*itr2)->type() << " at " << (*itr2)->getPos().getX() << " " << (*itr)->getPos().getY() << endl;
    }
    for (list<Piece*>::iterator itr2 = (*itr)->defenders.begin(); itr2 != (*itr)->defenders.end(); itr2++)
    {
        cout << (*itr)->getColor() << (*itr)->type() << " at " << (*itr)->getPos().getX() << " " << (*itr)->getPos().getY() << " defended by " << (*itr2)->getColor() << (*itr2)->type() << " at " << (*itr2)->getPos().getX() << " " << (*itr)->getPos().getY() << endl;
    }
}

for (list<Piece*>::iterator itr = mBoard->blackPieces.begin(); itr != mBoard->blackPieces.end(); itr++)
{
    for (list<Piece*>::iterator itr2 = (*itr)->attackers.begin(); itr2 != (*itr)->attackers.end(); itr2++)
    {
        cout << (*itr)->getColor() << (*itr)->type() << " at " << (*itr)->getPos().getX() << " " << (*itr)->getPos().getY() << " attacked by " << (*itr2)->getColor() << (*itr2)->type() << " at " << (*itr2)->getPos().getX() << " " << (*itr)->getPos().getY() << endl;
    }
    for (list<Piece*>::iterator itr2 = (*itr)->defenders.begin(); itr2 != (*itr)->defenders.end(); itr2++)
    {
        cout << (*itr)->getColor() << (*itr)->type() << " at " << (*itr)->getPos().getX() << " " << (*itr)->getPos().getY() << " defended by " << (*itr2)->getColor() << (*itr2)->type() << " at " << (*itr2)->getPos().getX() << " " << (*itr)->getPos().getY() << endl;
    }
}*/
