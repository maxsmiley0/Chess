//
//  searcher.hpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/25/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#ifndef searcher_h
#define searcher_h

#include "movegen.h"

class Searcher
{
public:
    Searcher();
    ~Searcher();
    
    //Returns principal variation move after an alpha beta search
    int reccomendMove();
    //Passively calculates (used when not player's turn)
    void ponder();
    //Auxiliary
    void prepSearch();
    //Sends signal to stop searching
    void sendSIGSTOP() { if (stop) {std::cerr << "SIGSTOP err" << std::endl; exit(1);} stop = true;}
    //Returns PV move for a given position
    int getPvMove() const;
    //Returns PV score for a given position
    int getPvScore() const;
    
    //Accessors
    Board* getBoard() const {return moveGenerator->getBoard(); } //returns a pointer to the board
    Movegen* getMoveGenerator() const {return moveGenerator; }   //returns a pointer to the move generator member
    
private:
    //Main searching algorithms
    int alphaBeta(int alpha, int beta, int depth);      //returns alpha in centipawns
    int quiescenceSearch(int alpha, int beta);          //called at nodes of alphaBeta
    bool multicut(int alpha, int beta, int depth, int r, int c, int m); //returns true if m moves have produced c cutoffs at a d reduced depth search
    
    //Move Ordering
    int pickNextMove(std::list<int>& li, int depth = -1);     //returns the next best move, deletes it from the list. Need depth for killer moves. Unspecified depth means '-1' (for quiescence search, since unbounded by depth), in which case we won't use killer moves
    int movePriority(int move, int depth) const;     //returns the priority of a move, depth is needed for killer heuristic
    
    //Related to the principal variation heuristic
    void storePvMove(int move, int score);     //stores pv move for a particular board
    void printPvLine(int depth);    //after recommending a move, print the pv line
    
    //Related to the killer heuristic
    void storeKillerMove(int move, int depth); //stores killer 1 move, pushes previous killer 1 to killer 2, discounts previous killer 2
    int getKiller1(int depth) const;      //returns most recent sibling move resulting in a beta cutoff
    int getKiller2(int depth) const;      //returns penultimate sibling move resulting in a beta cutoff
    
    //Related to the history heuristic
    void storeHistoryMove(int move, int score);     //stores a history move
    int getHistoryScore(int move) const;            //gets history score
    
    //Related to the transposition table
    void storeTransPos(int score, int depth);  //stores the score for a transposition at a given depth
    int getTransScore(int depth);    //returns score associated with a transposition, infinity if otherwise
    
    //Auxiliary
    void checkTime();               //updates "stop" bool if time alloc exceeded
    
    //Members
    Timer timer;                    //maintains time
    Stats stat;                     //statistics collection
    
    int searchDepth;                //depth at which we are searching
    int timeAllocated = 3000;       //allocated time to search in ms
    bool stop;                      //have we exceeded time alloc while hitting mindepth?
    int rootPosKey;                 //ensures no collisions occur in the PV table
    
    PVNode pvTable[TTABLEENTRIES];     //stores principal variation moves indexed by position key modulo TTABLEENTRIES
    Trans tTable[TTABLEENTRIES];       //transposition table
    int killerMoves[2 * MAXDEPTH + 1];     //stores killer moves, indexed by depth. For example, killer moves at depth 6 are stored in 6, 6 + MAXDEPTH
    int historyMoves[14 * 64];      //indexed by piece type, and to square
    
    Movegen* moveGenerator;         //Pointer to move generator
};

#endif /* searcher_h */
