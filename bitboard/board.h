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

struct Board {
    const map BPawn; const map BKnight; const map BBishop; const map BRook; 
    const map BQueen; const map BKing;
    const map WPawn; const map WKnight; map WBishop; const map WRook; 
    const map WQueen; const map WKing;

    const map Black;
    const map White;
    const map Occ;

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
static constexpr bool is_square_attacked(Board brd)
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

#endif