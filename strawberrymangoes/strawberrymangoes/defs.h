//
//  defs.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef defs_h
#define defs_h

#include <iostream>
#include <chrono>

#define OFFBOARD -1
#define WP 0
#define WN 1
#define WB 2
#define WR 3
#define WQ 4
#define WK 5
#define BP 6
#define BN 7
#define BB 8
#define BR 9
#define BQ 10
#define BK 11
#define NOPIECE 12

#define WKCA 8
#define WQCA 4
#define BKCA 2
#define BQCA 1

#define WHITE 0
#define BLACK 1
#define NEITHER 2

int PceCol (int pce);   //Returns color, given a piece
int RAND32 ();          //Returns a random 32 bit integer

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};

#endif /* defs_h */
