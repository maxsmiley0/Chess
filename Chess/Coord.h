#ifndef Coord_h
#define Coord_h

#include <iostream>

class Coord
{
public:
    Coord();
    Coord(int xPos, int yPos);
    Coord(const Coord& other);
    Coord operator+(const Coord& other);
    Coord operator-(const Coord& other);
    Coord& operator=(const Coord& other);
    bool operator==(const Coord& other);
    
    int getX() const;
    int getY() const;
    
    void swap(Coord& other);
    
private:
    int x, y;};

#endif /* Coord_h */
