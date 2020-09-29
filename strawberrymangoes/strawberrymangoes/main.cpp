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
    s.getMoveGenerator()->getBoard()->parseFen("r2q3k/pn2bprp/4pNp1/2p1PbQ1/3p1P2/5NR1/PPP3PP/2B2RK1 b - -");
     //s.getMoveGenerator()->getBoard()->parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -");
    s.reccomendMove(5);
    return 0;
}
