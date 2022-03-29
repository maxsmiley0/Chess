#include "board.h"
#include "Movemap.h"
#include <iostream>
#include <fstream>

//gets nth bit of square
#define get_bit(bitboard, square) ((bitboard) & (1ULL << (square)))
#define set_bit(bitboard, square) ((bitboard) |= (1ULL << (square)))
#define pop_bit(bitboard, square) ((bitboard) &= ~(1ULL << (square)))
#define count_bits(bitboard) __builtin_popcountll(bitboard)

#define BP 0
#define BN 1
#define BB 2
#define BR 3
#define BQ 4
#define BK 5
#define WP 6
#define WN 7
#define WB 8
#define WR 9
#define WQ 10
#define WK 11

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


/*
          binary move bits                               hexidecimal constants
    
    0000 0000 0000 0000 0011 1111    source square       0x3f
    0000 0000 0000 1111 1100 0000    target square       0xfc0
    0000 0000 1111 0000 0000 0000    piece               0xf000
    0000 1111 0000 0000 0000 0000    promoted piece      0xf0000
    0001 0000 0000 0000 0000 0000    capture flag        0x100000
    0010 0000 0000 0000 0000 0000    double push flag    0x200000
    0100 0000 0000 0000 0000 0000    enpassant flag      0x400000
    1000 0000 0000 0000 0000 0000    castling flag       0x800000
*/

// encode move
#define encode_move(source, target, piece, promoted, capture, double, enpas, castling) \
    (source) |          \
    (target << 6) |     \
    (piece << 12) |     \
    (promoted << 16) |  \
    (capture << 20) |   \
    (double << 21) |    \
    (enpas << 22) | \
    (castling << 23)    \
    
// extract source square
#define get_move_source(move) (move & 0x3f)

// extract target square
#define get_move_target(move) ((move & 0xfc0) >> 6)

// extract piece
#define get_move_piece(move) ((move & 0xf000) >> 12)

// extract promoted piece
#define get_move_promoted(move) ((move & 0xf0000) >> 16)

// extract capture flag
#define get_move_capture(move) (move & 0x100000)

// extract double pawn push flag
#define get_move_double(move) (move & 0x200000)

// extract enpassant flag
#define get_move_enpas(move) (move & 0x400000)

// extract castling flag
#define get_move_castling(move) (move & 0x800000)

// move list structure
typedef struct {
    // moves
    int moves[256];
    
    // move count
    int count;
} moves;

// add move to the move list
static inline void add_move(moves *move_list, int move)
{
    // strore move
    move_list->moves[move_list->count] = move;
    
    // increment move count
    move_list->count++;
}


/*
                           castling   move     in      in
                              right update     binary  decimal
 king & rooks didn't move:     1111 & 1111  =  1111    15
        white king  moved:     1111 & 1100  =  1100    12
  white king's rook moved:     1111 & 1110  =  1110    14
 white queen's rook moved:     1111 & 1101  =  1101    13
     
         black king moved:     1111 & 0011  =  1011    3
  black king's rook moved:     1111 & 1011  =  1011    11
 black queen's rook moved:     1111 & 0111  =  0111    7
*/

// castling rights update constants
const int castling_rights[64] = {
     7, 15, 15, 15,  3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

template <bool IsWhite>
static constexpr void genmoves(Board brd)
{
    if (IsWhite)
    {
        //how tf do we do check / pinmask
        
        //pawn moves
        //One move aheads
        //map pawns_one_forward = (brd.WPawn >> 8) & ~brd.Occ
        //print_bitboard((brd.WPawn >> 8) & ~brd.Occ);
        //Two move aheads
        //glean prev bitboard & RANK_3
        //print_bitboard((prev) >> 8 & ~brd.Occ);
        
        //(Noncapture) Promotions
        //Captures
        //Enpassants
        //Capture promotions
    }
    else
    {

    }
}

int main()
{
    //indexed by (bishop, king) square
    map bishop[64][64];
    map rook[64][64]; //given piece and king squares, returns a checkmask

    //we will do a piece lookup to see who can see the king, enumerate number of attackers, and if one can & moves with checkmask!
    
    for (int i = 0; i < 64; i++)
    {
        for (int j = 0; j < 64; j++)
        {
            bishop[i][j] = 0;
        }
    }
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 8; c++)
        {
            int b_square = r * 8 + c;
            for (int r1 = 0; r1 < 8; r1++)
            {
                for (int c1 = 0; c1 < 8; c1++)
                {
                    int k_square = r1 * 8 + c1;
                    if (r1 - r == c1 - c)
                    {
                        int tempr = r;
                        int tempc = c;
                        map set = 0ULL;
                        if (r > r1)
                        {
                            while (tempr > r1)
                            {
                                set_bit(set, tempr * 8 + tempc);
                                tempr--;
                                tempc--;
                            }
                        }
                        else if (r < r1)
                        {
                            while (tempr < r1)
                            {
                                set_bit(set, tempr * 8 + tempc);
                                tempr++;
                                tempc++;
                            }
                        }
                        bishop[b_square][k_square] = set;
                    }
                    else if (r1 - r == c - c1)
                    {
                        int tempr = r;
                        int tempc = c;
                        map set = 0ULL;
                        if (r > r1)
                        {
                            while (tempr > r1)
                            {
                                set_bit(set, tempr * 8 + tempc);
                                tempr--;
                                tempc++;
                            }
                        }
                        else if (r < r1)
                        {
                            while (tempr < r1)
                            {
                                set_bit(set, tempr * 8 + tempc);
                                tempr++;
                                tempc--;
                            }
                        }
                        bishop[b_square][k_square] = set;
                    }
                }
            }    
        }
    }
    
    Board brd = parseFen("rn1r2k1/pp2qppp/8/8/3N2n1/1N2P3/P1Q2PPP/R4RK1");
    genmoves<true>(brd);
    int rb = 6;
    int cb = 6;
    int rk = 2;
    int ck = 2;
    print_bitboard(bishop[rb * 8 + cb][rk * 8 + ck]);
    return 0;
}