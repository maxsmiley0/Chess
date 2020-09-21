//
//  defs.h
//  strawberrymangoes
//
//  Created by Max Smiley on 9/20/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef defs_h
#define defs_h

#define NOPIECE 0
#define WP 1
#define WN 2
#define WB 3
#define WR 4
#define WQ 5
#define WK 6
#define BP 7
#define BN 8
#define BB 9
#define BR 10
#define BQ 11
#define BK 12

#define WHITE 0
#define BLACK 1
#define NEITHER 2

//Returns color, indexed by piece type
const extern int PceColor[] =
{NEITHER,
WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};


#endif /* defs_h */
