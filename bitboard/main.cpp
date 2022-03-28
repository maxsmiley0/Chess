#include "board.h"
#include "Movemap.h"
#include <iostream>
#include <fstream>

//gets nth bit of square
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define count_bits(bitboard) __builtin_popcountll(bitboard)

void print_bitboard(map bitboard)
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
                printf("  %d  ", 8 - rank);
            }

            //shifts left to get the nth bit value, prints 1 if hits 0 if misses
            printf(" %d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }

    printf("\n      a  b  c  d  e  f  g  h\n\n");
    printf("      Bitboard: %llud\n\n", bitboard);
}


Board parseFen(std::string fen)
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
                    print_bitboard(bp);
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
    return Board(bp, bn, bb, br, bq, bk, wp, wn, wb, wr, wq, wk);
}

//for ls1b de bruijn multiplication
const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

enum {
    a8, b8, c8, d8, e8, f8, g8, h8,      
    a7, b7, c7, d7, e7, f7, g7, h7,      
    a6, b6, c6, d6, e6, f6, g6, h6,      
    a5, b5, c5, d5, e5, f5, g5, h5,      
    a4, b4, c4, d4, e4, f4, g4, h4,      
    a3, b3, c3, d3, e3, f3, g3, h3,      
    a2, b2, c2, d2, e2, f2, g2, h2,      
    a1, b1, c1, d1, e1, f1, g1, h1, no_sq
};

int main()
{
    Board brd = parseFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
    print_bitboard(brd.WBishop);
    print_bitboard(get_queen_attacks<f6>(brd.WPawn));
    std::ofstream o; //ofstream is the class for fstream package
    o.open("lol"); //open is the method of ofstream
    o << "hi";
    int maxi = 64;
    for (int i = 0; i < maxi; i++)
    {
        o << "{\n";
        
        int maxj = 4096;
        for (int j = 0; j < maxj; j++)
        {
            if (j < maxj - 1)
            {
                o << rook_attacks[i][j] << "ULL, ";
            }
            else 
            {
                o << rook_attacks[i][j] << "ULL";
            }
        }
        if (i < maxi - 1)
        {  
            o << "\n},\n";
        }
        else 
        {
            o << "\n}\n";
        }
    }
    o.close();
    //print_bitboard(EnemyOrEmpty<true>(brd));
    return 0;
}