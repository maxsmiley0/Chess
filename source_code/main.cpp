//
//  main.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "game.h"

int main()
{
    Game g1("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", WHITE);
    g1.play();
    Game g2("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -", BLACK);
    g2.play();
    Game g3("rnbqkbnr/8/8/pPpPpPpP/PpPpPpPp/8/8/RNBQKBNR w KQkq -", WHITE);
    g3.play();
    Game g4("rnbqkbnr/8/8/pPpPpPpP/PpPpPpPp/8/8/RNBQKBNR w KQkq -", BLACK);
    g4.play();
    
    return 0;
}
