//
//  defs.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "defs.h"
#include "board.h"
#include <random>
#include <utility>

//Returns true if a piece is a pawn
bool isPawn(int pce)
{
    return (pce == WP || pce == BP);
}

//Returns true if a piece is a king
bool isKing(int pce)
{
    return (pce == WK || pce == BK);
}

//Returns true if a piece is a bishop or a queen
bool doDiag(int pce)
{
    return (pce == WB || pce == BB || pce == WQ || pce == BQ);
}

//Returns true if a piece is a rook or a queen
bool doVert(int pce)
{
    return (pce == WR || pce == BR || pce == WQ || pce == BQ);
}

//Returns color of a piece
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

//Extracts information from a move key
int fromR(int move)
{
    return (move & 0x7);
}

int fromC(int move)
{
    return ((move & 0x38) >> 3);
}

int toR(int move)
{
    return ((move & 0x1C0) >> 6);
}

int toC(int move)
{
    return ((move & 0xE00) >> 9);
}

int captured(int move)
{
    return ((move & 0xF000) >> 12);
}

int promoted(int move)
{
    return ((move & 0x3C0000) >> 18);
}

bool isEnpasMove(int move)
{
    return ((move & 0x10000) >> 16);
}

bool isPawnstartMove(int move)
{
    return ((move & 0x20000) >> 17);
}
bool isCastleMove(int move)
{
    return ((move & 0x400000) >> 22);
}

//Converts move key into readable form
std::string printMove(int move)
{
    std::string str = "";
    str += FileChar[fromC(move)];
    str += RankChar[fromR(move)];
    str += FileChar[toC(move)];
    str += RankChar[toR(move)];
    
    return str;
}

//Returns worth of a piece
int worth(int pce)
{
    switch (pce)
    {
        case WP:
            return 100;
        case WN:
            return 325;
        case WB:
            return 325;
        case WR:
            return 550;
        case WQ:
            return 1000;
        case WK:
            return 50000;
        case BP:
            return -100;
        case BN:
            return -325;
        case BB:
            return -325;
        case BR:
            return -550;
        case BQ:
            return -1000;
        case BK:
            return -50000;
        default:
            std::cerr << "Trying to find worth of an undefined pce" << std::endl;
            exit(1);
    }
}

/*
 Piece Square Tables
 */
const int PawnTable[8][8] =
{
    {0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0    },
    {10    ,    10    ,    0    ,    -10    ,    -10    ,    0    ,    10    ,    10    },
    {5    ,    0    ,    0    ,    5    ,    5    ,    0    ,    0    ,    5    },
    {0    ,    0    ,    10    ,    20    ,    20    ,    10    ,    0    ,    0    },
    {5    ,    5    ,    5    ,    10    ,    10    ,    5    ,    5    ,    5    },
    {10    ,    10    ,    10    ,    20    ,    20    ,    10    ,    10    ,    10    },
    {20    ,    20    ,    20    ,    30    ,    30    ,    20    ,    20    ,    20   } ,
    {0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0}
};


const int KnightTable[8][8] =
{
    {0    ,    -10    ,    0    ,    0    ,    0    ,    0    ,    -10    ,    0    },
    {0    ,    0    ,    0    ,    5    ,    5    ,    0    ,    0    ,    0    },
    {0    ,    0    ,    10    ,    10    ,    10    ,    10    ,    0    ,    0    },
    {0    ,    0    ,    10    ,    20    ,    20    ,    10    ,    5    ,    0    },
    {5    ,    10    ,    15    ,    20    ,    20    ,    15    ,    10    ,    5    },
    {5    ,    10    ,    10    ,    20    ,    20    ,    10    ,    10    ,    5    },
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0    },
    {0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0}
};

const int BishopTable[8][8] =
{
    {0    ,    0    ,    -10    ,    0    ,    0    ,    -10    ,    0    ,    0    },
    {0    ,    0    ,    0    ,    10    ,    10    ,    0    ,    0    ,    0    },
    {0    ,    0    ,    10    ,    15    ,    15    ,    10    ,    0    ,    0    },
    {0    ,    10    ,    15    ,    20    ,    20    ,    15    ,    10    ,    0   } ,
    {0    ,    10    ,    15    ,    20    ,    20    ,    15    ,    10    ,    0   } ,
    {0    ,    0    ,    10    ,    15    ,    15    ,    10    ,    0    ,    0    },
    {0    ,    0    ,    0    ,    10    ,    10    ,    0    ,    0    ,    0   } ,
    {0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0    ,    0}
};

const int RookTable[8][8] =
{
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0    },
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0   } ,
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0    },
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0    },
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0    },
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0    },
    {25    ,    25    ,    25    ,    25    ,    25    ,    25    ,    25    ,    25    },
    {0    ,    0    ,    5    ,    10    ,    10    ,    5    ,    0    ,    0}
};

const int KingTable[8][8] =
{
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-30,-40,-40,-50,-50,-40,-40,-30},
    {-20,-30,-30,-40,-40,-30,-30,-20},
    {-10,-20,-20,-20,-20,-20,-20,-10},
    {20, 20,  0,  0,  0,  0, 20, 20},
    {20, 30, 10,  0,  0, 10, 30, 20}
};

const double KingTableEg[8][8] =
{
    {-50,-40,-30,-20,-20,-30,-40,-50},
    {-30,-20,-10,  0,  0,-10,-20,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 30, 40, 40, 30,-10,-30},
    {-30,-10, 20, 30, 30, 20,-10,-30},
    {-30,-30,  0,  0,  0,  0,-30,-30},
    {-50,-30,-30,-30,-30,-30,-30,-50}
};

//Static evaluation, to be called at the leaf nodes
int static_eval(Board* b)
{
    int score = b->getMaterial();
    
    bool inEg;          //are we in the endgame or not?
    int pceCount = 0;   //number of non pawn / king pieces
    
    pceCount += b->getPceNum(WN);
    pceCount += b->getPceNum(WB);
    pceCount += b->getPceNum(WR);
    pceCount += b->getPceNum(WQ);
    
    pceCount += b->getPceNum(BN);
    pceCount += b->getPceNum(BB);
    pceCount += b->getPceNum(BR);
    pceCount += b->getPceNum(BQ);
    
    inEg = (pceCount <= 6);
    
    //Looping through all white pawns
    for(int i = 0; i < b->getPceNum(WP); i++)
    {
        score += PawnTable[7 - b->getPceR(WP, i)][b->getPceC(WP, i)];
    }
    //Looping through all white knights
    for(int i = 0; i < b->getPceNum(WN); i++)
    {
        score += KnightTable[7 - b->getPceR(WN, i)][b->getPceC(WN, i)];
    }
    //Looping through all white bishops
    for(int i = 0; i < b->getPceNum(WB); i++)
    {
        score += BishopTable[7 - b->getPceR(WB, i)][b->getPceC(WB, i)];
    }
    //Looping through all white rooks
    for(int i = 0; i < b->getPceNum(WR); i++)
    {
        score += RookTable[7 - b->getPceR(WR, i)][b->getPceC(WR, i)];
    }
    //Looping through all white queens, use a rook table
    for(int i = 0; i < b->getPceNum(WQ); i++)
    {
        score += RookTable[7 - b->getPceR(WQ, i)][b->getPceC(WQ, i)];
    }
    //Use different PSTs depending on the game stage
    if (inEg)
    {
        score += KingTableEg[b->getKingR(WHITE)][b->getKingC(WHITE)];
    }
    else
    {
        score += KingTable[b->getKingR(WHITE)][b->getKingC(WHITE)];
    }
    //Looping through all black pawns
    for(int i = 0; i < b->getPceNum(BP); i++)
    {
        score -= PawnTable[b->getPceR(BP, i)][b->getPceC(BP, i)];
    }
    //Looping through all black knights
    for(int i = 0; i < b->getPceNum(BN); i++)
    {
        score -= KnightTable[b->getPceR(BN, i)][b->getPceC(BN, i)];
    }
    //Looping through all black bishops
    for(int i = 0; i < b->getPceNum(BB); i++)
    {
        score -= BishopTable[b->getPceR(BB, i)][b->getPceC(BB, i)];
    }
    //Looping through all black rooks
    for(int i = 0; i < b->getPceNum(BR); i++)
    {
        score -= RookTable[b->getPceR(BR, i)][b->getPceC(BR, i)];
    }
    //Looping through all black queens
    for(int i = 0; i < b->getPceNum(BQ); i++)
    {
        score -= RookTable[b->getPceR(BQ, i)][b->getPceC(BQ, i)];
    }
    //Use different PSTs depending on the game stage
    if (inEg)
    {
        score -= KingTableEg[7 - b->getKingR(BLACK)][b->getKingC(BLACK)];
    }
    else
    {
        score -= KingTable[7 - b->getKingR(BLACK)][b->getKingC(BLACK)];
    }
    
    return (b->getSide() == WHITE) ? (score) : (-score);
}

//Used for RAND32
static std::random_device rd;
static std::default_random_engine generator(rd());
//Returns random 32 bit number
int RAND32()
{
    std::uniform_int_distribution<> distro(0, INT_MAX);
    return distro(generator);
}

