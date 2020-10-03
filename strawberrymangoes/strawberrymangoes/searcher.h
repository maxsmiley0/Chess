//
//  searcher.hpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/25/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef searcher_hpp
#define searcher_hpp

#include "movegen.h"

class Searcher
{
public:
    Searcher();
    ~Searcher();
    
    int alphaBeta(int alpha, int beta, int depth);      //returns alpha in centipawns
    int quiescenceSearch(int alpha, int beta);          //called at nodes of alphaBeta
    int reccomendMove();            //computer plays the move recommended by alphabeta
    Movegen* getMoveGenerator();    //returns a pointer to the move generator member
    
    std::list<int> orderedMoves(std::list<int> moves, int depth = -1);    //takes in an unordered list of legal moves, orders them. Unspecified depth means '-1' (for quiescence search, since unbounded by depth), in which case we won't use killer moves
    int movePriority(int move, int depth);     //returns the priority of a move, depth is needed for killer heuristic
    
    void printPvLine(int depth);    //after recommending a move, print the pv line
    void storePvMove(int move);     //stores pv move for a particular board
    int getPvMove();                //returns PV move for a given position
    
    void prepSearch();              //prepares for search, clears tables, etc.
    
    void storeKillerMove(int move, int depth); //
    int getKiller1(int depth);
    int getKiller2(int depth);
    
    void storeHistoryMove(int move, int score);
    int getHistoryScore(int move);
    
    void checkTime();
    
private:
    Movegen* moveGenerator;
    PVNode pvTable[TTABLEENTRIES];     //stores principal variation moves indexed by position key modulo TTABLEENTRIES
    int killerMoves[2 * MAXDEPTH + 1];     //stores killer moves, indexed by depth. For example, killer moves at depth 6 are stored in 6, 6 + MAXDEPTH
    int historyMoves[14 * 64];      //indexed by piece type, and to square

    Timer timer;
    int timeAllocated = 3000;       //allocated time to search in ms
    int searchDepth = 0;           //depth at which we are searching
    bool stop = false;
    int minDepth = 5;              //minimum depth we search to, regardless of time allocated
    
    int rootPosKey;                //ensures no collisions occur in the PV table
    
    Stats stat;
};

#endif /* searcher_hpp */
