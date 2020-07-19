//
//  main.cpp
//  Chess
//
//  Created by Max Smiley on 5/8/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//


#include <iostream>
#include <vector>

#include "Auxiliary.h"
#include "Coord.h"
#include "Knight.h"
#include "Board.h"
#include "Game.h"
#include "King.h"
#include "Set.h"

using namespace std;
vector<vector<string>> b =
{
    {"BR","BN","BB","BQ","BK","BB","BN","BR"},
    {"BP","BP","BP","BP","BP","BP","BP","BP"},
    {"  ","  ","  ","  ","  ","  ","  ","  "},
    {"  ","  ","  ","  ","  ","  ","  ","  "},
    {"  ","  ","  ","  ","  ","  ","  ","  "},
    {"  ","  ","  ","  ","  ","  ","  ","  "},
    {"WP","WP","WP","WP","WP","WP","WP","WP"},
    {"WR","WN","WB","WQ","WK","WB","WN","WR"}
};

/*
vector<vector<string>> b =
{
    {"BR","  ","  ","  ","BK","BB","  ","BR"},
    {"BP","BP","BP","  ","  ","BP","BP","BP"},
    {"  ","  ","BN","  ","BP","  ","BB","  "},
    {"BQ","  ","  ","  ","  ","  ","WN","  "},
    {"  ","  ","  ","  ","  ","WB","  ","  "},
    {"  ","  ","  ","WP","  ","WQ","  ","  "},
    {"WP","WP","  ","  ","  ","WP","WP","WP"},
    {"WR","  ","  ","  ","WK","WB","  ","WR"}
};
*/

void pseudoLegalMovesBishop(Board* b)
{
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        if ((*itr)->type() == 'B')
        {
            (*itr)->pseudoLegalMoves().dump();
        }
    }
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        if ((*itr)->type() == 'B')
        {
            (*itr)->pseudoLegalMoves().dump();
        }
    }
}


int allLegalMoves(Board* b, char color)
{
    int i = 0;
    //char dir = 'G';
    
    if (color == 'W')
    {
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        cout << (*itr)->getColor() << (*itr)->type() << endl;
        for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
        {
            cout << (*itr2).getX() << " " << (*itr2).getY() <<  " ";
            /*Board* temp = new Board(*b);
            temp->movePiece(temp->getPiece((*itr)->getPos()), *itr2);
            //temp->moveRaw(temp->getPiece((*itr)->getPos()), set.get(i));
            temp->nextTurn();
            cout << eval(temp, temp->getTurn()) << endl;
          
            delete temp;*/
        }
        cout << endl;
        i += (*itr)->legalMoves.size();
    }
    }
    else if (color == 'B')
    {
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        cout << (*itr)->getColor() << (*itr)->type() << endl;
        for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
        {
            cout << (*itr2).getX() << " " << (*itr2).getY() <<  " ";
            /*Board* temp = new Board(*b);
            temp->movePiece(temp->getPiece((*itr)->getPos()), *itr2);
            //temp->moveRaw(temp->getPiece((*itr)->getPos()), set.get(i));
            temp->nextTurn();
            cout << eval(temp, temp->getTurn()) << endl;
            
            delete temp;*/
        }
        cout << endl;
        i += (*itr)->legalMoves.size();
    }
    }
    return i;
}

bool doGame = true;

int main() {
    
    Board* pos = new Board(b);
    Game g(pos);
    
    if (doGame)
    {
        g.play();
    }
    return 0;
}
