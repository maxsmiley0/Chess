//
//  board.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef board_h
#define board_h

class Board
{
public:
    Board();
    
    
private:
    //Initialization functions
    void InitBoard();   //initializes all values to NOPIECE
    int mBoard[8][8];
}

#endif /* board_h */
