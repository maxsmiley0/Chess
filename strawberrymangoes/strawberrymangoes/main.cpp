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

using namespace std;

int main()
{
    // insert code here...
    Timer t;
    t.start();
    Board b;
    
    b.parseFen("rnbqkbnr/pp3ppp/3pp3/2p5/2B1P2P/5N2/PPPP1PP1/RNBQK2R b Kq h3 0 1");
    b.printBoard();
    
    std::cout << t.elapsed() << endl;
    return 0;
}
