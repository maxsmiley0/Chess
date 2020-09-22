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
    
    b.parseFen("4RK2/4r3/1P5B/1b1pN2n/6p1/pB4P1/Q7/3q1k2 w - - 0 1");
    
    Movegen m(b);
    m.printAttacked();
    
    std::cout << t.elapsed() << endl;
    return 0;
}
