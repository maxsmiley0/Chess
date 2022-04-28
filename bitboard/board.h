#ifndef board_h
#define board_h

#include <list>
#include "defs.h"
#include "Movemap.h"

enum {wk = 1, wq = 2, bk = 4, bq = 8};

struct Brd {
    //Pieces
    map pce[12];    //piece map, indexed by piece type
    map side[2];    //side map, indexed by side
    map occ;        //all occupancies

    //State
    int color;
    int enpas;
    int castleperms;
};

//Returns true if the side to move is in check
static inline bool in_check(Brd brd)
{
    map occ = brd.occ;
    int k_square;

    if (brd.color == WHITE)
    {
        k_square = get_ls1b_index(brd.pce[WK]);
        return lookup(BQ, k_square, occ) & brd.pce[BQ] || lookup(BR, k_square, occ) & brd.pce[BR] || lookup(BB, k_square, occ) & brd.pce[BB] || 
               lookup(BN, k_square, occ) & brd.pce[BN] || lookup(WP, k_square, occ) & brd.pce[BP] || lookup(BK, k_square, occ) & brd.pce[BK];
    }
    else 
    {
        k_square = get_ls1b_index(brd.pce[BK]);
        return lookup(WQ, k_square, occ) & brd.pce[WQ] || lookup(WR, k_square, occ) & brd.pce[WR] || lookup(WB, k_square, occ) & brd.pce[WB] || 
               lookup(WN, k_square, occ) & brd.pce[WN] || lookup(BP, k_square, occ) & brd.pce[WP] || lookup(WK, k_square, occ) & brd.pce[WK];
    }
    
    return false;
}

//Given a square return character of piece on it
char pce_on_square(int square, Brd brd)
{
    map sq = 0ULL;
    set_bit(sq, square);

    for (int pce = WP; pce <= BK; pce++)
    {
        if (sq & brd.pce[pce])
        {
            return pce_char[pce];
        }
    }

    return '.';
}

Brd parse_fen(std::string fen)
{
    int section = 0;    //0 -> pieces | 1 -> side | 2 -> castle | 3 -> enpas square
    int r = 0;          //stores row we are on
    int c = 0;          //stores column we are on

    map bp = 0, bn = 0, bb = 0, br = 0, bq = 0, bk = 0, wp = 0, wn = 0, wb = 0, wr = 0, wq = 0, wk = 0;
    
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
                    set_bit(bp, r * 8 + c);
                    c++;
                    break;
                case 'n':
                    set_bit(bn, r * 8 + c);
                    c++;
                    break;
                case 'b':
                    set_bit(bb, r * 8 + c);
                    c++;
                    break;
                case 'r':
                    set_bit(br, r * 8 + c);
                    c++;
                    break;
                case 'q':
                    set_bit(bq, r * 8 + c);
                    c++;
                    break;
                case 'k':
                    set_bit(bk, r * 8 + c);
                    c++;
                    break;
                case 'P':
                    set_bit(wp, r * 8 + c);
                    c++;
                    break;
                case 'N':
                    set_bit(wn, r * 8 + c);
                    c++;
                    break;
                case 'B':
                    set_bit(wb, r * 8 + c);
                    c++;
                    break;
                case 'R':
                    set_bit(wr, r * 8 + c);
                    c++;
                    break;
                case 'Q':
                    set_bit(wq, r * 8 + c);
                    c++;
                    break;
                case 'K':
                    set_bit(wk, r * 8 + c);
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
        /*
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
        }*/
    }

    Brd brd;

    brd.pce[WP] = wp;
    brd.pce[WN] = wn;
    brd.pce[WB] = wb;
    brd.pce[WR] = wr;
    brd.pce[WQ] = wq;
    brd.pce[WK] = wk;

    brd.pce[BP] = bp;
    brd.pce[BN] = bn;
    brd.pce[BB] = bb;
    brd.pce[BR] = br;
    brd.pce[BQ] = bq;
    brd.pce[BK] = bk;

    brd.side[WHITE] = wp | wn | wb | wr | wq | wk;
    brd.side[BLACK] = bp | bn | bb | br | bq | bk;
    brd.occ = brd.side[WHITE] | brd.side[BLACK];

    brd.color = WHITE;

    return brd;
}

void print_board(Brd brd)
{
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            //Convert file & rank into square index
            int square = rank * 8 + file;

            //print ranks
            if (!file)
            {
                std::cout << "  " << 8 - rank << "  ";
            }

            //shifts left to get the nth bit value, prints 1 if hits 0 if misses
            std::cout << " ";
            std::cout << pce_on_square(square, brd);
            //get_bit(bitboard, square) ? std::cout << 1 : std::cout << 0;
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "      a  b  c  d  e  f  g  h" << std::endl << std::endl;
    brd.color ? std::cout << "      Color: Black" : std::cout << "      Color: White" << std::endl << std::endl;
}

void print_move(int move)
{
    std::cout << "From: " << get_move_source(move) << std::endl;
    std::cout << "To: " << get_move_target(move) << std::endl;
    std::cout << "Piece: " << get_move_piece(move) << std::endl;
}

std::list<int> generate_moves(Brd brd)
{
    std::list<int> li;
    li.push_back(2);
    //Generate moves here...

    for (std::list<int>::iterator itr = li.begin(); itr != li.end(); itr++)
    {
        print_move(*itr);
    }
    return li;
}

#endif