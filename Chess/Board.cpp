#include <string>

#include "Board.h"

#include "Auxiliary.h"
#include "Queen.h"
#include "Knight.h"
#include "King.h"
#include "Rook.h"
#include "Bishop.h"
#include "Pawn.h"

#include "Piece.h"

#include "Coord.h"
#include "Set.h"

#include <unordered_map>

Board::Board()
{
    turn = 'W';
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            mBoard[i][j] = nullptr;
        }
    }
}

Board::Board(const Board& other)
{
    turn = other.turn;
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (other.mBoard[i][j] == nullptr)
            {
                mBoard[i][j] = nullptr;
            }
            else if (other.mBoard[i][j]->type() == 'N') //creates a knight of given color
            {                      //assumes in the form "WN" "WK" etc "BB"
                mBoard[i][j] = new Knight(this, Coord(i, j), other.mBoard[i][j]->getColor());
            }
            else if (other.mBoard[i][j]->type() == 'K')
            {
                mBoard[i][j] = new King(this, Coord(i, j), other.mBoard[i][j]->getColor());
            }
            else if (other.mBoard[i][j]->type() == 'R')
            {
                mBoard[i][j] = new Rook(this, Coord(i, j), other.mBoard[i][j]->getColor());
            }
            else if (other.mBoard[i][j]->type() == 'B')
            {
                mBoard[i][j] = new Bishop(this, Coord(i, j), other.mBoard[i][j]->getColor());
            }
            else if (other.mBoard[i][j]->type() == 'Q')
            {
                mBoard[i][j] = new Queen(this, Coord(i, j), other.mBoard[i][j]->getColor());
            }
            else if (other.mBoard[i][j]->type() == 'P')
            {
                mBoard[i][j] = new Pawn(this, Coord(i, j), other.mBoard[i][j]->getColor());
            }
            else
            {
                cerr << "Copy constructor fail in Board::Board" << endl;
                exit(1);
            }
            
            if (mBoard[i][j] != nullptr)
            {
                mBoard[i][j]->setMoved(other.mBoard[i][j]->hasMoved());
                mBoard[i][j]->setPinDir(other.mBoard[i][j]->getPinDir());
                mBoard[i][j]->setNumDefending(other.mBoard[i][j]->getNumDefending());
                mBoard[i][j]->setNumAttacking(other.mBoard[i][j]->getNumAttacking());
                mBoard[i][j]->legalMoves = other.mBoard[i][j]->legalMoves;
            }
            if (mBoard[i][j] != nullptr && mBoard[i][j]->getColor() == 'W')
            {
                whitePieces.push_back(mBoard[i][j]);
            }
            else if (mBoard[i][j] != nullptr && mBoard[i][j]->getColor() == 'B')
            {
                blackPieces.push_back(mBoard[i][j]);
            }
        }
    }
}

Board::Board(vector<vector<string>> b)
{
    turn = 'W';
    for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (b[i][j][1] == 'N') //creates a knight of given color
                {                      //assumes in the form "WN" "WK" etc "BB"
                    mBoard[i][j] = new Knight(this, Coord(i, j), b[i][j][0]);
                }
                else if (b[i][j][1] == 'K')
                {
                    mBoard[i][j] = new King(this, Coord(i, j), b[i][j][0]);
                }
                else if (b[i][j][1] == 'R')
                {
                    mBoard[i][j] = new Rook(this, Coord(i, j), b[i][j][0]);
                }
                else if (b[i][j][1] == 'B')
                {
                    mBoard[i][j] = new Bishop(this, Coord(i, j), b[i][j][0]);
                }
                else if (b[i][j][1] == 'Q')
                {
                    mBoard[i][j] = new Queen(this, Coord(i, j), b[i][j][0]);
                }
                else if (b[i][j][1] == 'P')
                {
                    mBoard[i][j] = new Pawn(this, Coord(i, j), b[i][j][0]);
                }
                else
                {
                    mBoard[i][j] = nullptr;
                }
                
                if (b[i][j][0] == 'W')
                {
                    whitePieces.push_back(mBoard[i][j]);
                }
                else if (b[i][j][0] == 'B')
                {
                    blackPieces.push_back(mBoard[i][j]);
                }
            }
        }

    //Updates legal moves
    updateLegalMoves('W');
    updateLegalMoves('B');
}

bool Board::operator==(const Board& other)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mBoard[i][j] == nullptr && other.mBoard[i][j] == nullptr)
            {
                return true;
            }
            else if (mBoard[i][j] == nullptr || other.mBoard[i][j] == nullptr)
            {
                return true;
            }
            else if (*mBoard[i][j] != *other.mBoard[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

Board::~Board()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mBoard[i][j] != nullptr)
            {
                delete mBoard[i][j];
                mBoard[i][j] = nullptr;
            }
        }
    }
}

Piece* Board::getKing(char color)
{
    //assuming each side has at most one king
    if (color == 'W')
    {
        list<Piece*>::iterator itr = whitePieces.begin();
        while (itr != whitePieces.end())
        {
            if ((*itr)->type() == 'K')
            {
                return (*itr);
            }
            itr++;
        }
        print();
        cerr << "No White King!" << endl;
        exit(1);
    }
    else if (color == 'B')
    {
        list<Piece*>::iterator itr = blackPieces.begin();
        while (itr != blackPieces.end())
        {
            if ((*itr)->type() == 'K')
            {
                return (*itr);
            }
            itr++;
        }
        print();
        cerr << "No Black King!" << endl;
        exit(1);
    }
    else
    {
        cerr << "Invalid color specifier in Board::getKing(char)" << endl;
        exit(1);
    }
}

Piece* Board::getPiece(Coord c) const
{
    if (c.getX() >= 0 && c.getX() <= 7 && c.getY() >= 0 && c.getY() <= 7)
    {
        return mBoard[c.getX()][c.getY()];
    }
    return nullptr;
}

void Board::addPiece(Piece* p)
{
    mBoard[p->getPos().getX()][p->getPos().getY()] = p;
}

void Board::moveRaw(Piece* p, Coord c)
{
    
    if (p->type() == 'K' && !p->hasMoved() && c == Coord(7, 2))
    {
        //manually moving rook
        mBoard[7][3] = mBoard[7][0];
        mBoard[7][3]->setPos(Coord(7, 3));
        mBoard[7][0] = nullptr;
    }
    else if (p->type() == 'K' && !p->hasMoved() && c == Coord(7, 6))
    {
        //manually moving rook
        mBoard[7][5] = mBoard[7][7];
        mBoard[7][5]->setPos(Coord(7, 5));
        mBoard[7][7] = nullptr;
    }
    else if (p->type() == 'K' && !p->hasMoved() && c == Coord(0, 2))
    {
        //manually moving rook
        mBoard[0][3] = mBoard[0][0];
        mBoard[0][3]->setPos(Coord(0, 3));
        mBoard[0][0] = nullptr;
        }
    else if (p->type() == 'K' && !p->hasMoved() && c == Coord(0, 6))
    {
        //manually moving rook
        mBoard[0][5] = mBoard[0][7];
        mBoard[0][5]->setPos(Coord(0, 5));
        mBoard[0][7] = nullptr;
    }
        
    if (this->getPiece(c) != nullptr) //if space is occupied
    {
        Piece* takenPiece = mBoard[c.getX()][c.getY()];
        mBoard[c.getX()][c.getY()] = p;
        mBoard[p->getPos().getX()][p->getPos().getY()] = nullptr;
        
        //removing the takenpiece from the piece list
        //what if runs through end w/o finding piece?
        if (takenPiece->getColor() == 'W')
        {
            for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end();itr++)
            {
                if ((*itr) == takenPiece)
                {
                    itr = whitePieces.erase(itr);
                    break;
                }
            }
        }
        else
        {
            for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
            {
                if ((*itr) == takenPiece)
                {
                    itr = blackPieces.erase(itr);
                    break;
                }
            }
        }
        delete takenPiece;
    }
    else //if space is empty
    {
        mBoard[c.getX()][c.getY()] = p;
        mBoard[p->getPos().getX()][p->getPos().getY()] = nullptr;
    }
        
    //Promo check
        
    if (p->type() == 'P')
    {
        if (c.getX() == 0) //case white queen, perhaps change later
        {
            for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
            {
                if ((*itr) == p)
                {
                    //Creating the new Piece and saving the old one
                    Piece* switchedPiece = p;
                    p = new Queen(this, c, 'W');
                    p->setMoved(true);
                    
                    //Erasing the iterator from the list and deleting it
                    itr = whitePieces.erase(itr);
                    delete switchedPiece;
                    
                    //Adding the new piece to whitePieces and mBoard
                    mBoard[c.getX()][c.getY()] = p;
                    whitePieces.push_back(p);
                    break;
                }
            }
        }
        else if (c.getX() == 7) //case black queen, perhaps change later
        {
            for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
            {
                if ((*itr) == p)
                {
                    //Creating the new Piece and saving the old one
                    Piece* switchedPiece = p;
                    p = new Queen(this, c, 'B');
                    p->setMoved(true);
                    
                    //Erasing the iterator from the list and deleting it
                    itr = blackPieces.erase(itr);
                    delete switchedPiece;
                    
                    //Adding the new piece to whitePieces and mBoard
                    mBoard[c.getX()][c.getY()] = p;
                    blackPieces.push_back(p);
                    break;
                }
            }
        }
    }
    
    p->setPos(c);
    p->setMoved(true);
    
    //Updates pin information
    updatePinDir(p->getOppositeColor());
    //Updates which pieces are defended / attacked by which
    p->getBoard()->update();
}

void Board::movePiece(Piece* p, Coord c)
{
    if (p == nullptr)
    {
        cerr << "Trying to move a piece not at that position" << endl;
        exit(1);
    }
    //castling clause
    if (containsCoord(p->legalMoves, c)) //if this coord is in the legal move set
    {
        if (p->type() == 'K' && !p->hasMoved() && c == Coord(7, 2))
        {
            //manually moving rook
            mBoard[7][3] = mBoard[7][0];
            mBoard[7][3]->setPos(Coord(7, 3));
            mBoard[7][3]->setMoved(true);
            mBoard[7][0] = nullptr;
        }
        else if (p->type() == 'K' && !p->hasMoved() && c == Coord(7, 6))
        {
            //manually moving rook
            mBoard[7][5] = mBoard[7][7];
            mBoard[7][5]->setPos(Coord(7, 5));
            mBoard[7][5]->setMoved(true);
            mBoard[7][7] = nullptr;
        }
        else if (p->type() == 'K' && !p->hasMoved() && c == Coord(0, 2))
        {
            //manually moving rook
            mBoard[0][3] = mBoard[0][0];
            mBoard[0][3]->setPos(Coord(0, 3));
            mBoard[0][3]->setMoved(true);
            mBoard[0][0] = nullptr;
        }
        else if (p->type() == 'K' && !p->hasMoved() && c == Coord(0, 6))
        {
            //manually moving rook
            mBoard[0][5] = mBoard[0][7];
            mBoard[0][5]->setPos(Coord(0, 5));
            mBoard[0][5]->setMoved(true);
            mBoard[0][7] = nullptr;
        }
        
        if (this->getPiece(c) != nullptr) //if space is occupied
        {
            Piece* takenPiece = mBoard[c.getX()][c.getY()];
            mBoard[c.getX()][c.getY()] = p;
            mBoard[p->getPos().getX()][p->getPos().getY()] = nullptr;
            
            //removing the takenpiece from the piece list
            //what if runs through end w/o finding piece?
            if (takenPiece->getColor() == 'W')
            {
                for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
                {
                    if ((*itr) == takenPiece)
                    {
                        itr = whitePieces.erase(itr);
                        break;
                    }
                }
            }
            else
            {
                for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
                {
                    if ((*itr) == takenPiece)
                    {
                        itr = blackPieces.erase(itr);
                        break;
                    }
                }
            }
            delete takenPiece;
        }
        else //if space is empty
        {
            mBoard[c.getX()][c.getY()] = p;
            mBoard[p->getPos().getX()][p->getPos().getY()] = nullptr;
        }
        
        //Promo check
        
        if (p->type() == 'P')
        {
            if (c.getX() == 0) //case white queen, perhaps change later
            {
                for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
                {
                    if ((*itr) == p)
                    {
                        //Creating the new Piece and saving the old one
                        Piece* switchedPiece = p;
                        p = new Queen(this, c, 'W');
                        p->setMoved(true);
                        
                        //Erasing the iterator from the list and deleting it
                        itr = whitePieces.erase(itr);
                        delete switchedPiece;
                        
                        //Adding the new piece to whitePieces and mBoard
                        mBoard[c.getX()][c.getY()] = p;
                        whitePieces.push_back(p);
                        break;
                    }
                }
            }
            else if (c.getX() == 7) //case black queen, perhaps change later
            {
                for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
                {
                    if ((*itr) == p)
                    {
                        //Creating the new Piece and saving the old one
                        Piece* switchedPiece = p;
                        p = new Queen(this, c, 'B');
                        p->setMoved(true);
                        
                        //Erasing the iterator from the list and deleting it
                        itr = blackPieces.erase(itr);
                        delete switchedPiece;
                        
                        //Adding the new piece to whitePieces and mBoard
                        mBoard[c.getX()][c.getY()] = p;
                        blackPieces.push_back(p);
                        break;
                    }
                }
            }
        }
        
        //Promo check end
        
        p->setPos(c);
        p->setMoved(true);
        //Updates pin information
        updatePinDir(p->getOppositeColor());
        //Updates legal moves
        //Updates which pieces are defended / attacked by which
        update();
        updateLegalMoves('W');
        updateLegalMoves('B');
    }
    else //Crashes on illegal move atm
    {
        cerr << "This is not a legal move!" << endl;
        exit(1);
    }
}
 
void Board::updateLegalMoves(char c)
{
    if (c == 'W')
    {
        for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
        {
            (*itr)->updateLegalMoves();
        }
    }
    else
    {
        for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
        {
            (*itr)->updateLegalMoves();
        }
    }
}

list<Piece*> Board::canReachKingPseudo(char color) //color of king
{
    list<Piece*> li;
    
    if (color == 'W')
    {
        for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
        {
            if ((*itr)->canReachEnemyKing())
            {
                li.push_back(*itr);
            }
        }
    }
    else if (color == 'B')
    {
        for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
        {
            if ((*itr)->canReachEnemyKing())
            {
                li.push_back(*itr);
            }
        }
    }
    
    return li;
}

list<Piece*> Board::canReachCoordPseudo(Coord c, char color)
{
    list<Piece*> li;
    
    if (color == 'W')
    {
        for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
        {
            if ((*itr)->pseudoLegalMoves().contains(c))
            {
                li.push_back(*itr);
            }
        }
    }
    else if (color == 'B')
    {
        for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
        {
            if ((*itr)->pseudoLegalMoves().contains(c))
            {
                li.push_back(*itr);
            }
        }
    }
    
    return li;
}

bool Board::boolCanReachCoordPseudo(Coord c, char color)
{
    if (color == 'W')
    {
        for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
        {
            if ((*itr)->pseudoLegalMoves().contains(c))
            {
                return true;
            }
        }
    }
    else if (color == 'B')
    {
        for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
        {
            if ((*itr)->pseudoLegalMoves().contains(c))
            {
                return true;
            }
        }
    }
    
    return false;
}

void Board::updatePinDir(char color)
{
    char c = 'N';
    
    /*
     defaulting tags so we can recheck
     */
    if (color == 'W')
    {
        for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
        {
            (*itr)->setPinDir('N');
        }
    }
    else
    {
        for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
        {
            (*itr)->setPinDir('N');
        }
    }
    
    if (color == 'W')
    {
        /*
         iterate through whitePieces list and get rays of queen, bishops, and rooks
         get pinned pieces
         Need to set everything to default
         */
        for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
        {
            Piece* p = getPinned(*itr, c);
            if (p != nullptr)
            {
                p->setPinDir(c);
            }
        }
    }
    else if (color == 'B')
    {
        /*
         iterate through whitePieces list and get rays of queen, bishops, and rooks
         get pinned pieces
         Need to set everything to default
         */
        for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
        {
            Piece* p = getPinned(*itr, c);
            if (p != nullptr)
            {
                p->setPinDir(c);
            }
        }
    }
    else
    {
        cerr << "Invalid color specifier in Board::updatePinDir(char)" << endl;
        exit(1);
    }
}

void Board::update()
{
    for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
    {
        (*itr)->attackers.clear();
        (*itr)->defenders.clear();
    }
    for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
    {
        (*itr)->attackers.clear();
        (*itr)->defenders.clear();
    }
    for (list<Piece*>::iterator itr = whitePieces.begin(); itr != whitePieces.end(); itr++)
    {
        (*itr)->update();
    }
    for (list<Piece*>::iterator itr = blackPieces.begin(); itr != blackPieces.end(); itr++)
    {
        (*itr)->update();
    }
}

void Board::print()
{
    for (int i = 0; i < 8; i++)
    {
        cout << 8 - i << " "; //number coordinates
        for (int j = 0; j < 8; j++)
        {
            if (mBoard[i][j] != nullptr)
            {
                cout << ' ' << mBoard[i][j]->getColor() << mBoard[i][j]->type() << ' ';
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

char Board::getTurn()
{
    return turn;
}

void Board::nextTurn()
{
    if (turn == 'W')
    {
        turn = 'B';
    }
    else
    {
        turn = 'W';
    }
}

unsigned long Board::hashmap()
{
    hash<string> hasher;
    string s = "";
    s += turn;
    //A linear combination of the worth, x, and y. We add 1 so any x = y = worth = 0 dependancies dont kill the equation to zero
    
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mBoard[i][j] != nullptr)
            {
                s += mBoard[i][j]->getColor() + mBoard[i][j]->type();
            }
            else
            {
                s += 'N';
            }
        }
    }
    
    return hasher(s) % HASHCOUNT;
}
