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
    Timer t;
    
    Board b;
    Movegen m(b);
    
    m.getBoard().parseFen("r3k1r1/8/8/8/8/8/8/R3K2R b KQq -");
    m.perftTest(6);
    
    cout << "Time: " << t.elapsed() << endl;
    m.getBoard().printBoard();
    cout << m.getBoard().getPosKey();
    return 0;
}
