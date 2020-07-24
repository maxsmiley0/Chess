//
//  Auxiliary.cpp
//  Chess
//
//  Created by Max Smiley on 6/30/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//
/*
 ,476.621
 ,1019.58
 ,3079.75
 ,3577.13
 ,3462.71
 ,3654.03
 ,2923.18
 ,1711.18
 ,1112
 ,401.958
 ,1200.1
 ,874.527
 */
#include <iostream>
#include <cmath>

#include "Auxiliary.h"
#include "Board.h"
#include "Piece.h"
#include "Coord.h"
#include "Set.h"
#include "PST.h"

using namespace std;

list<TupleBE> whiteBoards[HASHCOUNT];
list<TupleBE> blackBoards[HASHCOUNT];

void hashInfo()
{
    for (int i = 0; i < HASHCOUNT; i++)
    {
        cout << whiteBoards[i].size() << blackBoards[i].size();
    }
}

void clearHash(char c)
{
    if (c == 'W')
    {
        for (int i = 0; i < HASHCOUNT; i++)
        {
            for (list<TupleBE>::iterator itr = whiteBoards[i].begin(); itr != whiteBoards[i].end(); itr++)
            {
                delete (*itr).b; //deleting the board
                itr = whiteBoards[i].erase(itr);
            }
        }
    }
    else
    {
        for (int i = 0; i < HASHCOUNT; i++)
        {
            for (list<TupleBE>::iterator itr = blackBoards[i].begin(); itr != blackBoards[i].end(); itr++)
            {
                delete (*itr).b; //deleting the board
                itr = blackBoards[i].erase(itr);
            }
        }
    }
}

double eval(Board* b, char c)
{
    //Checking for having already searched goes here
    unsigned long hashKey = b->hashmap();
    if (c == 'W')
    {
        for (list<TupleBE>::iterator itr = whiteBoards[hashKey].begin(); itr != whiteBoards[hashKey].end(); itr++)
        {
            if (*itr->b == *b && (*itr->b).getTurn() != b->getTurn()) //dubious, have we defined operator== for pointers?
            {
                delete b;
                return (*itr).eval;
            }
        }
    }
    else
    {
        for (list<TupleBE>::iterator itr = blackBoards[hashKey].begin(); itr != blackBoards[hashKey].end(); itr++)
        {
            if ((*itr->b) == *b) //dubious, have we defined operator== for pointers?
            {
                cout << "run";
                return (*itr).eval;
            }
        }
    }
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
    
    if (c == 'W')
    {
        TupleBE tuple = {b, score};
        whiteBoards[hashKey].push_back(tuple);
    }
    else
    {
        TupleBE tuple = {b, score};
        blackBoards[hashKey].push_back(tuple);
    }
    return score;
}

double evalOpening(Board* b, char color)
{
    /*
     This is how the opening is evaluated:
     -A simple linear combination of the worth of the pieces for each side
     -A mobility bonus
     -A detector if a piece is in danger of being captured (evalPiece)
     -A penalty for moving rooks / queen too early
     -A king safety bonus
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
    //score += centerBonus(b, color);
    score += evalPst(b);
    score -= majorPiecePenalty(b, color);
    //score += evalKingSafety(b, color);
    
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
    -An added bonus for overprotection
    -An added penalty for unprotected pawns that guard others
    -Center controlling bonus reduced to 33%
    -Added penalty for isolated / doubled pawns
     
    -A king safety bonus (TO BE ADDED)
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
    //score += .33*centerBonus(b, color);
    score += evalPawn(b, 'W');
    score -= evalPawn(b, 'B');
    //score += evalKingSafety(b, color);
    
    return score;
}

double evalEndGame(Board* b, char color)
{
    /*
     This is how the endgame is evaluated (with respect to middlegame):
     
     -A simple linear combination of the worth of the pieces for each side
     -Mobility bonus NOT removed
     -A detector if a piece is in danger of being captured (evalPiece)
     -Overprotection bonus removed
     -An added penalty for unprotected pawns that guard others (like before)
     -Center controlling bonus removed
     -Penalty for isolated / doubled pawns INCREASED weighting
     
     -A king activity bonus (TO BE ADDED)
     -A passed pawn (protected / candidate) analyzer
     -A king safety bonus (TO BE ADDED)
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
    //score += evalKingActivity(b, color);
    //score += evalPassedPawn(b, color);
    
    return score;
}

double evalPst(Board* b)
{
    double score = 0.0;
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
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
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
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
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
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
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        Coord c((*itr)->getPos());
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
    
    /*
     We examine if any pieces are attacked by a weaker piece
     -if undefended and attacked, check agains HXP
     -if defended and attacked, check worth against weakest attacker
     -if worth > weakest attacker check worth-weakest attacker against HXP
     -if worth == weakest attacker don't bother
     -if worth < weakest attacker don't bother
     */
    if (color == 'B')
    {
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
                }
                else
                {
                    double val = (*itr)->worth() - (*itr)->getWeakestAttacker()->worth();
                    /*
                     Assuming nonempty list of defenders, and the weakest (in this case only due to the nature of getWeakestAttacker) is the king
                     Then no added penalty
                     */
                    if (val > 0 && val > highestWhitePenalty)
                    {
                        highestWhitePenalty = val;
                    }
                }
            }
        }
    }
    else
    {
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
                }
                else
                {
                    double val = (*itr)->worth() - (*itr)->getWeakestAttacker()->worth();
                    if (val > 0 && val > highestBlackPenalty)
                    {
                        highestBlackPenalty = val;
                    }
                }
            }
        }
    }
    return highestBlackPenalty - highestWhitePenalty;
}

double evalPassedPawn(Board* b, char color)
{
    double score = 0.0;
    
    //If either protected awns will be worth 1 extra point
    
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        if ((*itr)->type() == 'P')
        {
            if ((*itr)->getPos().getX() < 4)
            {
                //more of a bonus if protected
                if ((*itr)->defenders.empty())
                {
                    score += .5*(4 - (*itr)->getPos().getX());
                }
                else
                {
                    score += .33*(4 - (*itr)->getPos().getX());
                }
            }
        }
    }
    
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        if ((*itr)->type() == 'P')
        {
            if ((*itr)->getPos().getX() > 3)
            {
                //more of a bonus if protected
                if ((*itr)->defenders.empty())
                {
                    score -= .5*((*itr)->getPos().getX() - 3);
                }
                else
                {
                    score -= .33*((*itr)->getPos().getX() - 3);
                }
            }
        }
    }
    
    return score;
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

double centerBonus(Board* b, char color)
{
    double score = 0.0;
    
    for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
    {
        //write a Piece::pseudoControlsCenter fnct
        /*
         Pawns: bonus if in d4e4 for white, d5e5 for black
                bonus if controlling attacking center squares
         Knights: bonus if in 2/3 file or 6/7
                bonus if controlling center squares
         Bishops:bonus if in certain squares (we know the ones)
         
         Rooks / queen / kings get nut'n
         */
        score += (*itr)->centerControl();
    }
    for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
    {
        score -= (*itr)->centerControl();
    }
    
    return score;
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

double evalKingSafety(Board* b, char c)
{
    double score = 0.0;
    
    Piece* whiteKing = b->getKing('W');
    Piece* blackKing = b->getKing('B');
    
    if (whiteKing->getPos().getX() != 7)
    {
        //penalty if king not on back rank
        score -= .4;
    }
    else if (whiteKing->getPos().getY() == 3)
    {
        //penalty for king on the d file
        score -= .4;
    }
    else if (whiteKing->getPos().getY() == 1 || whiteKing->getPos().getY() == 2 || whiteKing->getPos().getY() == 6 || whiteKing->getPos().getY() == 7)
    {
        //bonus if king is on back rank and is castled / has moved while castled
        score += .3;
        //extra bonus if king is surrounded by pawns while castled
        if (whiteKing->getNumDefending() >= 2)
        {
            score += .4;
        }
    }
    
    if (blackKing->getPos().getX() != 0)
    {
        //penalty if king not on back rank
        score += .4;
    }
    else if (blackKing->getPos().getY() == 3)
    {
        //penalty for king on the d file
        score += .4;
    }
    else if (blackKing->getPos().getY() == 1 || blackKing->getPos().getY() == 2 || blackKing->getPos().getY() == 6 || blackKing->getPos().getY() == 7)
    {
        //bonus if king is on back rank and is castled / has moved while castled
        score -= .3;
        //extra bonus if king is surrounded by pawns while castled
        if (blackKing->getNumDefending() >= 2)
        {
            score -= .4;
        }
    }
    
    return score;
}

double evalKingActivity(Board* b, char c)
{
    double score = 0.0;
    Piece* whiteKing = b->getKing('W');
    Piece* blackKing = b->getKing('B');
    
    /*
     How do we evaluate? probably king is near at least one piece
     King is either:
     more towards the middle than the other king
     if passed pawn, defending / accosting that pawn
     trying to work behind the ranks to capture enemy pawns
     */
    
    if (whiteKing->getPos().getX() < 5)
    {
        score += .22;
    }
    if (whiteKing->getNumDefending() != 0)
    {
        score += .44;
    }
    if (whiteKing->getNumAttacking() != 0)
    {
        score += .55;
    }
    
    if (blackKing->getPos().getX() < 5)
    {
        score -= .22;
    }
    if (blackKing->getNumDefending() != 0)
    {
        score -= .44;
    }
    if (blackKing->getNumAttacking() != 0)
    {
        score -= .55;
    }
    score += .1*whiteKing->legalMoves.size();
    score -= .1*blackKing->legalMoves.size();
    
    return score;
}

const bool isAttacked(const Piece* p)
{
    if (!p->getBoard()->canReachCoordPseudo(p->getPos(), p->getOppositeColor()).empty())
    {
        return true;
    }
    else
    {
        return false;
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

const bool isPinned(const Piece* p)
{
    if (isAttacked(p))
    {
        list<Piece*> li = p->getBoard()->canReachCoordPseudo(p->getPos(), p->getOppositeColor());
        list<Piece*>::iterator itr = li.begin();
        for (; itr != li.end(); itr++)
        {
            //all pieces that can reach a certain square
            
        }
    }
    /*
     pinned iff:
     -attacked (check first so we can discount others)
     -in a ray with same side king
     -only piece in ray
     
     if pinned:
     -can only move along the ray
     */
    return true;
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

TuplePC reccomendMove(Board* b, char turn, int depth, double alpha, double beta)
{
    TuplePC tuple;          //keeps track of what will be returned
    double bestEval = 0.0;  //keeps track of the best eval
    bool endNode = true;   //if no legal moves at all, we call static eval
    /*
     RECURSIVE STEP
     */
    if (depth > 1)
    {
        if (turn == 'W')
        {
            //Iterate through pieces
            for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
            {
                //Iterate through legal moves
                for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr)->getPos()), *itr2);
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
                        tuple.p = *itr;
                        tuple.c = *itr2;
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
            for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
            {
                //Iterate through legal moves
                for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr)->getPos()), *itr2);
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
                        tuple.p = *itr;
                        tuple.c = *itr2;
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
            for (list<Piece*>::iterator itr = b->whitePieces.begin(); itr != b->whitePieces.end(); itr++)
            {
                //Iterate through legal moves
                for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
                {
                    endNode = false;
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr)->getPos()), *itr2);
                    temp->nextTurn();
                    //Simply call static eval on the board
                    double currentEval = eval(temp, 'B');
                    alpha = max(alpha, currentEval);
                    /*
                    If the tuple is empty (e.g. first move in our set) OR the eval is better than bestEval, then we replace this as our move
                    */
                    
                    if (tuple.p == nullptr || currentEval > bestEval)
                    {
                        tuple.p = *itr;
                        tuple.c = *itr2;
                        bestEval = currentEval;
                        tuple.eval = bestEval;
                    }
                    //We DON'T delete the board here, because at depth zero, the board will be pushed into the transposition table to be deleted in Auxiliary::clearHash
                    //Pruning
                    if (alpha >= beta)
                    {
                        break;
                    }
                }
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
            for (list<Piece*>::iterator itr = b->blackPieces.begin(); itr != b->blackPieces.end(); itr++)
            {
                endNode = false;
                //Iterate through legal moves
                for (list<Coord>::iterator itr2 = (*itr)->legalMoves.begin(); itr2 != (*itr)->legalMoves.end(); itr2++)
                {
                    //Creating a temp board to evaluate, moving piece in temp board
                    Board* temp = new Board(*b);
                    temp->movePiece(temp->getPiece((*itr)->getPos()), *itr2);
                    temp->nextTurn();
                    //Simply call static eval on board
                    double currentEval = eval(temp, 'W');
                    beta = min(beta, currentEval);
                    /*
                    If the tuple is empty (e.g. first move in our set) OR the eval is better than bestEval, then we replace this as our move
                    */
                    
                    if (tuple.p == nullptr || currentEval < bestEval)
                    {
                        tuple.p = *itr;
                        tuple.c = *itr2;
                        bestEval = currentEval;
                        tuple.eval = bestEval;
                    }
                    //We DON'T delete the board here, because at depth zero, the board will be pushed into the transposition table to be deleted in Auxiliary::clearHash
                    //Pruning
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
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
    
    return tuple;
}
