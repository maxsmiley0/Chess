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
    
    b.parseFen("rnbqkbnr/ppppp1Pp/5p2/8/4B3/2NQ4/PPPPPP1P/R3KBNR w KQkq - 0 1");
    
    Movegen m(b);/*
    m.printAttacked();
    b.printBoard();
    int move = m.getMove(1, 6, 0, 5, WQ);
    
    cout << move << endl;
    cout << fromR(move) << endl;
    cout << fromC(move) << endl;
    cout << toR(move) << endl;
    cout << toC(move) << endl;
    cout << captured(move) << endl;
    cout << promoted(move) << endl;
    
    */
    m.generateMoves(0);
    std::cout << t.elapsed() << endl;
    return 0;
}
