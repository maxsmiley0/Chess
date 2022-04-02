#ifndef BOARD_H
#define BOARD_H

#include "Movemap.h"

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

#define map unsigned long long

enum {wk = 1, wq = 2, bk = 4, bq = 8};

struct Board {
    map BPawn; map BKnight; map BBishop; map BRook; 
    map BQueen; map BKing;
    map WPawn; map WKnight; map WBishop; map WRook; 
    map WQueen; map WKing;

    map Black;
    map White;
    map Occ;

    int enpas = 0;
    int castleperms = wk | wq | bk | bq;
    int side = 0;

    constexpr Board(
        map bp, map bn, map bb, map br, map bq, map bk,
        map wp, map wn, map wb, map wr, map wq, map wk) :
        BPawn(bp), BKnight(bn), BBishop(bb), BRook(br), BQueen(bq), BKing(bk),
        WPawn(wp), WKnight(wn), WBishop(wb), WRook(wr), WQueen(wq), WKing(wk),
        Black(bp | bn | bb | br | bq | bk),
        White(wp | wn | wb | wr | wq | wk),
        Occ(Black | White)
    {
    }
};


//existe un besoin de changer le scheme. bfs, cmk, di
class BoardStatus {
    static constexpr uint64_t WNotOccupiedL = 0b01110000ull;
    static constexpr uint64_t WNotAttackedL = 0b00111000ull;

    static constexpr uint64_t WNotOccupiedR = 0b00000110ull;
    static constexpr uint64_t WNotAttackedR = 0b00001110ull;

    static constexpr uint64_t BNotOccupiedL = 0b01110000ull << 56ull;
    static constexpr uint64_t BNotAttackedL = 0b00111000ull << 56ull;

    static constexpr uint64_t BNotOccupiedR = 0b00000110ull << 56ull;
    static constexpr uint64_t BNotAttackedR = 0b00001110ull << 56ull;

    static constexpr uint64_t WRookL_Change = 0b11111000ull;
    static constexpr uint64_t BRookL_Change = 0b11111000ull << 56ull;
    static constexpr uint64_t WRookR_Change = 0b00001111ull;
    static constexpr uint64_t BRookR_Change = 0b00001111ull << 56ull;

    static constexpr uint64_t WRookL = 0b10000000ull;
    static constexpr uint64_t BRookL = 0b10000000ull << 56ull;
    static constexpr uint64_t WRookR = 0b00000001ull;
    static constexpr uint64_t BRookR = 0b00000001ull << 56ull;

public:
    const bool WhiteMove;
    const bool HasEPPawn;

    const bool WCastleL;
    const bool WCastleR;

    const bool BCastleL;
    const bool BCastleR;


    constexpr BoardStatus(bool white, bool ep, bool wcast_left, bool wcast_right, bool bcast_left, bool bcast_right) :
        WhiteMove(white), HasEPPawn(ep), WCastleL(wcast_left), WCastleR(wcast_right), BCastleL(bcast_left), BCastleR(bcast_right)
    {

    }

    constexpr BoardStatus(int pat) :
       WhiteMove((pat & 0b100000) != 0), HasEPPawn((pat & 0b010000) != 0), 
        WCastleL((pat & 0b001000) != 0), WCastleR((pat & 0b000100) != 0), 
        BCastleL((pat & 0b000010) != 0), BCastleR((pat & 0b000001) != 0)
    {

    }

    constexpr bool CanCastle() const {
        if (WhiteMove) return WCastleL | WCastleR;
        else return BCastleL | BCastleR;
    }

    constexpr bool CanCastleLeft() const {
        if (WhiteMove) return WCastleL;
        else return BCastleL;
    }

    constexpr bool CanCastleRight() const {
        if (WhiteMove) return WCastleR;
        else return BCastleR;
    }

    constexpr uint64_t Castle_RookswitchR() const {
        if (WhiteMove) return 0b00000101ull;
        else return 0b00000101ull << 56;
    }
    constexpr uint64_t Castle_RookswitchL() const {
        if (WhiteMove) return 0b10010000ull;
        else return 0b10010000ull << 56;
    }

    //https://lichess.org/analysis/r3k2r/p1pppppp/8/1R6/1r6/8/P1PPPPPP/R3K2R_w_KQkq_-_0_1

    constexpr BoardStatus PawnPush() const {
        return BoardStatus(!WhiteMove, true, WCastleL, WCastleR, BCastleL, BCastleR);
    }
};


template <bool IsWhite>
constexpr map EnemyOrEmpty(const Board& brd)
{
    if (IsWhite) 
    {
        return ~brd.White;
    }
    return ~brd.Black;
}


template <int square, bool IsWhite>
static inline bool is_square_attacked(Board brd)
{
    if (IsWhite)
    {
        return Lookup<WP, square>(0ULL) & brd.BPawn | Lookup<BN, square>(0ULL) & brd.BKnight | Lookup<BB, square>(brd.Occ) & brd.BBishop | 
               Lookup<BR, square>(brd.Occ) & brd.BRook | Lookup<BQ, square>(brd.Occ) & brd.BQueen | Lookup<BK, square>(0ULL) & brd.BKing;

    }
    else 
    {
        return Lookup<BP, square>(0ULL) & brd.WPawn | Lookup<WN, square>(0ULL) & brd.WKnight | Lookup<WB, square>(brd.Occ) & brd.WBishop | 
               Lookup<WR, square>(brd.Occ) & brd.WRook | Lookup<WQ, square>(brd.Occ) & brd.WQueen | Lookup<WK, square>(0ULL) & brd.WKing;
    }
}

//define a function that, given a board and a slider piece(s), fills in squares

//check, double check, pin

//for ls1b de bruijn multiplication
static constexpr int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

static inline int get_ls1b_index(map bitboard) {
   return index64[((bitboard & -bitboard) * 0x03f79d71b4cb0a89) >> 58];
}

//checkmask - how tf do we deal with double check?
//King will be passed in as an arg
//Assumes legal fen, may break otherwise...

//from now on no worry abt tmp just try this scheme
//Assumes LEGAL positions... may give wrong answer if checked by 6 bishops which should never happen irl

//start thinking about board / boardstatus

template <bool IsWhite>
static constexpr map checkmask(int square, Board brd, int& numatk)
{
    map chkmsk = 0ULL;
    numatk = 0;
    if (IsWhite)
    {
        map pawnmask = (WP, square, 0ULL) & brd.BPawn;
        chkmsk |= pawnmask;
        if (pawnmask) numatk++;

        map knightmask = Lookup(BN, square, 0ULL) & brd.BKnight;
        chkmsk |= knightmask;
        if (knightmask) numatk++;

        map bishops = Lookup(BB, square, brd.Occ) & brd.BBishop;
        if (bishops)
        {
            chkmsk |= bishop_checkmask[get_ls1b_index(bishops)][square];
            numatk++;
        }

        map rooks = Lookup(BR, square, brd.Occ) & brd.BRook;
        if (rooks)
        {
            chkmsk |= rook_checkmask[get_ls1b_index(rooks)][square];
            numatk++;
        }

        map queens = Lookup(BQ, square, brd.Occ) & brd.BQueen;
        if (queens)
        {
            chkmsk |= queen_checkmask[get_ls1b_index(queens)][square];
            numatk++;
        }
    }
    else 
    {
        map pawnmask = Lookup(BP, square, 0ULL) & brd.WPawn;
        chkmsk |= pawnmask;
        if (pawnmask) numatk++;

        map knightmask = Lookup(WN, square, 0ULL) & brd.WKnight;
        chkmsk |= knightmask;
        if (knightmask) numatk++;

        map bishops = Lookup(WB, square, brd.Occ) & brd.WBishop;
        if (bishops)
        {
            chkmsk |= bishop_checkmask[get_ls1b_index(bishops)][square];
            numatk++;
        }

        map rooks = Lookup(WR, square, brd.Occ) & brd.WRook;
        if (rooks)
        {
            chkmsk |= rook_checkmask[get_ls1b_index(rooks)][square];
            numatk++;
        }

        map queens = Lookup(WQ, square, brd.Occ) & brd.WQueen;
        if (queens)
        {
            chkmsk |= queen_checkmask[get_ls1b_index(queens)][square];
            numatk++;
        }
    }
    return chkmsk;
}

//few things can be inlined, mainly bools, and we have to specify state transitions...
//how to get pinmask? give up on tmp
//pinmask - if piece & blockers yields a path to the king, move forward, otherwsise return 0
//determine ray from before. ie hdvl/1234. then get diagonal blockers and & to see if queen or bishop lies there
//ezmoney

//finally need a function to return all attacked squares

#endif