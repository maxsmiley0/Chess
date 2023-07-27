//
//  main.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "game.h"
#include "nnue/nnue.h"

int main()
{
    nnue_init("nnue/nn-04cf2b4ed1da.nnue");

    Game g;
    g.play();
}