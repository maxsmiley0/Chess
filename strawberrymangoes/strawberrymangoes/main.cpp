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

using namespace std;

int main()
{
    // insert code here...
    Timer t;
    t.start();
    
    Board b;
    
    b.parseFen("r3k2r/p1p1p2p/1nb2nq1/2qpPb2/NpPPBPp1/3Q4/PPP2BN1/R3K2R b KQkq d6 0 1");
    b.printBoard();
    
    Movegen m(b);
    
    m.generateMoves(0);
    m.printMoves(0);
    /*
    for (int i = 0; m.moves[i] != 0; i++)
    {
        cout << m.makeMove(m.moves[i]);
        m.getBoard().parseFen("rnbqk1nr/ppppp2p/8/4Pb2/2PPBPp1/2NQ1q2/PPP5/R3KBNR w KQkq - 0 1");
    }
    */
    std::cout << t.elapsed() << endl;
    return 0;
}
