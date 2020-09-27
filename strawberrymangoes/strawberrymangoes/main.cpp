//
//  main.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include <iostream>
#include "defs.h"
#include "board.h"
#include "movegen.h"
#include "searcher.h"

using namespace std;

int main()
{
    Timer t;
    
    Searcher s;
    //r2q3k/pn2bprp/4pNp1/2p1PbQ1/3p1P2/5NR1/PPP3PP/2B2RK1 b - -
    //rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -
    s.getMoveGenerator()->getBoard()->parseFen("r2q3k/pn2bprp/4pNp1/2p1PbQ1/3p1P2/5NR1/PPP3PP/2B2RK1 w - -");
    //s.reccomendMove(6);
    s.getMoveGenerator()->printMoves(s.getMoveGenerator()->generateCaptures());
    /*
     110
     5 e2e4 3328294
     4 e7e6 3197089
     3 f1c4 3196207
     2 e7e5 3328225
     1 e5c6 3171491
     -110
     4 e7e6 3197089
     3 f1c4 3196207
     2 e7e5 3328225
     1 c4f7 3172948
     110
     3 f1c4 3196207
     2 e6e5 3197154
     1 h5h7 3174011
     -110
     2 e6e5 3197154
     1 c4f7 3172948
     110
     1 c4f7 3172948
     */
    //s.getMoveGenerator()->makeMove(3328294);
    //s.reccomendMove(4);
    //s.getMoveGenerator()->makeMove(3197089);
    //s.reccomendMove(3);
    //s.getMoveGenerator()->makeMove(3196207);
    //s.reccomendMove(2);
    //s.getMoveGenerator()->makeMove(3197154);
    //s.reccomendMove(1);
    //s.getMoveGenerator()->makeMove(3172948);
    //s.getMoveGenerator()->perftTest(6);
    //m.generateMoves(0);
    //m.printMoves(0);
    //m.makeMove(3158464);
    //cout << "material: " << m.getBoard().material << endl;
    s.getMoveGenerator()->getBoard()->printBoard();
    //cout << static_eval(s.getMoveGenerator()->getBoard()) << endl;
    //cout << m.getBoard().getPosKey();
    return 0;
}
