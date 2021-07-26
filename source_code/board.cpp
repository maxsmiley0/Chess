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

void Board::parseFen(std::string fen)
{
    ClearBoard();       //clears board
    
    int section = 0;    //0 -> pieces | 1 -> side | 2 -> castle | 3 -> enpas square
    int r = 0;          //stores row we are on
    int c = 0;          //stores column we are on
    
    //Loop through all characters of the FEN
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
    
    //Updating material
    material += worth(pce);
}

void Board::removePiece(int r, int c)
{
    //XORing out the piece at (r, c)
    int pce = mBoard[r][c];
    
    if (pce == NOPIECE)
    {
        std::cerr << "removing no piece Board::removePiece" << std::endl;
    }
    
    posKey ^= pceKeys[r][c][pce];
    pceNum[mBoard[r][c]]--;                 //decrementing piece num of that type
    
    for (int i = 10 * pce; i < 10 * pce + pceNum[pce]; i++)
    {
        if (r == pListR[i] && c == pListC[i])
        {
            std::swap(pListR[i], pListR[10 * pce + pceNum[pce]]);
            std::swap(pListC[i], pListC[10 * pce + pceNum[pce]]);
            break;
            //does this actually work???
        }
    }
    
    //Setting the square on mBoard
    mBoard[r][c] = NOPIECE;
    
    //Updating material
    material -= worth(pce);
}

void Board::pushHistory(int move)
{
    History h;
    
    //Updating fields of history
    h.posKey = posKey;
    h.move = move;
    h.pce = mBoard[fromR(move)][fromC(move)];
    h.castlePerm = castlePerm;
    h.enpasSquareR = enpasSquareR;
    h.enpasSquareC = enpasSquareC;
    h.fiftyMove = fiftyMove;
    
    history[hisPly] = h;    //update history
    hisPly++;               //increase hisply
    
    if (h.pce == NOPIECE)
    {
        printBoard();
        std::cout << printMove(move) << std::endl;
        std::cerr << "how is this possible (pce invalid in pushHistory)" << std::endl;
        exit(1);
    }
}

void Board::popHistory()
{
    //We don't actually have to delete history, simply go back one hisPly, because if we add a new history, the previous one will be overwritten
    hisPly--;
    fiftyMove = history[hisPly].fiftyMove;
}

int Board::getPce(int r, int c) const
{
    return mBoard[r][c];
}

int Board::getPceNum(int pce) const
{
    return pceNum[pce];
}

int Board::getPceR(int pce, int i) const
{
    return pListR[10 * pce + i];
}

int Board::getPceC(int pce, int i) const
{
    return pListC[10 * pce + i];
}

int Board::getKingR(int color) const
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

int Board::getKingC(int color) const
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

int Board::numRep() const
{
    int repCount = 0;
    
    if (hisPly != 0)
    {
        //Remember - only need to check moves up to the fiftyMove counter
        for (int i = hisPly - 1; i >= fiftyMove; i--)
        {
            if (posKey == history[i].posKey)
            {
                repCount++;
            }
            if (repCount >= 2)
            {
                break;
            }
        }
    }
    else
    {
        return 0;
    }
    
    return repCount;
}

int Board::sqExposed(int r, int c) const
{
    int sq = 0;
    
    //Loop in 4 directions until we hit a piece or go offboard
    int indexR = r + 1;
    int indexC = c + 1;
    //down right direction
    while (indexR <= 7 && indexC <= 7)
    {
        if (getPce(indexR, indexC) == NOPIECE)
        {
            sq++;
        }
        else
        {
            break;
        }
        indexR++;
        indexC++;
    }
    //down left direction
    indexR = r - 1;
    indexC = c + 1;
    
    while (indexR >= 0 && indexC <= 7)
    {
        if (getPce(indexR, indexC) == NOPIECE)
        {
            sq++;
        }
        else
        {
            break;
        }
        indexR--;
        indexC++;
    }
    //up right direction
    indexR = r + 1;
    indexC = c - 1;
    
    while (indexR <= 7 && indexC >= 0)
    {
        if (getPce(indexR, indexC) == NOPIECE)
        {
            sq++;
        }
        else
        {
            break;
        }
        indexR++;
        indexC--;
    }
    //up left direction
    indexR = r - 1;
    indexC = c - 1;
    
    while (indexR >= 0 && indexC >= 0)
    {
        if (getPce(indexR, indexC) == NOPIECE)
        {
            sq++;
        }
        else
        {
            break;
        }
        indexR--;
        indexC--;
    }
    
    //Vertical case
    sq += sqExposedVert(r, c);
    
    return sq;
}

int Board::sqExposedVert(int r, int c) const
{
    int sq = 0;
    
    //Loop in 2 directions until we hit a piece or go offboard
    
    int indexR = r - 1;
    int indexC = c;
    //up direction
    while (indexR >= 0)
    {
        if (getPce(indexR, indexC) == NOPIECE)
        {
            sq++;
        }
        else
        {
            break;
        }
        indexR--;
    }
    
    indexR = r + 1;
    indexC = c;
    //down direction
    while (indexR <= 7)
    {
        if (getPce(indexR, indexC) == NOPIECE)
        {
            sq++;
        }
        else
        {
            break;
        }
        indexR++;
    }
    
    return sq;
}

bool Board::hasKcPerm() const
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

bool Board::hasQcPerm() const
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

int Board::getFiftyMove() const
{
    return fiftyMove;
}

void Board::updateFiftyMove()
{
    fiftyMove = hisPly;
}

History Board::getLastState() const
{
    if (hisPly == 0)
    {
        std::cerr << "Attempting to access history with no moves made (Board::getLastMove())" << std::endl;
        exit(1);
    }
    return history[hisPly - 1];
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

void Board::hashInEp(int r, int c)
{
    //Assumes ep is empty
    posKey ^= enpasKey[r][c];
    enpasSquareR = r;
    enpasSquareC = c;
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

void Board::changeSide()
{
    //Changes side and changes pos key
    (side == WHITE) ? (side = BLACK) : (side = WHITE);
    posKey ^= sideKey;
}

void Board::ClearBoard()
{
    //Resetting side to white
    side = WHITE;
    
    //Resetting history
    hisPly = 0;
    fiftyMove = 0;
    
    //Resetting material
    material = 0;
    
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

void Board::printPieces(int pce)
{
    for (int i = 10 * pce; i < 10 * pce + pceNum[pce]; i++)
    {
        std::cout << pListR[i] << ' ' << pListC[i] << std::endl;
    }
}

void Board::printBoard(int side)
{
    if (side == WHITE)
    {
        //Printing out each rank, first the character and then the pieces
        for (int r = 0; r < 8; r++)
        {
            std::cout << RankChar[r] << "   |   ";
            
            for (int c = 0; c < 8; c++)
            {
                std::cout << PceChar[mBoard[r][c]] << "   ";
            }
            std::cout << std::endl << "    |" << std::endl;
        }
        //Divider between board and file letters
        std::cout << "    ----------------------------------" << std::endl;
        std::cout << "        ";
        for (int i = 0; i < 8; i++)
        {
            std::cout << FileChar[i] << "   ";
        }
        
        std::cout << std::endl << std::endl;
    }
    else if (side == BLACK)
    {
        //Printing out each rank, first the character and then the pieces
        for (int r = 7; r >= 0; r--)
        {
            std::cout << RankChar[r] << "   |   ";
            
            for (int c = 7; c >= 0; c--)
            {
                std::cout << PceChar[mBoard[r][c]] << "   ";
            }
            std::cout << std::endl << "    |" << std::endl;
        }
        //Divider between board and file letters
        std::cout << "    ----------------------------------" << std::endl;
        std::cout << "        ";
        for (int i = 7; i >= 0; i--)
        {
            std::cout << FileChar[i] << "   ";
        }
        
        std::cout << std::endl << std::endl;
    }
    else
    {
        std::cerr << "Printing board from an invalid color" << std::endl;
    }
    
    if (debugMode)
    {
        std::cout << "Side to play: ";
        (side == WHITE) ? (std::cout << 'w' << std::endl) : (std::cout << 'b' << std::endl);
        
        std::cout << "En Pas Square: ";
        if (enpasSquareC != OFFBOARD)
        {
            std::cout << (char)((int)'a' + enpasSquareC);
            std::cout << 8 - enpasSquareR;
        }
        std::cout << std::endl;
        std::cout << "Current ply: " << hisPly << std::endl;
        std::cout << "Fifty move ply: " << fiftyMove << std::endl;
        std::cout << std::endl;
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
