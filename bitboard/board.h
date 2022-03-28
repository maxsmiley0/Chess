#ifndef BOARD_H
#define BOARD_H

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

template<bool IsWhite>
map EnemyOrEmpty(const Board& brd)
{
    if constexpr (IsWhite) return ~brd.White;
    return ~brd.Black;
}

#endif