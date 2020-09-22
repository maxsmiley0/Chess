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
    
    Board b;
    
    b.parseFen("rnbqk1nr/ppppp2p/8/4Pb2/2PPBPp1/2NQ4/PPP4P/R3KBNR w KQkq - 0 1");
    
    Movegen m(b);
    t.start();
    m.generateMoves(0);
    m.printMoves(0);
    
    b.printBoard();
    /*
    int move = m.getMove(1, 6, 0, 5, WQ);
    
    cout << move << endl;
    cout << fromR(move) << endl;
    cout << fromC(move) << endl;
    cout << toR(move) << endl;
    cout << toC(move) << endl;
    cout << captured(move) << endl;
    cout << promoted(move) << endl;
    */
    std::cout << t.elapsed() << endl;
    return 0;
}
