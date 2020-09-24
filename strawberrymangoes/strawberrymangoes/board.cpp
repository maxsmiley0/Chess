//
//  board.cpp
//  strawberrymangoes
//
//  Created by Max Smiley on 9/21/20.
//  Copyright © 2020 Max Smiley. All rights reserved.
//

#include "board.h"

Board::Board()
{
    InitKeys();
    ClearBoard();
}

int Board::getPosKey()
{
    return posKey;
}

void Board::addPiece(int r, int c, int pce)
{
    //XORing out the piece at (r, c)
    mBoard[r][c] = pce;
    
    //Updating piece numbers and piece lists
    pListR[(10 * pce) + pceNum[pce]] = r;   //setting piece row to r
    pListC[(10 * pce) + pceNum[pce]] = c;   //setting piece column to c
    pceNum[pce]++;                          //adding another piece of that type
    
    //Updating posKey
    posKey ^= pceKeys[r][c][pce];
}

void Board::removePiece(int r, int c)
{
    //XORing out the piece at (r, c)
    posKey ^= pceKeys[r][c][mBoard[r][c]];
    
    pceNum[mBoard[r][c]]--;                 //decrementing piece num of that type
    
    //Setting the square on mBoard
    mBoard[r][c] = NOPIECE;
}

void Board::hashInCastle(int castlePerm)   //assumes parameter is macro WKCA, BQCA, etc.
{
    int newPerm = (this->castlePerm | castlePerm);
    //This executes if we are trying to hash in something that is not already in there
    if (this->castlePerm != newPerm)
    {
        //Change the castle perms, and then the position key
        this->castlePerm = newPerm;
        if (castlePerm == WKCA)
        {
            posKey ^= castleKeys[0];
        }
        else if (castlePerm == WQCA)
        {
            posKey ^= castleKeys[1];
        }
        else if (castlePerm == BKCA)
        {
            posKey ^= castleKeys[2];
        }
        else if (castlePerm == BQCA)
        {
            posKey ^= castleKeys[3];
        }
    }
}

void Board::hashOutCastle(int castlePerm)   //assumes parameter is macro WKCA, BQCA, etc.
{
    int invCastlePerm = (castlePerm ^ INVERSE); //inverting the castle key
    //This executes if we are trying to hash out something that is in there
    if (this->castlePerm != (invCastlePerm & this->castlePerm))
    {
        //Change the castle perms, and then the position key
        this->castlePerm &= invCastlePerm;
        if (castlePerm == WKCA)
        {
            posKey ^= castleKeys[0];
        }
        else if (castlePerm == WQCA)
        {
            posKey ^= castleKeys[1];
        }
        else if (castlePerm == BKCA)
        {
            posKey ^= castleKeys[2];
        }
        else if (castlePerm == BQCA)
        {
            posKey ^= castleKeys[3];
        }
    }
}

void Board::hashOutEp()
{
    if (enpasSquareR != OFFBOARD)
    {
        posKey ^= enpasKey[enpasSquareR][enpasSquareC];
        enpasSquareR = OFFBOARD;
        enpasSquareC = OFFBOARD;
    }
    //We don't do anything if an en passant square was not there to begin with
}

void Board::hashInEp(int r, int c)
{
    //Assumes ep is empty
    posKey ^= enpasKey[r][c];
    enpasSquareR = r;
    enpasSquareC = c;
}

void Board::changeSide()
{
    //Changes side and changes pos key
    (side == WHITE) ? (side = BLACK) : (side = WHITE);
    posKey ^= sideKey;
}

void Board::printBoard()
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            std::cout << PceChar[mBoard[r][c]] << "   ";
        }
        std::cout << std::endl << std::endl;
    }
    
    std::cout << "Side to play: ";
    (side == WHITE) ? (std::cout << 'w' << std::endl) : (std::cout << 'b' << std::endl);
    
    std::cout << "En Pas Square: ";
    if (enpasSquareC != OFFBOARD)
    {
        std::cout << (char)((int)'a' + enpasSquareC);
        std::cout << 8 - enpasSquareR;
    }
    std::cout << std::endl;
    
    
    std::cout << "Castling Perms: ";
    if ((castlePerm & WKCA) != 0) std::cout << 'K';
    if ((castlePerm & WQCA) != 0) std::cout << 'Q';
    if ((castlePerm & BKCA) != 0) std::cout << 'k';
    if ((castlePerm & BQCA) != 0) std::cout << 'q';
    std::cout << std::endl;
    /*
    for (int i = 0; i < 12; i++)
    {
        std::cout << pceNum[i] << std::endl;
        
        for (int j = 0; j < pceNum[i]; j++)
        {
            std::cout << pListR[10 * i + j];
            std::cout << pListC[10 * i + j] << std::endl;
        }
        
        std::cout << std::endl << std::endl;
    }
    */
}

int Board::getPce(int r, int c)
{
    return mBoard[r][c];
}

int Board::getPceR(int pce, int i)
{
    return pListR[10 * pce + i];
}

int Board::getPceC(int pce, int i)
{
    return pListC[10 * pce + i];
}

int Board::getPceNum(int pce)
{
    return pceNum[pce];
}

void Board::parseFen(std::string fen)
{
    ClearBoard();
    
    int section = 0;    //0 -> pieces | 1 -> side | 2 -> castle | 3 -> enpas square
    int r = 0;          //stores row we are on
    int c = 0;          //stores column we are on
    
    for (int i = 0; i < fen.length(); i++)
    {
        //putting the pieces on the board
        if (section == 0)
        {
            switch (fen[i])
            {
                case ' ':
                    section++;
                    break;
                case 'p':
                    addPiece(r, c, BP);
                    c++;
                    break;
                case 'n':
                    addPiece(r, c, BN);
                    c++;
                    break;
                case 'b':
                    addPiece(r, c, BB);
                    c++;
                    break;
                case 'r':
                    addPiece(r, c, BR);
                    c++;
                    break;
                case 'q':
                    addPiece(r, c, BQ);
                    c++;
                    break;
                case 'k':
                    addPiece(r, c, BK);
                    c++;
                    break;
                case 'P':
                    addPiece(r, c, WP);
                    c++;
                    break;
                case 'N':
                    addPiece(r, c, WN);
                    c++;
                    break;
                case 'B':
                    addPiece(r, c, WB);
                    c++;
                    break;
                case 'R':
                    addPiece(r, c, WR);
                    c++;
                    break;
                case 'Q':
                    addPiece(r, c, WQ);
                    c++;
                    break;
                case 'K':
                    addPiece(r, c, WK);
                    c++;
                    break;
                case '/':
                    c = 0;
                    r++;
                    break;
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                    c += (int)fen[i] - (int)'0';
                    break;
                default:
                    std::cerr << "Invalid FEN" << std::endl;
                    exit(1);
                    break;
            }
        }
        //side to move
        else if (section == 1)
        {
            if (fen[i] == 'w')
            {
                section++;
                i++;
            }
            else if (fen[i] == 'b')
            {
                side = BLACK;
                posKey ^= sideKey;
                section++;
                i++;
            }
        }
        //castle perm
        else if (section == 2)
        {
            //updating position key, and castling perms
            switch (fen[i])
            {
                case ' ':
                    section++;
                    break;
                case '-':
                    break;
                case 'K':
                    posKey ^= castleKeys[0];
                    castlePerm |= WKCA;
                    break;
                case 'Q':
                    posKey ^= castleKeys[1];
                    castlePerm |= WQCA;
                    break;
                case 'k':
                    posKey ^= castleKeys[2];
                    castlePerm |= BKCA;
                    break;
                case 'q':
                    posKey ^= castleKeys[3];
                    castlePerm |= BQCA;
                    break;
                default:
                    std::cerr << "Invalid FEN" << std::endl;
                    exit(1);
                    break;
            }
        }
        //en pas square
        else if (section == 3)
        {
            if (fen[i] != '-')
            {
                //update position key
                //update en passant square
                enpasSquareC = (int)fen[i] - (int)'a';
                enpasSquareR = 8 - ((int)fen[i + 1] - (int)'0');
                posKey ^= enpasKey[enpasSquareR][enpasSquareC];
            }
            section++;
        }
    }
}

void Board::InitKeys()
{
    sideKey = RAND32();
    
    for (int i = 0; i < 4; i++)
    {
        castleKeys[i] = RAND32();
    }
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            enpasKey[i][j] = RAND32();
        }
    }
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 12; k++)
            {
                pceKeys[i][j][k] = RAND32();
            }
        }
    }
}

void Board::ClearBoard()
{
    //Resetting side to white
    side = WHITE;
    
    //Resetting history
    hisPly = 0;
    
    //Resetting en passant square and castle perms
    enpasSquareC = OFFBOARD;
    enpasSquareR = OFFBOARD;
    castlePerm = 0;
    
    //Resetting position key to zero XORed with the sideKey, since white moves first
    posKey = sideKey;
    
    //Resetting each piece on the board
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            mBoard[i][j] = NOPIECE;
        }
    }
    
    //Resetting all piece numbers to zero
    for (int i = 0; i < 12; i++)
    {
        pceNum[i] = 0;
    }
    
    //We don't need to reset piece coordinates, as they are accessed by piece num
}

bool Board::hasKcPerm()
{
    if (side == WHITE)
    {
        return ((castlePerm & WKCA) != 0);
    }
    else
    {
        return ((castlePerm & BKCA) != 0);
    }
}

bool Board::hasQcPerm()
{
    if (side == WHITE)
    {
        return ((castlePerm & WQCA) != 0);
    }
    else
    {
        return ((castlePerm & BQCA) != 0);
    }
}

int Board::getKingR(int color)
{
    if (color == WHITE)
    {
        return pListR[10 * WK];
    }
    else
    {
        return pListR[10 * BK];
    }
}

int Board::getKingC(int color)
{
    if (color == WHITE)
    {
        return pListC[10 * WK];
    }
    else
    {
        return pListC[10 * BK];
    }
}

void Board::pushHistory(int move)
{
    History h;
    h.move = move;
    h.castlePerm = castlePerm;
    h.enpasSquareR = enpasSquareR;
    h.enpasSquareC = enpasSquareC;
    
    history[hisPly] = h;
    hisPly++;
}

void Board::popHistory()
{
    hisPly--;
}

History Board::getLastState()
{
    if (hisPly == 0)
    {
        std::cerr << "Attempting to access history with no moves made (Board::getLastMove())" << std::endl;
        exit(1);
    }
    return history[hisPly - 1];
}
