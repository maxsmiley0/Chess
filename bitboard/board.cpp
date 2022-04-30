#include "board.h"
#include "Movemap.h"

//Returns true if square is under attack
static inline bool is_sq_atk(Brd brd, int square, int color)
{
    map occ = brd.occ;
    if (color == WHITE)
    {
        return get_queen_attacks(square, occ) & brd.pce[BQ] || get_rook_attacks(square, occ) & brd.pce[BR] || get_bishop_attacks(square, occ) & brd.pce[BB] || 
               knight_attacks[square] & brd.pce[BN] || pawn_attacks[WHITE][square] & brd.pce[BP] || king_attacks[square] & brd.pce[BK];
    }
    else 
    {
        return get_queen_attacks(square, occ) & brd.pce[WQ] || get_rook_attacks(square, occ) & brd.pce[WR] || get_bishop_attacks(square, occ) & brd.pce[WB] || 
               knight_attacks[square] & brd.pce[WN] || pawn_attacks[BLACK][square] & brd.pce[WP] || king_attacks[square] & brd.pce[WK];
    }

    return false;
}

//Returns true if the side to move is in check
static inline bool in_check(Brd brd)
{
    map occ = brd.occ;
    int square;

    if (brd.color == WHITE)
    {
        square = get_ls1b_index(WK);
        return get_queen_attacks(square, occ) & brd.pce[BQ] || get_rook_attacks(square, occ) & brd.pce[BR] || get_bishop_attacks(square, occ) & brd.pce[BB] || 
               knight_attacks[square] & brd.pce[BN] || pawn_attacks[WHITE][square] & brd.pce[BP] || king_attacks[square] & brd.pce[BK];
    }
    else 
    {
        square = get_ls1b_index(BK);
        return get_queen_attacks(square, occ) & brd.pce[WQ] || get_rook_attacks(square, occ) & brd.pce[WR] || get_bishop_attacks(square, occ) & brd.pce[WB] || 
               knight_attacks[square] & brd.pce[WN] || pawn_attacks[BLACK][square] & brd.pce[WP] || king_attacks[square] & brd.pce[WK];
    }
    
    return false;
}

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

//Given a fen string, returns a board loaded to that fen
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

//Given a board, returns a list of generated (pseudo-legal) moves in the position
std::list<int> generate_moves(Brd brd)
{
    std::list<int> li;
    
    //Pce current / future position
    int src_sq, tar_sq;
    //Current piece map copy and its attack squares
    map bitboard, attacks;
    
    if (brd.color == WHITE)
    {
        //White pawns
        bitboard = brd.pce[WP];
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
                    li.push_back(encode_move(src_sq, tar_sq, WP, WQ, 0, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, WP, WR, 0, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, WP, WB, 0, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, WP, WN, 0, 0, 0, 0));
                }
                else 
                {
                    //One ahead
                    li.push_back(encode_move(src_sq, tar_sq, WP, 0, 0, 0, 0, 0));
                    //Two ahead
                    if ((src_sq >= a2 && src_sq <= h2) && !get_bit(brd.occ, tar_sq - 8))
                    {
                        li.push_back(encode_move(src_sq, tar_sq - 8, WP, 0, 0, 1, 0, 0));
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
                    li.push_back(encode_move(src_sq, tar_sq, WP, WQ, 1, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, WP, WR, 1, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, WP, WB, 1, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, WP, WN, 1, 0, 0, 0));
                }
                else 
                {
                    //Regular capture case
                    li.push_back(encode_move(src_sq, tar_sq, WP, 0, 1, 0, 0, 0));  
                }

                pop_bit(attacks, tar_sq);
            }

            //Enpas
            if (brd.enpas)
            {
                map enpas_attack = pawn_attacks[WHITE][src_sq] & (1ULL << brd.enpas);
                if (enpas_attack)
                {
                    li.push_back(encode_move(src_sq, get_ls1b_index(enpas_attack), WP, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, src_sq);
        }

        //White king
        //Castle
        if (brd.castleperms & WKCA && !get_bit(brd.occ, f1) && !get_bit(brd.occ, g1) && !is_sq_atk(brd, e1, WHITE) && !is_sq_atk(brd, f1, WHITE))
        {
            li.push_back(encode_move(e1, g1, WK, 0, 0, 0, 0, 1));
        }
        if (brd.castleperms & WQCA && !get_bit(brd.occ, d1) && !get_bit(brd.occ, c1) && !get_bit(brd.occ, b1) && !is_sq_atk(brd, e1, WHITE) && !is_sq_atk(brd, d1, WHITE))
        {
            li.push_back(encode_move(e1, c1, WK, 0, 0, 0, 0, 1));
        }
        //Regular move
        src_sq = get_ls1b_index(brd.pce[WK]);
        attacks = king_attacks[src_sq] & ~brd.side[WHITE];
        //Loop over all WK attacks
        while (attacks)
        {
            tar_sq = get_ls1b_index(attacks);
            //Quiet
            if (!get_bit(brd.side[BLACK], tar_sq))
            {
                li.push_back(encode_move(src_sq, tar_sq, WK, 0, 0, 0, 0, 0));
            }
            //Capture
            else 
            {
                li.push_back(encode_move(src_sq, tar_sq, WK, 0, 1, 0, 0, 0));
            }
            pop_bit(attacks, tar_sq);
        }

        //Knight moves
        bitboard = brd.pce[WN];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = knight_attacks[src_sq] & ~brd.side[WHITE];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[BLACK], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, WN, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, WN, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }

        //Bishop moves
        bitboard = brd.pce[WB];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = get_bishop_attacks(src_sq, brd.occ) & ~brd.side[WHITE];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[BLACK], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, WB, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, WB, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }

        //Rook moves
        bitboard = brd.pce[WR];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = get_rook_attacks(src_sq, brd.occ) & ~brd.side[WHITE];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[BLACK], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, WR, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, WR, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }

        //Queen moves
        bitboard = brd.pce[WQ];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = get_queen_attacks(src_sq, brd.occ) & ~brd.side[WHITE];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[BLACK], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, WQ, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, WQ, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }
    }
    else 
    {
        //Black pawns
        bitboard = brd.pce[BP];
        //Loop over all BP
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
                    li.push_back(encode_move(src_sq, tar_sq, BP, BQ, 0, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, BP, BR, 0, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, BP, BB, 0, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, BP, BN, 0, 0, 0, 0));
                }
                else 
                {
                    //One ahead
                    li.push_back(encode_move(src_sq, tar_sq, BP, 0, 0, 0, 0, 0));
                    //Two ahead
                    if ((src_sq >= a7 && src_sq <= h7) && !get_bit(brd.occ, tar_sq + 8))
                    {
                        li.push_back(encode_move(src_sq, tar_sq + 8, BP, 0, 0, 1, 0, 0));
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
                    li.push_back(encode_move(src_sq, tar_sq, BP, BQ, 1, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, BP, BR, 1, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, BP, BB, 1, 0, 0, 0));
                    li.push_back(encode_move(src_sq, tar_sq, BP, BN, 1, 0, 0, 0));
                }
                else 
                {
                    //Regular capture case
                    li.push_back(encode_move(src_sq, tar_sq, BP, 0, 1, 0, 0, 0));  
                }

                pop_bit(attacks, tar_sq);
            }

            //Enpas
            if (brd.enpas)
            {
                map enpas_attack = pawn_attacks[BLACK][src_sq] & (1ULL << brd.enpas);
                if (enpas_attack)
                {
                    li.push_back(encode_move(src_sq, get_ls1b_index(enpas_attack), BP, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, src_sq);
        }

        //Black king
        //Castle
        if (brd.castleperms & BKCA && !get_bit(brd.occ, f8) && !get_bit(brd.occ, g8) && !is_sq_atk(brd, e8, WHITE) && !is_sq_atk(brd, f8, WHITE))
        {
            li.push_back(encode_move(e8, g8, BK, 0, 0, 0, 0, 1));
        }
        if (brd.castleperms & BQCA && !get_bit(brd.occ, d8) && !get_bit(brd.occ, c8) && !get_bit(brd.occ, b8) && !is_sq_atk(brd, e8, WHITE) && !is_sq_atk(brd, d8, WHITE))
        {
            li.push_back(encode_move(e8, c8, BK, 0, 0, 0, 0, 1));
        }
        //Regular move
        src_sq = get_ls1b_index(brd.pce[BK]);
        attacks = king_attacks[src_sq] & ~brd.side[BLACK];
        //Loop over all BK attacks
        while (attacks)
        {
            tar_sq = get_ls1b_index(attacks);
            //Quiet
            if (!get_bit(brd.side[WHITE], tar_sq))
            {
                li.push_back(encode_move(src_sq, tar_sq, BK, 0, 0, 0, 0, 0));
            }
            //Capture
            else 
            {
                li.push_back(encode_move(src_sq, tar_sq, BK, 0, 1, 0, 0, 0));
            }
            pop_bit(attacks, tar_sq);
        }

        //Knight moves
        bitboard = brd.pce[BN];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = knight_attacks[src_sq] & ~brd.side[BLACK];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[WHITE], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, BN, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, BN, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }

        //Bishop moves
        bitboard = brd.pce[BB];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = get_bishop_attacks(src_sq, brd.occ) & ~brd.side[BLACK];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[WHITE], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, BB, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, BB, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }

        //Rook moves
        bitboard = brd.pce[BR];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = get_rook_attacks(src_sq, brd.occ) & ~brd.side[BLACK];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[WHITE], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, BR, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, BR, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }

        //Queen moves
        bitboard = brd.pce[BQ];
        while (bitboard)
        {
            src_sq = get_ls1b_index(bitboard);
            
            attacks = get_queen_attacks(src_sq, brd.occ) & ~brd.side[BLACK];
            
            while (attacks)
            {
                tar_sq = get_ls1b_index(attacks);
                //Quiet
                if (!get_bit(brd.side[WHITE], tar_sq))
                {
                    li.push_back(encode_move(src_sq, tar_sq, BQ, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    li.push_back(encode_move(src_sq, tar_sq, BQ, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }
    }

    return li;
}

Brd copy_board(Brd& brd)
{
    Brd new_brd;
    memcpy(&new_brd, &brd, sizeof(Brd));
    return new_brd;
}

void take_back(Brd& brd)
{
    brd = *(brd.last_state);
}

bool make_move(Brd& brd, int move)
{
    //In case we need to take back
    Brd cpy_brd = copy_board(brd);

    //Extracting move information
    int src_sq = get_move_source(move);
    int tar_sq = get_move_target(move);
    int pce = get_move_piece(move);
    int promo_pce = get_move_promoted(move);
    int capture = get_move_capture(move);
    int double_push = get_move_double(move);
    int enpas = get_move_enpas(move);
    int castling = get_move_castling(move);

    //Move piece
    pop_bit(brd.pce[pce], src_sq);
    set_bit(brd.pce[pce], tar_sq);

    if (brd.color == WHITE)
    {
        //Handling capture
        if (capture)
        {
            for (int cap_pce = BP; cap_pce <= BQ; cap_pce++)
            {
                if (get_bit(brd.pce[cap_pce], tar_sq))
                {
                    pop_bit(brd.pce[cap_pce], tar_sq);
                }
            }
        }

        //Handling promotions
        if (promo_pce)
        {
            pop_bit(brd.pce[WP], tar_sq);
            set_bit(brd.pce[promo_pce], tar_sq);
        }

        //Handling enpas
        if (enpas)
        {
            pop_bit(brd.pce[BP], tar_sq + 8);
        }

        //Reset enpas square to none
        brd.enpas = 0;

        //Reinitialize enpas square if double pawn push
        if (double_push)
        {
            brd.enpas = tar_sq + 8;
        }

        //Handle castling (move rook)
        if (castling)
        {
            switch (tar_sq)
            {
                //WKCA
                case g1:
                    pop_bit(brd.pce[WR], h1);
                    set_bit(brd.pce[WR], f1);
                    break;
                //WQCA
                case c1:
                    pop_bit(brd.pce[WR], a1);
                    set_bit(brd.pce[WR], d1);
                    break;
            }
        }
    }
    else 
    {
        //Handling capture
        if (capture)
        {
            for (int cap_pce = WP; cap_pce <= WQ; cap_pce++)
            {
                if (get_bit(brd.pce[cap_pce], tar_sq))
                {
                    pop_bit(brd.pce[cap_pce], tar_sq);
                }
            }
        }

        //Handling promotions
        if (promo_pce)
        {
            pop_bit(brd.pce[BP], tar_sq);
            set_bit(brd.pce[promo_pce], tar_sq);
        }

        //Handling enpas
        if (enpas)
        {
            pop_bit(brd.pce[WP], tar_sq - 8);
        }

        //Reset enpas square to none
        brd.enpas = 0;

        //Reinitialize enpas square if double pawn push
        if (double_push)
        {
            brd.enpas = tar_sq - 8;
        }

        //Handle castling (move rook)
        if (castling)
        {
            switch (tar_sq)
            {
                //BKCA
                case g8:
                    pop_bit(brd.pce[BR], h8);
                    set_bit(brd.pce[BR], f8);
                    break;
                //BQCA
                case c8:
                    pop_bit(brd.pce[BR], a8);
                    set_bit(brd.pce[BR], d8);
                    break;
            }
        }
    }

    //Update castling rights
    brd.castleperms &= castling_rights[src_sq];
    brd.castleperms &= castling_rights[tar_sq];

    //Update occupancies
    brd.side[WHITE] = brd.pce[WP] | brd.pce[WN] | brd.pce[WB] | brd.pce[WR] | brd.pce[WQ] | brd.pce[WK];
    brd.side[BLACK] = brd.pce[BP] | brd.pce[BN] | brd.pce[BB] | brd.pce[BR] | brd.pce[BQ] | brd.pce[BK];
    brd.occ = brd.side[WHITE] | brd.side[BLACK];

    brd.fifty_move++;
    brd.last_state = &cpy_brd;
    
    //Take back if in check (illegal)
    if (in_check(brd))
    {
        take_back(brd);
        return false;
    }

    brd.color ^= 1;

    return true;
}

int perft_driver(Brd& brd, int depth)
{    
    if (depth == 0)
    {
        return 1;
    }

    int total_nodes = 0;
    std::list<int> moves = generate_moves(brd);

    for (std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        Brd cpy_brd = copy_board(brd);
        if (!make_move(brd, *itr))
        {
            continue;
        }
        total_nodes += perft_driver(brd, depth - 1);
        brd = cpy_brd;
    }

    return total_nodes;
}