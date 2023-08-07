#include "board.h"
#include "Movemap.h"
#include "nnue/nnue.h"

int sideKey;              //key for if white is to move
int castleKey[4];         //4 keys for each castling side {WK, WQ, BK, BQ}
int castleKeyCombo[16];   //16 keys for all combinations of hashed castle keys
int enpasKey[64];         //only 16 possible squares, but we'll generate 64 keys for looping convenience
int pceKey[64][12];       //64 squares with 12 possible pieces

void init_keys() {
    sideKey = rand32();
    for (int i = 0; i < 4; i++) {
        castleKey[i] = rand32();
    }
    for (int i = 0; i < 64; i++) {
        enpasKey[i] = rand32();
    }
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 12; j++) {
            pceKey[i][j] = rand32();
        }
    }
    for (int i = 0; i < 16; i++) {
        int key = 0;
        if (i & WKCA) {
            key ^= castleKey[0];
        }
        if (i & WQCA) {
            key ^= castleKey[1];
        }
        if (i & BKCA) {
            key ^= castleKey[2];
        }
        if (i & BQCA) {
            key ^= castleKey[3];
        }
        castleKeyCombo[i] = key;
    }
}

//Returns true if square is under attack
static inline bool is_sq_atk(const Brd& brd, int square, int color)
{
    bitbrd occ = brd.occ;
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
static inline bool in_check(const Brd& brd)
{
    bitbrd occ = brd.occ;
    int square;

    if (brd.color == WHITE)
    {
        square = get_ls1b_index(brd.pce[WK]);
        return get_queen_attacks(square, occ) & brd.pce[BQ] || get_rook_attacks(square, occ) & brd.pce[BR] || get_bishop_attacks(square, occ) & brd.pce[BB] || 
               knight_attacks[square] & brd.pce[BN] || pawn_attacks[WHITE][square] & brd.pce[BP] || king_attacks[square] & brd.pce[BK];
    }
    else 
    {
        square = get_ls1b_index(brd.pce[BK]);
        return get_queen_attacks(square, occ) & brd.pce[WQ] || get_rook_attacks(square, occ) & brd.pce[WR] || get_bishop_attacks(square, occ) & brd.pce[WB] || 
               knight_attacks[square] & brd.pce[WN] || pawn_attacks[BLACK][square] & brd.pce[WP] || king_attacks[square] & brd.pce[WK];
    }
    
    return false;
}

char pce_on_square(int square, const Brd& brd)
{
    bitbrd sq = 0ULL;
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

int static_eval(const Brd& brd) {
    int pieces[33];
    int squares[33];

    int index = 2;
    for (int pce = 0; pce < 12; pce++)
    {
        int nnuePceValue = strawmanToNnuePce[pce]; //bitmapping from strawman pieces to nnue pieces
        bitbrd bitboard = brd.pce[pce];

        while (bitboard) {
            int sq = get_ls1b_index(bitboard);
            int nnue_sq = strawmanToNnueSq[sq];
            
            if (pce == WK)
            {
                pieces[0] = nnuePceValue;
                squares[0] = nnue_sq;
            }
            else if (pce == BK)
            {
                pieces[1] = nnuePceValue;
                squares[1] = nnue_sq;
            }
            else
            {
                pieces[index] = nnuePceValue;
                squares[index] = nnue_sq;
                index++;
            }

            pop_bit(bitboard, sq);
        }
    }

    pieces[index] = 0;
    squares[index] = 0;
    
    return nnue_evaluate(brd.color, pieces, squares);
}

//Given a fen string, returns a board loaded to that fen
Brd parse_fen(std::string fen)
{   
    Brd brd;
    memset(&brd, 0, sizeof(Brd));
    
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
                    add_pce(brd, BP, r * 8 + c);
                    c++;
                    break;
                case 'n':
                    add_pce(brd, BN, r * 8 + c);
                    c++;
                    break;
                case 'b':
                    add_pce(brd, BB, r * 8 + c);
                    c++;
                    break;
                case 'r':
                    add_pce(brd, BR, r * 8 + c);
                    c++;
                    break;
                case 'q':
                    add_pce(brd, BQ, r * 8 + c);
                    c++;
                    break;
                case 'k':
                    add_pce(brd, BK, r * 8 + c);
                    c++;
                    break;
                case 'P':
                    add_pce(brd, WP, r * 8 + c);
                    c++;
                    break;
                case 'N':
                    add_pce(brd, WN, r * 8 + c);
                    c++;
                    break;
                case 'B':
                    add_pce(brd, WB, r * 8 + c);
                    c++;
                    break;
                case 'R':
                    add_pce(brd, WR, r * 8 + c);
                    c++;
                    break;
                case 'Q':
                    add_pce(brd, WQ, r * 8 + c);
                    c++;
                    break;
                case 'K':
                    add_pce(brd, WK, r * 8 + c);
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
                brd.poskey ^= sideKey;
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
                    hashInCastle(brd, g1);
                    break;
                case 'Q':
                    hashInCastle(brd, c1);
                    break;
                case 'k':
                    hashInCastle(brd, g8);
                    break;
                case 'q':
                    hashInCastle(brd, c8);
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
                brd.enpas = (8 - ((int)fen[i + 1] - (int)'0')) * 8 + (int)fen[i] - (int)'a';
                hashInEp(brd, brd.enpas);
            }
            section++;
        }
    }

    brd.side[WHITE] = brd.pce[WP] | brd.pce[WN] | brd.pce[WB] | brd.pce[WR] | brd.pce[WQ] | brd.pce[WK];
    brd.side[BLACK] = brd.pce[BP] | brd.pce[BN] | brd.pce[BB] | brd.pce[BR] | brd.pce[BQ] | brd.pce[BK];
    brd.occ = brd.side[WHITE] | brd.side[BLACK];

    return brd;
}

//Given a board, returns a list of generated (pseudo-legal) moves in the position
void generate_moves(const Brd& brd, MoveList* move_list)
{
    move_list->cnt = 0;
    
    //Pce current / future position
    int src_sq, tar_sq;
    //Current piece bitbrd copy and its attack squares
    bitbrd bitboard, attacks;
    
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
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WQ, 0, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WR, 0, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WB, 0, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WN, 0, 0, 0, 0));
                }
                else 
                {
                    //One ahead
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, 0, 0, 0, 0, 0));
                    //Two ahead
                    if ((src_sq >= a2 && src_sq <= h2) && !get_bit(brd.occ, tar_sq - 8))
                    {
                        add_move(move_list, encode_move(src_sq, tar_sq - 8, WP, 0, 0, 1, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WQ, 1, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WR, 1, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WB, 1, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, WN, 1, 0, 0, 0));
                }
                else 
                {
                    //Regular capture case
                    add_move(move_list, encode_move(src_sq, tar_sq, WP, 0, 1, 0, 0, 0));  
                }

                pop_bit(attacks, tar_sq);
            }

            //Enpas
            if (brd.enpas)
            {
                bitbrd enpas_attack = pawn_attacks[WHITE][src_sq] & (1ULL << brd.enpas);
                if (enpas_attack)
                {
                    add_move(move_list, encode_move(src_sq, get_ls1b_index(enpas_attack), WP, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, src_sq);
        }

        //White king
        //Castle
        if (brd.castleperms & WKCA && !get_bit(brd.occ, f1) && !get_bit(brd.occ, g1) && !is_sq_atk(brd, e1, WHITE) && !is_sq_atk(brd, f1, WHITE))
        {
            add_move(move_list, encode_move(e1, g1, WK, 0, 0, 0, 0, 1));
        }
        if (brd.castleperms & WQCA && !get_bit(brd.occ, d1) && !get_bit(brd.occ, c1) && !get_bit(brd.occ, b1) && !is_sq_atk(brd, e1, WHITE) && !is_sq_atk(brd, d1, WHITE))
        {
            add_move(move_list, encode_move(e1, c1, WK, 0, 0, 0, 0, 1));
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
                add_move(move_list, encode_move(src_sq, tar_sq, WK, 0, 0, 0, 0, 0));
            }
            //Capture
            else 
            {
                add_move(move_list, encode_move(src_sq, tar_sq, WK, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, WN, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, WN, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, WB, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, WB, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, WR, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, WR, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, WQ, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, WQ, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BQ, 0, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BR, 0, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BB, 0, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BN, 0, 0, 0, 0));
                }
                else 
                {
                    //One ahead
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, 0, 0, 0, 0, 0));
                    //Two ahead
                    if ((src_sq >= a7 && src_sq <= h7) && !get_bit(brd.occ, tar_sq + 8))
                    {
                        add_move(move_list, encode_move(src_sq, tar_sq + 8, BP, 0, 0, 1, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BQ, 1, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BR, 1, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BB, 1, 0, 0, 0));
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, BN, 1, 0, 0, 0));
                }
                else 
                {
                    //Regular capture case
                    add_move(move_list, encode_move(src_sq, tar_sq, BP, 0, 1, 0, 0, 0));  
                }

                pop_bit(attacks, tar_sq);
            }

            //Enpas
            if (brd.enpas)
            {
                bitbrd enpas_attack = pawn_attacks[BLACK][src_sq] & (1ULL << brd.enpas);
                if (enpas_attack)
                {
                    add_move(move_list, encode_move(src_sq, get_ls1b_index(enpas_attack), BP, 0, 1, 0, 1, 0));
                }
            }

            pop_bit(bitboard, src_sq);
        }

        //Black king
        //Castle
        if (brd.castleperms & BKCA && !get_bit(brd.occ, f8) && !get_bit(brd.occ, g8) && !is_sq_atk(brd, e8, BLACK) && !is_sq_atk(brd, f8, BLACK))
        {
            add_move(move_list, encode_move(e8, g8, BK, 0, 0, 0, 0, 1));
        }
        if (brd.castleperms & BQCA && !get_bit(brd.occ, d8) && !get_bit(brd.occ, c8) && !get_bit(brd.occ, b8) && !is_sq_atk(brd, e8, BLACK) && !is_sq_atk(brd, d8, BLACK))
        {
            add_move(move_list, encode_move(e8, c8, BK, 0, 0, 0, 0, 1));
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
                add_move(move_list, encode_move(src_sq, tar_sq, BK, 0, 0, 0, 0, 0));
            }
            //Capture
            else 
            {
                add_move(move_list, encode_move(src_sq, tar_sq, BK, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, BN, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, BN, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, BB, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, BB, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, BR, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, BR, 0, 1, 0, 0, 0));
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
                    add_move(move_list, encode_move(src_sq, tar_sq, BQ, 0, 0, 0, 0, 0));
                }
                //Capture
                else 
                {
                    add_move(move_list, encode_move(src_sq, tar_sq, BQ, 0, 1, 0, 0, 0));
                }
                pop_bit(attacks, tar_sq);
            }

            pop_bit(bitboard, src_sq);
        }
    }
}

/*
        Need:
        pce, pce_sq, pce_bitboard, pce_attacker, attacker_bitboard
        pce: enemy pieces outer loop from Q -> P
        attacker pce: our piece inner loop from P -> Q

        return move info here
        Pass in move_hint.pce = BQ or WQ from QA
        Pce->Cap

        Ex: I'm white, I want to search the captures in this order
        WP BQ   WN BQ   WB BQ   WR BQ   WQ BQ   WK BQ
        WP BR   WN BR   WB BR   WR BR   WQ BR   WK BR
        WP BB   WN BB   WB BB   WR BB   WQ BB   WK BB
        WP BN   WN BN   WB BN   WR BN   WQ BN   WK BN
        WP BP   WN BP   WB BP   WR BP   WQ BP   WK BP
        */

MoveHint get_starter_hint(const Brd& brd) {
    MoveHint hint;
    if (brd.color == WHITE) {
        hint.capturing_pce = WP;
        hint.captured_pce = BQ;
        hint.capturing_bitboard = brd.pce[WP];
        hint.captured_bitboard = 0;
    }
    else {
        hint.capturing_pce = BP;
        hint.captured_pce = WQ;
        hint.capturing_bitboard = brd.pce[BP];
        hint.captured_bitboard = 0;
    }
    return hint;
}

int generate_capture_incr(const Brd& brd, MoveHint& move_hint) {
    if (move_hint.captured_bitboard) {
        int tar_sq = get_ls1b_index(move_hint.captured_bitboard);
        pop_bit(move_hint.captured_bitboard, tar_sq);
        //some logic here required to set the actual pieces - enpas and promotion cases!
        return encode_move(move_hint.capturing_pce_sq, tar_sq, move_hint.capturing_pce, 0, 1, 0, 0, 0);
    }// pce > pce_bitboard > attackers_bitboard
    else if (move_hint.capturing_bitboard) {
        move_hint.capturing_pce_sq = get_ls1b_index(move_hint.capturing_bitboard);
        pop_bit(move_hint.capturing_bitboard, move_hint.capturing_pce_sq);

        switch (move_hint.capturing_pce) {        //Get attack squares, and-ed with opponent's pieces to form capture moves
            case WQ:
            case BQ:
                move_hint.captured_bitboard = get_queen_attacks(move_hint.capturing_pce_sq, brd.occ) & brd.pce[move_hint.captured_pce];
                break;
            case WR:
            case BR:
                move_hint.captured_bitboard = get_rook_attacks(move_hint.capturing_pce_sq, brd.occ) & brd.pce[move_hint.captured_pce];
                break;
            case WB:
            case BB:
                move_hint.captured_bitboard = get_bishop_attacks(move_hint.capturing_pce_sq, brd.occ) & brd.pce[move_hint.captured_pce];
                break;
            case WN:
            case BN:
                move_hint.captured_bitboard = knight_attacks[move_hint.capturing_pce_sq] & brd.pce[move_hint.captured_pce];
                break;
            case WP:
                move_hint.captured_bitboard = pawn_attacks[WHITE][move_hint.capturing_pce_sq] & brd.pce[move_hint.captured_pce];
                break;
            case BP:
                move_hint.captured_bitboard = pawn_attacks[BLACK][move_hint.capturing_pce_sq] & brd.pce[move_hint.captured_pce];
                break;
            case WK:
            case BK:
                move_hint.captured_bitboard = king_attacks[move_hint.capturing_pce_sq] & brd.pce[move_hint.captured_pce];
                break;
        }
        return generate_capture_incr(brd, move_hint);
        //some optimizations here, goto's and delaying certain operations
    }
    else {
        //MVV LVA controller
        //Where are we generating en passant moves?
        move_hint.capturing_pce++;
        if (move_hint.capturing_pce == 6) {          //We were just on attacker == white king, loop back to white pawn and try next less valuable piece
            move_hint.capturing_pce = WP;
            move_hint.captured_pce--;
            if (move_hint.captured_pce == 5) {               //We were just on captured == black pawn, terminate the routine and return -1
                return -1;
            }
        }
        else if (move_hint.capturing_pce == 12) {    //We were just on attacker == black king, loop back to black pawn and try next less valuable piece
            move_hint.capturing_pce = BP;
            move_hint.captured_pce--;
            if (move_hint.captured_pce == -1) {              //We were just on captured == white pawn, terminate the routine and return -1
                return -1;
            }
        }

        move_hint.capturing_bitboard = brd.pce[move_hint.capturing_pce];
        return generate_capture_incr(brd, move_hint);
    }
}

/*
Curious to test following benchmarks:
generate_captures vs generate_captures_incr with and without AB pruning in QS
*/

bool make_move(Brd& brd, int move)
{
    //Copying
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
    remove_pce(brd, pce, src_sq);
    add_pce(brd, pce, tar_sq);

    if (brd.color == WHITE)
    {
        //Handling capture
        if (capture)
        {
            for (int cap_pce = BP; cap_pce <= BQ; cap_pce++)
            {
                if (get_bit(brd.pce[cap_pce], tar_sq))
                {
                    remove_pce(brd, cap_pce, tar_sq);
                    break;
                }
            }
        }

        //Handling promotions
        if (promo_pce)
        {
            remove_pce(brd, WP, tar_sq);
            add_pce(brd, promo_pce, tar_sq);
        }

        //Handling enpas
        if (enpas)
        {
            remove_pce(brd, BP, tar_sq + 8);
        }

        hashOutEp(brd);

        //Reinitialize enpas square if double pawn push
        if (double_push)
        {
            hashInEp(brd, tar_sq + 8);
        }

        //Handle castling (move rook)
        if (castling)
        {
            switch (tar_sq)
            {
                //WKCA
                case g1:
                    remove_pce(brd, WR, h1);
                    add_pce(brd, WR, f1);
                    break;
                //WQCA
                case c1:
                    remove_pce(brd, WR, a1);
                    add_pce(brd, WR, d1);
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
                    remove_pce(brd, cap_pce, tar_sq);
                    break;
                }
            }
        }

        //Handling promotions
        if (promo_pce)
        {
            remove_pce(brd, BP, tar_sq);
            add_pce(brd, promo_pce, tar_sq);
        }

        //Handling enpas
        if (enpas)
        {
            remove_pce(brd, WP, tar_sq - 8);
        }

        hashOutEp(brd);

        //Reinitialize enpas square if double pawn push
        if (double_push)
        {
            hashInEp(brd, tar_sq - 8);
        }

        //Handle castling (move rook)
        if (castling)
        {
            switch (tar_sq)
            {
                //BKCA
                case g8:
                    remove_pce(brd, BR, h8);
                    add_pce(brd, BR, f8);
                    break;
                //BQCA
                case c8:
                    remove_pce(brd, BR, a8);
                    add_pce(brd, BR, d8);
                    break;
            }
        }
    }

    //Update castling rights
    hashOutCastle(brd, src_sq, tar_sq);

    //Update occupancies
    brd.side[WHITE] = brd.pce[WP] | brd.pce[WN] | brd.pce[WB] | brd.pce[WR] | brd.pce[WQ] | brd.pce[WK];
    brd.side[BLACK] = brd.pce[BP] | brd.pce[BN] | brd.pce[BB] | brd.pce[BR] | brd.pce[BQ] | brd.pce[BK];
    brd.occ = brd.side[WHITE] | brd.side[BLACK];

    brd.fifty_move++;

    //Take back if in check (illegal)
    if (in_check(brd))
    {
        brd = cpy_brd;
        return false;
    }

    changeSide(brd);

    return true;
}

int perft_driver(Brd& brd, int depth)
{    
    if (depth == 0)
    {
        return 1;
    }

    int total_nodes = 0;
    MoveList move_list[1];
    generate_moves(brd, move_list);

    for (int move_cnt = 0; move_cnt < move_list->cnt; move_cnt++)
    {
        int move = move_list->move[move_cnt];
        Brd cpy_brd = copy_board(brd);

        if (!make_move(brd, move))
        {
            continue;
        }

        total_nodes += perft_driver(brd, depth - 1);
        brd = cpy_brd;
    }

    return total_nodes;
}

static inline void add_move(MoveList* move_list, int move)
{
    move_list->move[move_list->cnt] = move;
    move_list->cnt++;
}

//Sets the bit in the according b.pce[] and updates the poskey
static inline void add_pce(Brd& brd, int pce, int sq) {
    set_bit(brd.pce[pce], sq);
    brd.poskey ^= pceKey[sq][pce];
}

//Removes the bit in the according b.pce[] given known pce and updates the poskey
static inline void remove_pce(Brd& brd, int pce, int sq) {
    pop_bit(brd.pce[pce], sq);
    brd.poskey ^= pceKey[sq][pce];
}

//Adds the castleperms and poskey (does not move any pieces)
static inline void hashInCastle(Brd& brd, int sq) {
    switch (sq) {
        case c1: //WQ
            brd.poskey ^= castleKey[1];
            brd.castleperms |= WQCA;
            return;
        case g1: //WK
            brd.poskey ^= castleKey[0];
            brd.castleperms |= WKCA;
            return;
        case c8: //BQ
            brd.poskey ^= castleKey[3];
            brd.castleperms |= BQCA;
            return;
        case g8: //BK
            brd.poskey ^= castleKey[2];
            brd.castleperms |= BKCA;
            return;
    }
}

//Updates the castleperms and poskey (does not move any pieces)
static inline void hashOutCastle(Brd& brd, int src_sq, int tar_sq) {
    int init_castleperms = brd.castleperms;
    brd.castleperms &= castling_rights[src_sq];
    brd.castleperms &= castling_rights[tar_sq];

    int castle_bit_diff = init_castleperms ^ brd.castleperms;
    brd.poskey ^= castleKeyCombo[castle_bit_diff];
}

//Sets the ep, and updates position key, assuming no ep
static inline void hashInEp(Brd& brd, int sq) {
    brd.enpas = sq;
    brd.poskey ^= enpasKey[sq];
}

//Resets the ep square to 0, and updates position key
static inline void hashOutEp(Brd& brd) {
    if (brd.enpas != 0) {
        brd.poskey ^= enpasKey[brd.enpas];
        brd.enpas = 0;
    }
}

//Changes the side and updates the poskey
static inline void changeSide(Brd& brd) {
    brd.color ^= 1;
    brd.poskey ^= sideKey;
}

static inline Brd copy_board(const Brd& brd) {
    Brd new_brd;
    memcpy(&new_brd, &brd, sizeof(Brd));
    return new_brd;
}