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

int main()
{
    Board* pos = new Board(b); //deleted as part of game destructor
    Game g(pos);
    
    g.play();
}
