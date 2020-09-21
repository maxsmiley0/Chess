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
    
    cout << b.getPosKey() << endl;
    b.addPiece(2, 3, WB);
    b.addPiece(7, 3, BK);
    
    cout << b.getPosKey() << endl;
    
    b.addPiece(1, 2, WP);
    b.removePiece(7, 3);
    
    cout << b.getPosKey() << endl;
    
    b.removePiece(1, 2);
    b.addPiece(7, 3, BK);
    
    cout << b.getPosKey() << endl;
    
    b.removePiece(7, 3);
    b.addPiece(7, 3, WK);
    
    cout << b.getPosKey() << endl;
    
    std::cout << t.elapsed() << endl;
    return 0;
}
