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
    Timer t;
    //r2q3k/pn2bprp/4pNp1/2p1PbQ1/3p1P2/5NR1/PPP3PP/2B2RK1 b - -
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -
    Searcher s;
    s.getMoveGenerator()->getBoard()->parseFen("2R1B2k/n2q4/8/3RN3/Q2r4/8/p2P4/3b3K b - -");
    s.getMoveGenerator()->printMoves(s.orderedMoves(s.getMoveGenerator()->generateMoves()));
    //s.getMoveGenerator()->getBoard()->parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    //s.reccomendMove(6);
    return 0;
}
