//
//  Coord.cpp
//  Chess
//
//  Created by Max Smiley on 6/26/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "Coord.h"
#include <iostream>
using namespace std;

Coord::Coord()
{
    
}

Coord::Coord(int xPos, int yPos)
{
    x = xPos;
    y = yPos;
}

Coord::Coord(const Coord& other)
{
    x = other.x;
    y = other.y;
}

Coord Coord::operator+(const Coord& other)
{
    Coord tempSet(other);
    tempSet.x += x;
    tempSet.y += y;
    return tempSet;
}

Coord Coord::operator-(const Coord& other)
{
    Coord tempSet(other);
    tempSet.x -= x;
    tempSet.y -= y;
    return tempSet;
}

Coord& Coord::operator=(const Coord& other)
{
    if (this != &other)
    {
        Coord tempSet(other);
        swap(tempSet);
    }
    return *this;
}

bool Coord::operator==(const Coord& other)
{
    if (other.getX() == x && other.getY() == y)
    {
        return true;
    }
    return false;
}

int Coord::getX() const
{
    return x;
    
}

int Coord::getY() const
{
    return y;
    
}

void Coord::swap(Coord& other)
{
    std::swap(x, other.x);
    std::swap(y, other.y);
}
