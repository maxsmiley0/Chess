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
    Movegen m(b);
    
    m.getBoard().parseFen("r2qkb1r/ppp2ppp/2np1n2/1B2p3/4P1b1/3P1N2/PPPQ1PPP/RNB1K2R b KQkq -");
    cout << m.getBoard().getPosKey() << endl;
    
    m.generateMoves(0);
    m.printMoves(0);
    (m.makeMove(3196532)) ? (cout << "legal" << endl) : (cout << "illegal" << endl);
    //m.makeMove(3182599);
    m.getBoard().printBoard();
    
    //cout << "Poskey is now: " << m.getBoard().getPosKey() << endl;
    //m.getBoard().parseFen("R2qkbnr/1ppppppp/1nb5/3p4/4P2P/1P5R/1PPP1PP1/1NBQKBN1 b k - 0 1");
    //cout << m.getBoard().getPosKey() << endl;
    
    //std::cout << t.elapsed() << endl;
    return 0;
}
