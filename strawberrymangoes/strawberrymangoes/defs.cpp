//
//  defs.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "defs.h"
#include <random>
#include <utility>

int PceCol (int pce)
{
    if (pce >= 0 && pce <= 5)
    {
        return WHITE;
    }
    else if (pce >= 6 && pce <= 11)
    {
        return BLACK;
    }
    else
    {
        std::cerr << "Invalid piece color" << std::endl;
        exit(1);
    }
}

//Used for RAND32
static std::random_device rd;
static std::default_random_engine generator(rd());

int RAND32()
{
    std::uniform_int_distribution<> distro(0, INT_MAX);
    return distro(generator);
}

