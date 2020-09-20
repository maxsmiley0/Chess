//
//  Auxiliary.cpp
//  Chess
//
//  Created by Max Smiley on 6/30/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include <iostream>
#include <cmath>

#include "Auxiliary.h"
#include "Board.h"
#include "Piece.h"
#include "Coord.h"
#include "Set.h"
#include "PST.h"

using namespace std;

list<TupleHASH> tTable[HASHCOUNT];
Zobrist zob;

void hashInfo()
{
    for (int i = 0; i < HASHCOUNT; i++)
    {
        cout << tTable[i].size();
    }
}

void zobristFill()
{
    zob.zobristFill();
}

int zobristKey(Board* b)
{
    int key = -1; //-1 significes uninitialized
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece* p = b->getPiece(Coord(i, j));
            if (p != nullptr)
            {
                if (key == -1)
                {
                    key = getZobristValue(p);
                }
                else
                {
                    key ^= getZobristValue(p);
                }
            }
        }
    }
    
    if (b->getTurn() == 'B')
    {
        key ^= zob.blackToMove;
    }
    
    return key % HASHCOUNT;
}

int getZobristValue(Piece* p)
{
    if (p == nullptr)
    {
        cerr << "Trying to get zobrist value of an empty square" << endl;
        exit(1);
    }
    else
    {
        if (p->getColor() == 'W')
        {
            switch (p->type())
            {
                case 'P':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][0];
                case 'B':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][1];
                case 'N':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][2];
                case 'R':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][3];
                case 'Q':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][4];
                case 'K':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][5];
            }
        }
        else
        {
            switch (p->type())
            {
                case 'P':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][6];
                case 'B':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][7];
                case 'N':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][8];
                case 'R':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][9];
                case 'Q':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][10];
                case 'K':
                    return zob.val[p->getPos().getX()][p->getPos().getY()][11];
            }
        }
    }
    
    cerr << "Error in Auxiliary::getZobristValue(Piece*)" << endl;
    exit(1);
}

void clearHash()
{
    for (int i = 0; i < HASHCOUNT; i++)
    {
        for (list<TupleHASH>::iterator itr = tTable[i].begin(); itr != tTable[i].end(); itr++)
        {
            Board* b = (*itr).b;
            itr = tTable[i].erase(itr);
            delete b;
        }
    }
}

TuplePC reccomendMove(Board* b, char turn, int depth, double alpha, double beta)
{
    TuplePC tuple;          //keeps track of what will be returned
    double bestEval = 0.0;  //keeps track of the best eval
    bool endNode = true;   //if no legal moves at all, we call static eval
    /*
     RECURSIVE STEP
     */
    
    for (list<TupleHASH>::iterator itr = tTable[zobristKey(b)].begin(); itr != tTable[zobristKey(b)].end(); itr++)
    {
        if ((*itr->b) == *b && (*itr).depth >= depth)
        {
            tuple = {b->getPiece((*itr).p->getPos()), (*itr).c, (*itr).eval};
            return tuple;
        }
    }
    
    if (depth > 1)
    {
        if (turn == 'W')
        {
            //Iterate through pieces
            
                list<TupleCC> li = getOrderedLegalMoves(b);
                //Iterate through legal moves
            for (list<TupleCC>::iterator itr = li.begin(); itr != li.end(); itr++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr).s), (*itr).e);
                    temp->nextTurn();
                    /*
                     We need to know what the opponent will play in this scenario to evaluate if this is a path we want to go down. So, we call reccomendMove, but for the opposite side, on the temp board, and at 1 less depth
                     */
                    TuplePC tempTuple = reccomendMove(temp, 'B', depth - 1, alpha, beta);
                    alpha = max(alpha, tempTuple.eval);
                    /*
                     If the tuple is empty (e.g. first move in our set) OR the eval is better than bestEval, then we replace this as our move
                     */
                    if (tuple.p == nullptr || tempTuple.eval > bestEval)
                    {
                        tuple.p = b->getPiece((*itr).s);
                        tuple.c = (*itr).e;
                        tuple.eval = tempTuple.eval;
                        bestEval = tempTuple.eval;
                    }
                    //Deleting dynamically allocated temporary board
                    delete temp;
                    //Pruning
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
        }
        else
        {
            //Iterate through pieces
            
                list<TupleCC> li = getOrderedLegalMoves(b);
                //Iterate through legal moves
                 for (list<TupleCC>::iterator itr = li.begin(); itr != li.end(); itr++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr).s), (*itr).e);
                    temp->nextTurn();
                    /*
                    We need to know what the opponent will play in this scenario to evaluate if this is a path we want to go down. So, we call reccomendMove, but for the opposite side, on the temp board, and at 1 less depth
                    */
                    TuplePC tempTuple = reccomendMove(temp, 'W', depth - 1, alpha, beta);
                    beta = min(beta, tempTuple.eval);
                    /*
                    If the tuple is empty (e.g. first move in our set) OR the eval is better than bestEval, then we replace this as our move
                    */
                    if (tuple.p == nullptr || tempTuple.eval < bestEval)
                    {
                        tuple.p = b->getPiece((*itr).s);
                        tuple.c = (*itr).e;
                        tuple.eval = tempTuple.eval;
                        bestEval = tempTuple.eval;
                    }
                    //Deleting dynamically allocated temporary board
                    delete temp;
                    //Pruning
                    if (beta <= alpha)
                    {
                        break;
                    }
            }
        }
        
    }
    else if (depth == 1)
    {
        /*
         BASE CASE (depth == 1) is true
         */
        if (turn == 'W')
        {
            //Iterate through pieces
            
                list<TupleCC> li = getOrderedLegalMoves(b);
                //Iterate through legal moves
                 for (list<TupleCC>::iterator itr = li.begin(); itr != li.end(); itr++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr).s), (*itr).e);
                    temp->nextTurn();
                    //Simply call static eval on the board
                    double currentEval = eval(temp, 'B');
                    alpha = max(alpha, currentEval);
                    /*
                    If the tuple is empty (e.g. first move in our set) OR the eval is better than bestEval, then we replace this as our move
                    */
                    
                    if (tuple.p == nullptr || currentEval > bestEval)
                    {
                        tuple.p = b->getPiece((*itr).s);
                        tuple.c = (*itr).e;
                        bestEval = currentEval;
                        tuple.eval = bestEval;
                    }
                    //Deleting dynamically allocated temporary board
                    delete temp;
                    //Pruning
                    if (alpha >= beta)
                    {
                        break;
                    }
            }
        }
        else
        {
            //Iterate through pieces
            
                
               list<TupleCC> li = getOrderedLegalMoves(b);
                //Iterate through legal moves
                 for (list<TupleCC>::iterator itr = li.begin(); itr != li.end(); itr++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr).s), (*itr).e);
                    temp->nextTurn();
                    //Simply call static eval on board
                    double currentEval = eval(temp, 'W');
                    beta = min(beta, currentEval);
                    /*
                    If the tuple is empty (e.g. first move in our set) OR the eval is better than bestEval, then we replace this as our move
                    */
                    
                    if (tuple.p == nullptr || currentEval < bestEval)
                    {
                        tuple.p = b->getPiece((*itr).s);
                        tuple.c = (*itr).e;
                        bestEval = currentEval;
                        tuple.eval = bestEval;
                    }
                    //Deleting dynamically allocated temporary board
                    delete temp;
                    //Pruning
                    if (beta <= alpha)
                    {
                        break;
                    }

            }
        }
    }
    else
    {
        cerr << "Passing an invalid depth to Auxiliary::reccomendMove(Board*,char,int)" << endl;
        exit(1);
    }
    
    if (endNode) //e.g. no legal moves
    {
        if (b->getTurn() == 'W')
        {
            if (b->getKing('W')->attackers.empty()) //stalemate
            {
                tuple.eval = 0.0;
            }
            else //checkmate, white lost
            {
                tuple.eval = -100.0;
            }
        }
        else
        {
            if (b->getKing('B')->attackers.empty()) //stalemate
            {
                tuple.eval = 0.0;
            }
            else //checkmate, black lost
            {
                tuple.eval = 100.0;
            }
        }
    }
    /*
     Here is where we store the tuple into the hash table
     We need to create new stuff though
     e.g. say we have a struct HASHENTRY
     that holds a board, a piece, a best move, an eval, and a depth
     
     -create new board using copy constructor - make sure it does it correctly!
     -store piece into struct (temp->getPiece(tuple.p->getPos())
     -store move into struct (tuple.c)
     -store eval and depth into struct (only if not 1)
     
     -At beginning, we hash into the tTable
     -Iterate through the bucket
     -If we find that the boards are the same, then:
     -Check if depth in tTable is > or = to what we're looking for, if it is then we use that
     */
    if (!endNode) //maxdepth
    {
        Board* storedInHashTable = new Board(*b);
        TupleHASH tupleHash = {storedInHashTable, storedInHashTable->getPiece(tuple.p->getPos()), tuple.c, tuple.eval, depth};
        tTable[zobristKey(b)].push_back(tupleHash);
    }
    
    return tuple;
}

double eval(Board* b, char c)
{
    /*
     DETERMINING GAME-STAGE PHASE
     */
    enum GamePhase {OPENING, MIDDLEGAME, ENDGAME};
    GamePhase gameStage;
    int numPieces = 0; //discounting pawns and king
    int developedWhite = 0;
    int developedBlack = 0;
    
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        if ((*itr)->worth() >= 3)
        {
            numPieces++;
            if ((*itr)->hasMoved())
            {
                developedWhite++;
            }
        }
    }
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        if ((*itr)->worth() >= 3)
        {
            numPieces++;
            if ((*itr)->hasMoved())
            {
                developedBlack++;
            }
        }
    }
    
    if (numPieces <= 6)
    {
        gameStage = ENDGAME;
    }
    else if ((numPieces <= 10 && numPieces > 6) || developedBlack >= 5 || developedWhite >= 5)
    {
        gameStage = MIDDLEGAME;
    }
    else
    {
        gameStage = OPENING;
    }
    
    /*
     GAME-STAGE DETERMINATION COMPLETE
     */
    
    double score;
    
    switch (gameStage)
    {
        case OPENING:
            score = evalOpening(b, c);
            break;
        case MIDDLEGAME:
            score = evalMiddleGame(b, c);
            break;
        case ENDGAME:
            score = evalEndGame(b, c);
            break;
        default:
            cerr << "Invalid Game Stage in Auxiliary::eval(Board*,char)" << endl;
            exit(1);
            break;
    }
    
    return score;
}

double evalOpening(Board* b, char color)
{
    /*
     This is how the opening is evaluated:
     -A simple linear combination of the worth of the pieces for each side
     -A mobility bonus
     -A bonus linearly proportional to the number of pieces defended
     -A detector if a piece is in danger of being captured (evalPiece)
     -A penalty for moving rooks / queen too early
     -Piece square table
     */
    
    double score = 0.0;
    
    //Simply adding up material for white
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        score += (*itr)->worth();
        score += .04*(*itr)->legalMoves.size();
        score += .06*(*itr)->defenders.size();
    }
    //And subtracting black's material
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        score -= (*itr)->worth();
        score -= .04*(*itr)->legalMoves.size();
        score -= .06*(*itr)->defenders.size();
    }
    
    score += .8*evalPiece(b, color);
    score += evalPst(b);
    score -= majorPiecePenalty(b, color);
    
    return score;
}

double evalMiddleGame(Board* b, char color)
{
    /*
    This is how the middlegame is evaluated (with respect to opening):
    -A simple linear combination of the worth of the pieces for each side
    -A mobility bonus (50% higher than in the opening, but still slight)
    -A detector if a piece is in danger of being captured (evalPiece)
    -Penalty for moving rooks / queen too early is REMOVED
    -A bonus linearly proportional to the number of pieces defended
    -Added penalty for isolated / doubled pawns
    -Piece square table
    */
    
    double score = 0.0;
    //Simply adding up material for white
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        score += (*itr)->worth();
        score += .06*(*itr)->legalMoves.size();
        score += .09*(*itr)->defenders.size();
    }
    //And subtracting black's material
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        score -= (*itr)->worth();
        score -= .06*(*itr)->legalMoves.size();
        score -= .09*(*itr)->defenders.size();
    }
    
    score += evalPst(b);
    score += .8*evalPiece(b, color);
    score += evalPawn(b, 'W');
    score -= evalPawn(b, 'B');
    
    return score;
}

double evalEndGame(Board* b, char color)
{
    /*
     This is how the endgame is evaluated (with respect to middlegame):
     -A simple linear combination of the worth of the pieces for each side
     -A mobility bonus (50% higher than in the opening, but still slight)
     -A detector if a piece is in danger of being captured (evalPiece)
     -A bonus linearly proportional to the number of pieces defended (increased weighting)
     -INCREASED penalty for isolated / doubled pawns
     -New piece square table endgame-oriented
     */
    
    double score = 0.0;
    //Simply adding up material for white
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        score += (*itr)->worth();
        score += .08*(*itr)->legalMoves.size();
        score += .12*(*itr)->defenders.size();
        
        //penalty for unprotected pawns that guard others - and have more attackers than defenders
        if ((*itr)->type() == 'P' && (*itr)->defenders.size() < (*itr)->attackers.size())
        {
            score -= .4;
        }
    }
    //And subtracting black's material
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        score -= (*itr)->worth();
        score -= .08*(*itr)->legalMoves.size();
        score -= .12*(*itr)->defenders.size();
        
        //penalty for unprotected pawns that guard others
        if ((*itr)->type() == 'P' && (*itr)->defenders.size() < (*itr)->attackers.size())
        {
            score += .4;
        }
    }
    
    score += evalPstEg(b);
    score += .8*evalPiece(b, color);
    score += 1.5*evalPawn(b, 'W');
    score -= 1.5*evalPawn(b, 'B');
    
    return score;
}

double evalPst(Board* b)
{
    double score = 0.0;
    //Iterating through white pieces
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
        //Look at the appropriate PST, and the bonus assigned at that tile
        switch ((*itr)->type())
        {
            case 'P':
                score += pstPawn[c.getX()][c.getY()]/100;
                break;
            case 'B':
                score += pstBishop[c.getX()][c.getY()]/100;
                break;
            case 'N':
                score += pstKnight[c.getX()][c.getY()]/100;
                break;
            case 'R':
                score += pstRook[c.getX()][c.getY()]/100;
                break;
            case 'Q':
                score += pstQueen[c.getX()][c.getY()]/100;
                break;
            case 'K':
                score += pstKing[c.getX()][c.getY()]/100;
                break;
        }
    }
    //Iterate through the black pieces
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
        //Look at the appropriate PST, and the bonus assigned at that tile
        //We use "7 - x" since the PST was created from white's point of view, and so we must flip the board about the horizontal axis for the bonuses to be consistent
        switch ((*itr)->type())
        {
            case 'P':
                score -= pstPawn[7 - c.getX()][c.getY()]/100;
                break;
            case 'B':
                score -= pstBishop[7 - c.getX()][c.getY()]/100;
                break;
            case 'N':
                score -= pstKnight[7 - c.getX()][c.getY()]/100;
                break;
            case 'R':
                score -= pstRook[7 - c.getX()][c.getY()]/100;
                break;
            case 'Q':
                score -= pstQueen[7 - c.getX()][c.getY()]/100;
                break;
            case 'K':
                score -= pstKing[7 - c.getX()][c.getY()]/100;
                break;
        }
    }
    return score;
}

double evalPstEg(Board* b)
{
    double score = 0.0;
    //Iterate through the white pieces
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
        //Look at the appropriate PST, and the bonus assigned at that tile
        switch ((*itr)->type())
        {
            case 'P':
                score += pstPawn[c.getX()][c.getY()]/100;
                break;
            case 'B':
                score += pstBishop[c.getX()][c.getY()]/100;
                break;
            case 'N':
                score += pstKnight[c.getX()][c.getY()]/100;
                break;
            case 'R':
                score += pstRook[c.getX()][c.getY()]/100;
                break;
            case 'Q':
                score += pstQueen[c.getX()][c.getY()]/100;
                break;
            case 'K':
                score += pstKingEg[c.getX()][c.getY()]/100;
                break;
        }
    }
    //Iterate through the black pieces
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
        //Look at the appropriate PST, and the bonus assigned at that tile
        //We use "7 - x" since the PST was created from white's point of view, and so we must flip the board about the horizontal axis for the bonuses to be consistent
        switch ((*itr)->type())
        {
            case 'P':
                score -= pstPawn[7 - c.getX()][c.getY()]/100;
                break;
            case 'B':
                score -= pstBishop[7 - c.getX()][c.getY()]/100;
                break;
            case 'N':
                score -= pstKnight[7 - c.getX()][c.getY()]/100;
                break;
            case 'R':
                score -= pstRook[7 - c.getX()][c.getY()]/100;
                break;
            case 'Q':
                score -= pstQueen[7 - c.getX()][c.getY()]/100;
                break;
            case 'K':
                score -= pstKingEg[7 - c.getX()][c.getY()]/100;
                break;
        }
    }
    return score;
}

double evalPiece(Board* b, char color)
{
    double highestWhitePenalty = 0.0;
    double highestBlackPenalty = 0.0;
    
    double secondHighestWhitePenalty = 0.0;
    double secondHighestBlackPenalty = 0.0;
    /*
     We examine if any pieces are attacked by a weaker piece
     -if undefended and attacked, check worth against current highest penalty
     -if defended and attacked, check (worth - weakest attacker) against current highest penalty
     -if worth == weakest attacker don't check
     -if worth < weakest attacker don't check
     */
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        if (!(*itr)->attackers.empty())
        {
            if ((*itr)->defenders.empty())
            {
                if ((*itr)->worth() > highestWhitePenalty)
                {
                    highestWhitePenalty = (*itr)->worth();
                }
                else if ((*itr)->worth() > secondHighestWhitePenalty)
                {
                    secondHighestWhitePenalty = (*itr)->worth();
                }
            }
            else
            {
                double val = (*itr)->worth() - (*itr)->getWeakestAttacker()->worth();
                /*
                Assuming nonempty list of defenders, and the weakest (in this case only due to the nature of getWeakestAttacker) is the king
                    Then no added penalty
                */
                if (val > highestWhitePenalty)
                {
                    highestWhitePenalty = val;
                }
                else if (val > secondHighestWhitePenalty)
                {
                    secondHighestWhitePenalty = val;
                }
            }
        }
    }

    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        if (!(*itr)->attackers.empty())
        {
            if ((*itr)->defenders.empty())
            {
                if ((*itr)->worth() > highestBlackPenalty)
                {
                    highestBlackPenalty = (*itr)->worth();
                }
                else if ((*itr)->worth() > secondHighestBlackPenalty)
                {
                    secondHighestBlackPenalty = (*itr)->worth();
                }
            }
            else
            {
                double val = (*itr)->worth() - (*itr)->getWeakestAttacker()->worth();
                /*
                 Assuming nonempty list of defenders, and the weakest (in this case only due to the nature of getWeakestAttacker) is the king
                 Then no added penalty
                 */
                if (val > highestBlackPenalty)
                {
                    highestBlackPenalty = val;
                }
                else if (val > secondHighestBlackPenalty)
                {
                    secondHighestBlackPenalty = val;
                }
            }
        }
    }
    
    if (color == 'W')
    {
        return highestBlackPenalty - secondHighestWhitePenalty;
    }
    else
    {
        return secondHighestBlackPenalty - highestWhitePenalty;
    }
}

double evalPawn(Board* b, char color)
{
    /*
     The ith index in numPawnsInCol represents the number of pawns of a certain color in the
     (i + 1)th row of the board
     */
    int numPawnsInCol[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int numIsolatedPawn = 0;
    int numDoubledPawn = 0;
    
    if (color == 'W')
    {
        for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
        {
            if ((*itr)->type() == 'P')
            {
                numPawnsInCol[(*itr)->getPos().getY()]++;
            }
        }
    }
    else
    {
        for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
        {
            if ((*itr)->type() == 'P')
            {
                numPawnsInCol[(*itr)->getPos().getY()]++;
            }
        }
    }
    //Boundry conditions
    if (numPawnsInCol[0] >= 2)
    {
        numDoubledPawn++;
    }
    if (numPawnsInCol[7] >= 2)
    {
        numDoubledPawn++;
    }
    if (numPawnsInCol[0] >= 0 && numPawnsInCol[1] == 0)
    {
        numIsolatedPawn++;
    }
    if (numPawnsInCol[7] >= 0 && numPawnsInCol[6] == 0)
    {
        numIsolatedPawn++;
    }
    
    /*
     General case:
     if a column has a nonzero number of pawns in it, while the adjacent columns do not have pawns in them, then an isolated pawn penalty is incurred
     if a column has 2 or more pawns in it, then a doubled pawn penalty is incurred
     */
    for (int i = 1; i < 7; i++)
    {
        if (numPawnsInCol[i] >= 0 && numPawnsInCol[i - 1] == 0 && numPawnsInCol[i + 1] == 0)
        {
            numIsolatedPawn++;
        }
        if (numPawnsInCol[i] >= 2)
        {
            numDoubledPawn++;
        }
    }
    /*
     WEIGHTING:
     each doubled pawn is a penalty of -.3
     each isolated pawn is a penalty of -.2
     */
    return -1*(.3*numDoubledPawn + .2*numIsolatedPawn);
}

double majorPiecePenalty(Board* b, char c)
{
    double score = 0.0;;
    
    if (!b->getKing('W')->hasMoved())
    {
        for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
        {
            if ((*itr)->worth() >= 5 && (*itr)->hasMoved())
            {
                score += .43;
            }
        }
    }
    if (!b->getKing('B')->hasMoved())
    {
        for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
        {
            if ((*itr)->worth() >= 5 && (*itr)->hasMoved())
            {
                score -= .43;
            }
        }
    }
    
    return score;
}

const Set getRay(const Piece* p, char& dir)
{
    /*
     Returns a set of coords containing the king
     May be horizontal or diagonal or vertical
     Will start near the piece and emanate towards king
     Empty set if no ray directed towards king
     Restricted to only rook, queen, and bishop
     Assuming piece and enemy king are on different coordinates
     */
    dir = 'N'; //If no ray, no direction, represented by 'N'
    
    bool doStraight, doDiagonal;
    switch (p->type())
    {
        case 'B':
            doStraight = false;
            doDiagonal = true;
            break;
        case 'R':
            doStraight = true;
            doDiagonal = false;
            break;
        case 'Q':
            doStraight = true;
            doDiagonal = true;
            break;
        default:
            doStraight = false;
            doDiagonal = false;
            break;
    }
    
    Set set;
    Coord c(p->getPos());
    Piece* enemyKing = p->getBoard()->getKing(p->getOppositeColor());
    Coord e = enemyKing->getPos();
    
    if (doStraight)
    {
        if (c.getX() == e.getX())
        {
            dir = 'H';
            //case horizontal
            if (c.getY() < e.getY())
            {
                int i = 1;
                while ((c + Coord(0, i)).getY() < e.getY())
                {
                    set.insert(c + Coord(0, i));
                    i++;
                }
            }
            else
            {
                int i = -1;
                while ((c + Coord(0, i)).getY() > e.getY())
                {
                    set.insert(c + Coord(0, i));
                    i--;
                }
            }
        }
        else if (c.getY() == e.getY())
        {
            dir = 'V';
            //case vertical
            if (c.getX() < e.getX())
            {
                int i = 1;
                while ((c + Coord(i, 0)).getX() < e.getX())
                {
                    set.insert(c + Coord(i, 0));
                    i++;
                }
            }
            else
            {
                int i = -1;
                while ((c + Coord(i, 0)).getX() > e.getX())
                {
                    set.insert(c + Coord(i, 0));
                    i--;
                }
            }
        }
        
    }
    
    if (doDiagonal)
    {
        Coord differential(e - c);
        //case y = -x diagonal
        if (differential.getX() == differential.getY())
        {
            dir = 'D';
            //case DR vector
            if (c.getX() < e.getX())
            {
                int i = 1;
                while ((c + Coord(i, i)).getX() < e.getX())
                {
                    set.insert(c + Coord(i, i));
                    i++;
                }
            }
            //case UL vector
            else
            {
                int i = -1;
                while ((c + Coord(i, i)).getX() > e.getX())
                {
                    set.insert(c + Coord(i, i));
                    i--;
                }
            }
        }
        //case y = x diagonal
        else if (differential.getX() == -1*differential.getY())
        {
            dir = 'd';
            //case UR vector
            if (c.getX() > e.getX())
            {
                int i = 1;
                while ((c + Coord(-i, i)).getX() > e.getX())
                {
                    set.insert(c + Coord(-i, i));
                    i++;
                }
            }
            //case DL vector
            else
            {
                int i = 1;
                while ((c + Coord(i, -i)).getX() < e.getX())
                {
                    set.insert(c + Coord(i, -i));
                    i++;
                }
            }
        }
    }
    return set;
}

const char defineDir(const Coord c1, const Coord c2)
{
    //assumes c1 and c2 are distinct
    if (c1.getX() == c2.getX())
    {
        return 'H';
    }
    else if (c1.getY() == c2.getY())
    {
        return 'V';
    }
    else if (c1.getX() - c2.getX() == c1.getY() - c2.getY())
    {
        return 'D';
    }
    else if (c1.getX() - c2.getX() == c2.getY() - c1.getY())
    {
        return 'd';
    }
    else
    {
        return 'N';
    }
}


bool containsCoord(list<Coord> li, Coord c)
{
    //iterating through the list
    for (list<Coord>::iterator itr = li.begin(); itr != li.end(); itr++)
    {
        //if the iterator "points" to the coord, return true
        if ((*itr) == c)
        {
            return true;
        }
    }
    return false; //else return false
}

Piece* getPinned(const Piece* p, char& dir)
{
    Set ray(getRay(p, dir));
    /*
     Analyzes a ray:
        If only one piece in the ray, that piece is pinned, and it returns that piece
        If 0 or multiple pieces in the ray, returns nullptr
     */
    
    bool foundPiece = false;
    Piece* returnPiece = nullptr;
    
    for (int i = 0; i < ray.size(); i++)
    {
        Coord c;
        c = ray.get(i);
        Piece* k = p->getBoard()->getPiece(c);
        
        if (k != nullptr)
        {
            if (foundPiece || p->getColor() == k->getColor())
            {
                return nullptr;
            }
            foundPiece = true;
            returnPiece = k;
        }
    }
    return returnPiece;
}

list<TupleCC> getOrderedLegalMoves(Board* b)
{
    list<TupleCC> li;
    
    if (b->getTurn() == 'W')
    {
        for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
        {
            for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
            {
                TupleCC tuple;
                tuple.s = (*itr)->getPos();
                tuple.e = *itr2;
                
                bool inserted = false;
                Board* temp = new Board(*b);
                temp->moveRaw(temp->getPiece(tuple.s), tuple.e);
                temp->nextTurn();
                tuple.eval = eval(temp, temp->getTurn());
                                
                for (list<TupleCC>::iterator i = li.begin(); i != li.end(); i++)
                {
                    if ((*i).eval <= tuple.eval)
                    {
                        li.insert(i, tuple);
                        inserted = true;
                        break;
                    }
                }
                
                if (!inserted)
                {
                    li.push_back(tuple);
                }
                delete temp;
            }
        }
    }
    else
    {
        for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
        {
            for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
            {
                TupleCC tuple;
                tuple.s = (*itr)->getPos();
                tuple.e = *itr2;
                
                bool inserted = false;
                Board* temp = new Board(*b);
                temp->moveRaw(temp->getPiece(tuple.s), tuple.e);
                temp->nextTurn();
                tuple.eval = eval(temp, temp->getTurn());
                                
                for (list<TupleCC>::iterator i = li.begin(); i != li.end(); i++)
                {
                    if ((*i).eval >= tuple.eval)
                    {
                        li.insert(i, tuple);
                        inserted = true;
                        break;
                    }
                }
                
                if (!inserted)
                {
                    li.push_back(tuple);
                }
                delete temp;
            }
        }
    }
    
    
    return li;
}

void print(Board* b, char c)
{
    if (c == 'W')
    {
        for (int i = 0; i < 8; i++)
        {
            cout << 8 - i << " "; //number coordinates
            for (int j = 0; j < 8; j++)
            {
                Piece* p = b->getPiece(Coord(i, j));
                if (p != nullptr)
                {
                    cout << ' ' << p->getColor() << p->type() << ' ';
                }
                else
                {
                    cout << " .. ";
                }
            }
            cout << endl << endl;
        }
        cout << "   a   b   c   d   e   f   g   h" << endl << endl; //letter coordinates
    }
    else if (c == 'B')
    {
        for (int i = 7; i >= 0; i--)
        {
            cout << 8 - i << " "; //number coordinates
            for (int j = 7; j >= 0; j--)
            {
                Piece* p = b->getPiece(Coord(i, j));
                if (p != nullptr)
                {
                    cout << ' ' << p->getColor() << p->type() << ' ';
                }
                else
                {
                    cout << " .. ";
                }
            }
            cout << endl << endl;
        }
        cout << "   h   g   f   e   d   c   b   a" << endl << endl; //letter coordinates
    }
}
