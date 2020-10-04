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
    
    //Returns principal variation move after an alpha beta search
    int reccomendMove();
    
    //Accessors
    Board* getBoard() {return moveGenerator->getBoard(); } //returns a pointer to the board
    Movegen* getMoveGenerator() {return moveGenerator; }   //returns a pointer to the move generator member
    
private:
    //Main searching algorithms
    int alphaBeta(int alpha, int beta, int depth);      //returns alpha in centipawns
    int quiescenceSearch(int alpha, int beta);          //called at nodes of alphaBeta
    
    
    //Move Ordering
    std::list<int> orderedMoves(std::list<int> moves, int depth = -1);    //takes in an unordered list of legal moves, orders them. Unspecified depth means '-1' (for quiescence search, since unbounded by depth), in which case we won't use killer moves
    int movePriority(int move, int depth);     //returns the priority of a move, depth is needed for killer heuristic
    
    //Related to the principal variation heuristic
    void storePvMove(int move);     //stores pv move for a particular board
    int getPvMove();                //returns PV move for a given position
    void printPvLine(int depth);    //after recommending a move, print the pv line
    
    //Related to the killer heuristic
    void storeKillerMove(int move, int depth); //stores killer 1 move, pushes previous killer 1 to killer 2, discounts previous killer 2
    int getKiller1(int depth);      //returns most recent sibling move resulting in a beta cutoff
    int getKiller2(int depth);      //returns penultimate sibling move resulting in a beta cutoff
    
    //Related to the history heuristic
    void storeHistoryMove(int move, int score);     //stores a history move
    int getHistoryScore(int move);                  //gets history score
    
    //Auxiliary
    void prepSearch();              //prepares for search, clears tables, etc.
    void checkTime();               //updates "stop" bool if time alloc exceeded
    
    //Members
    Timer timer;                    //maintains time
    Stats stat;                     //statistics collection
    
    int searchDepth;                //depth at which we are searching
    int timeAllocated = 3000;       //allocated time to search in ms
    bool stop;                      //have we exceeded time alloc while hitting mindepth?
    int rootPosKey;                 //ensures no collisions occur in the PV table
    
    PVNode pvTable[TTABLEENTRIES];     //stores principal variation moves indexed by position key modulo TTABLEENTRIES
    int killerMoves[2 * MAXDEPTH + 1];     //stores killer moves, indexed by depth. For example, killer moves at depth 6 are stored in 6, 6 + MAXDEPTH
    int historyMoves[14 * 64];      //indexed by piece type, and to square
    
    Movegen* moveGenerator;         //Pointer to move generator
};

#endif /* searcher_hpp */
