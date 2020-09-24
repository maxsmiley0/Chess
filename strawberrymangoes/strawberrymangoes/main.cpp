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
    Movegen m(b);
    
    m.getBoard().parseFen("7q/r5PK/8/3k4/8/8/8/8 w - - 0 1");
    
    m.getBoard().printBoard();
    m.generateMoves(0);
    m.printMoves(0);
    cout << m.getBoard().getPosKey() << endl;
    t.start();
    (m.makeMove(1093169)) ? (cout << "legal" << endl) : (cout << "illegal" << endl);
    cout << t.elapsed() << endl;
    
    m.getBoard().printBoard();
    cout << m.getBoard().getPosKey() << endl;
    
    
    return 0;
}
