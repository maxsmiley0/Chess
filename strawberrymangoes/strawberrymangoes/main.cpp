//
//  main.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include <iostream>
#include <list>
#include "defs.h"
#include "board.h"
#include "movegen.h"
#include "searcher.h"

using namespace std;

int main()
{
    //r2q3k/pn2bprp/4pNp1/2p1PbQ1/3p1P2/5NR1/PPP3PP/2B2RK1 b - -
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -
    Searcher s;
    s.getMoveGenerator()->getBoard()->parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");

    while (true)
    {
        s.getMoveGenerator()->getBoard()->printBoard();
        
        string move;
        cin >> move;
        
        //vet moves somewhere in here
        
        int f1 = 7 + (int)'1' - (int)move[1];
        int f2 = (int)move[0] - (int)'a';
        int f3 = 7 + (int)'1' - (int)move[3];
        int f4 = (int)move[2] - (int)'a';
        
        int pce = NOPIECE;
        s.getMoveGenerator()->makeMove(s.getMoveGenerator()->getMove(f1, f2, f3, f4, pce));
        s.getMoveGenerator()->makeMove(s.reccomendMove());
    }
    return 0;
}
