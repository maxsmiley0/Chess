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

//Returns true if square is under attack
static inline bool is_sq_atk(Brd brd, int square, int color)
{
    map occ = brd.occ;
    if (color == WHITE)
    {
        return lookup(BQ, square, occ) & brd.pce[BQ] || lookup(BR, square, occ) & brd.pce[BR] || lookup(BB, square, occ) & brd.pce[BB] || 
               lookup(BN, square, occ) & brd.pce[BN] || lookup(WP, square, occ) & brd.pce[BP] || lookup(BK, square, occ) & brd.pce[BK];
    }
    else 
    {
        return lookup(WQ, square, occ) & brd.pce[WQ] || lookup(WR, square, occ) & brd.pce[WR] || lookup(WB, square, occ) & brd.pce[WB] || 
               lookup(WN, square, occ) & brd.pce[WN] || lookup(BP, square, occ) & brd.pce[WP] || lookup(WK, square, occ) & brd.pce[WK];
    }

    return false;
}

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
    Brd brd;

    int castlePerms = 0;
    int enpas = 0;
    
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
        else if (section == 1)
        {
            if (fen[i] == 'w')
            {
                brd.color = WHITE;
                section++;
                i++;
            }
            else if (fen[i] == 'b')
            {
                brd.color = BLACK;
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
                    castlePerms |= WKCA;
                    break;
                case 'Q':
                    castlePerms |= WQCA;
                    break;
                case 'k':
                    castlePerms |= BKCA;
                    break;
                case 'q':
                    castlePerms |= BQCA;
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
                enpas = (8 - ((int)fen[i + 1] - (int)'0')) * 8 + (int)fen[i] - (int)'a';
            }
            section++;
        }
    }

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

    brd.enpas = enpas;
    brd.castleperms = castlePerms;

    return brd;
}

void print_board(Brd brd)
{
    //Prints ranks, pieces, files
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

    //Prints auxiliary information
    std::cout << std::endl << "      a  b  c  d  e  f  g  h" << std::endl << std::endl;
    brd.color ? std::cout << "      Color: Black" : std::cout << "      Color: White" << std::endl;
    std::cout << "Castle Perms: ";
    if (brd.castleperms & WKCA)
    {
        std::cout << "K";
    }
    if (brd.castleperms & WQCA)
    {
        std::cout << "Q";
    }
    if (brd.castleperms & BKCA)
    {
        std::cout << "k";
    }
    if (brd.castleperms & BQCA)
    {
        std::cout << "q";
    }
    std::cout << std::endl;
    std::cout << "Enpas: " << brd.enpas << std::endl << std::endl;
}

void print_move(int move)
{
    std::cout << "From: " << square_to_coordinates[get_move_source(move)] << std::endl;
    std::cout << "To: " << square_to_coordinates[get_move_target(move)] << std::endl;
    std::cout << "Piece: " << pce_char[get_move_piece(move)] << std::endl;
}

//Given a board, returns a list of generated (pseudo-legal) moves in the position
std::list<int> generate_moves(Brd brd)
{
    std::list<int> li;

    //Pce current / future position
    int src_sq, tar_sq;
    //Current piece map copy and its attack squares
    map bitboard, attacks;
    //Loop through WP->WK / BP->BK depending on side
    int start_pce, end_pce;
    if (brd.color == WHITE)
    {
        start_pce = WP; 
        end_pce = WK;
    }
    else 
    {
        start_pce = BP; 
        end_pce = BK;
    }

    //Loop through all piece bitboards and all attacks
    for (int pce = start_pce; pce <= end_pce; pce++)
    {
        bitboard = brd.pce[pce];
        
        //Pawn & castling moves
        if (pce == WP)
        {
            //Loop over all WP
            while (bitboard)
            {
                src_sq = get_ls1b_index(bitboard);
                tar_sq = src_sq - 8;

                //generate quiet pawn moves
                if (!(tar_sq < a8) && !get_bit(brd.occ, tar_sq))
                {
                    //pawn promotion
                    if (src_sq >= a7 && src_sq <= h7)
                    {
                        li.push_back(encode_move(src_sq, tar_sq, pce, WQ, 0, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, WR, 0, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, WB, 0, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, WN, 0, 0, 0, 0));
                    }
                    else 
                    {
                        //One ahead
                        li.push_back(encode_move(src_sq, tar_sq, pce, 0, 0, 0, 0, 0));
                        //Two ahead
                        if ((src_sq >= a2 && src_sq <= h2) && !get_bit(brd.occ, tar_sq - 8))
                        {
                            li.push_back(encode_move(src_sq, tar_sq - 8, pce, 0, 0, 1, 0, 0));
                        }
                    }
                }
                //generate attack pawn moves
                attacks = pawn_attacks[WHITE][src_sq] & brd.side[BLACK];
                while (attacks)
                {
                    tar_sq = get_ls1b_index(attacks);

                    //Pawn promotion / capture
                    if (src_sq >= a7 && src_sq <= h7)
                    {
                        li.push_back(encode_move(src_sq, tar_sq, pce, WQ, 1, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, WR, 1, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, WB, 1, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, WN, 1, 0, 0, 0));
                    }
                    else 
                    {
                        //Regular capture case
                        li.push_back(encode_move(src_sq, tar_sq, pce, 0, 1, 0, 0, 0));  
                    }

                    pop_bit(attacks, tar_sq);
                }

                //Enpas
                if (brd.enpas)
                {
                    map enpas_attack = pawn_attacks[WHITE][src_sq] & (1ULL << brd.enpas);
                    if (enpas_attack)
                    {
                        li.push_back(encode_move(src_sq, get_ls1b_index(enpas_attack), pce, 0, 1, 0, 1, 0));
                    }
                }

                pop_bit(bitboard, src_sq);
            }
        }

        else if (pce == BP)
        {
            //Loop over all WP
            while (bitboard)
            {
                src_sq = get_ls1b_index(bitboard);
                tar_sq = src_sq + 8;

                //generate quiet pawn moves
                if (!(tar_sq > h1) && !get_bit(brd.occ, tar_sq))
                {
                    //pawn promotion
                    if (src_sq >= a2 && src_sq <= h2)
                    {
                        li.push_back(encode_move(src_sq, tar_sq, pce, BQ, 0, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, BR, 0, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, BB, 0, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, BN, 0, 0, 0, 0));
                    }
                    else 
                    {
                        //One ahead
                        li.push_back(encode_move(src_sq, tar_sq, pce, 0, 0, 0, 0, 0));
                        //Two ahead
                        if ((src_sq >= a7 && src_sq <= h7) && !get_bit(brd.occ, tar_sq + 8))
                        {
                            li.push_back(encode_move(src_sq, tar_sq + 8, pce, 0, 0, 1, 0, 0));
                        }
                    }
                }
                //generate attack pawn moves
                attacks = pawn_attacks[BLACK][src_sq] & brd.side[WHITE];
                while (attacks)
                {
                    tar_sq = get_ls1b_index(attacks);

                    //Pawn promotion / capture
                    if (src_sq >= a2 && src_sq <= h2)
                    {
                        li.push_back(encode_move(src_sq, tar_sq, pce, BQ, 1, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, BR, 1, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, BB, 1, 0, 0, 0));
                        li.push_back(encode_move(src_sq, tar_sq, pce, BN, 1, 0, 0, 0));
                    }
                    else 
                    {
                        //Regular capture case
                        li.push_back(encode_move(src_sq, tar_sq, pce, 0, 1, 0, 0, 0));  
                    }

                    pop_bit(attacks, tar_sq);
                }

                //Enpas
                if (brd.enpas)
                {
                    map enpas_attack = pawn_attacks[BLACK][src_sq] & (1ULL << brd.enpas);
                    if (enpas_attack)
                    {
                        li.push_back(encode_move(src_sq, get_ls1b_index(enpas_attack), pce, 0, 1, 0, 1, 0));
                    }
                }

                pop_bit(bitboard, src_sq);
            }
        }

        else if (pce == WK)
        {
            if (brd.castleperms & WKCA && !get_bit(brd.occ, f1) && !get_bit(brd.occ, g1) && !is_sq_atk(brd, e1, WHITE) && !is_sq_atk(brd, f1, WHITE))
            {
                li.push_back(encode_move(e1, g1, pce, 0, 0, 0, 0, 1));
            }
            if (brd.castleperms & WQCA && !get_bit(brd.occ, d1) && !get_bit(brd.occ, c1) && !get_bit(brd.occ, b1) && !is_sq_atk(brd, e1, WHITE) && !is_sq_atk(brd, d1, WHITE))
            {
                li.push_back(encode_move(e1, c1, pce, 0, 0, 0, 0, 1));
            }
        }

        else if (pce == BK)
        {
            if (brd.castleperms & BKCA && !get_bit(brd.occ, f8) && !get_bit(brd.occ, g8) && !is_sq_atk(brd, e8, WHITE) && !is_sq_atk(brd, f8, WHITE))
            {
                li.push_back(encode_move(e8, g8, pce, 0, 0, 0, 0, 1));
            }
            if (brd.castleperms & BQCA && !get_bit(brd.occ, d8) && !get_bit(brd.occ, c8) && !get_bit(brd.occ, b8) && !is_sq_atk(brd, e8, WHITE) && !is_sq_atk(brd, d8, WHITE))
            {
                li.push_back(encode_move(e8, c8, pce, 0, 0, 0, 0, 1));
            }
        }

        //All other moves
        if (pce == WN || pce == )
    }

    for (std::list<int>::iterator itr = li.begin(); itr != li.end(); itr++)
    {
        print_move(*itr);
    }
    return li;
}

#endif