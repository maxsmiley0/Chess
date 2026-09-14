#include "bitboard.h"
#include <cstddef>

Bitboard PAWN_ATTACKS[NUM_COLORS][NUM_SQUARES];
Bitboard KNIGHT_ATTACKS[NUM_SQUARES];
Bitboard KING_ATTACKS[NUM_SQUARES];

Magic BISHOP_MAGICS[NUM_SQUARES];
Magic ROOK_MAGICS[NUM_SQUARES];

Bitboard BETWEEN_BB[NUM_SQUARES][NUM_SQUARES];
Bitboard LINE_BB[NUM_SQUARES][NUM_SQUARES];

// Each square is allocated exactly 2^popcount(mask) entries in one shared
// array rather than a uniform worst-case slot, so the working set is the sum
// of per-square requirements instead of 64 times the maximum.
static Bitboard ROOK_TABLE[102400];
static Bitboard BISHOP_TABLE[5248];

// ------------------------------------------------------------------ //
//  Occupancy subset enumeration (carry-rippler trick)
// ------------------------------------------------------------------ //

static Bitboard slow_slider_attacks(Square s, Bitboard occ, bool bishop) {
    Bitboard attacks = 0;
    const int dirs[2][4][2] = {
        {{ 1, 1}, { 1,-1}, {-1, 1}, {-1,-1}},   // bishop
        {{ 1, 0}, {-1, 0}, { 0, 1}, { 0,-1}},   // rook
    };
    int r = square_rank(s), f = square_file(s);
    for (int d = 0; d < 4; d++) {
        int nr = r + dirs[bishop ? 0 : 1][d][0];
        int nf = f + dirs[bishop ? 0 : 1][d][1];
        while (nr >= 0 && nr < 8 && nf >= 0 && nf < 8) {
            Square sq = make_square(static_cast<File>(nf), static_cast<Rank>(nr));
            attacks |= bit(sq);
            if (occ & bit(sq)) break;
            nr += dirs[bishop ? 0 : 1][d][0];
            nf += dirs[bishop ? 0 : 1][d][1];
        }
    }
    return attacks;
}

// Relevant occupancy excludes the final square of each ray: a blocker on the
// board edge cannot change which squares are attacked beyond it.
static Bitboard make_mask(Square s, bool bishop) {
    static constexpr int BISHOP_DIRS[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    static constexpr int ROOK_DIRS[4][2]   = { {1,0}, {-1,0}, {0,1},  {0,-1}  };

    Bitboard mask = 0;
    int rank = square_rank(s), file = square_file(s);

    for (int d = 0; d < 4; d++) {
        int dr = bishop ? BISHOP_DIRS[d][0] : ROOK_DIRS[d][0];
        int df = bishop ? BISHOP_DIRS[d][1] : ROOK_DIRS[d][1];

        int r = rank + dr, f = file + df;
        while (r + dr >= 0 && r + dr < 8 && f + df >= 0 && f + df < 8) {
            mask |= bit(make_square(static_cast<File>(f), static_cast<Rank>(r)));
            r += dr;
            f += df;
        }
    }
    return mask;
}

// ------------------------------------------------------------------ //
//  Magic table population
// ------------------------------------------------------------------ //

static void init_magic(Magic* magics, Bitboard* storage,
                       const Bitboard* magic_numbers, bool bishop) {
    Bitboard* cursor = storage;

    for (int s = 0; s < NUM_SQUARES; s++) {
        Square   sq   = static_cast<Square>(s);
        Bitboard mask = make_mask(sq, bishop);

        magics[s].mask   = mask;
        magics[s].number = magic_numbers[s];
        magics[s].table  = cursor;
        magics[s].shift  = 64 - popcount(mask);

        Bitboard occ = 0;
        do {
            cursor[(occ * magics[s].number) >> magics[s].shift] =
                slow_slider_attacks(sq, occ, bishop);
            occ = (occ - mask) & mask;
        } while (occ);

        cursor += 1ULL << popcount(mask);
    }
}

// ------------------------------------------------------------------ //
//  Leaper attacks
// ------------------------------------------------------------------ //

static constexpr int KNIGHT_DELTAS[8][2] = {
    { 2, 1}, { 2,-1}, {-2, 1}, {-2,-1},
    { 1, 2}, { 1,-2}, {-1, 2}, {-1,-2},
};

static constexpr int KING_DELTAS[8][2] = {
    { 1, 0}, {-1, 0}, { 0, 1}, { 0,-1},
    { 1, 1}, { 1,-1}, {-1, 1}, {-1,-1},
};

static Bitboard leaper_attacks(Square sq, const int deltas[8][2]) {
    Bitboard attacks = 0;
    int r = square_rank(sq), f = square_file(sq);
    for (int i = 0; i < 8; i++) {
        int nr = r + deltas[i][0], nf = f + deltas[i][1];
        if (nr >= 0 && nr < 8 && nf >= 0 && nf < 8)
            attacks |= bit(make_square(static_cast<File>(nf), static_cast<Rank>(nr)));
    }
    return attacks;
}

static void init_leapers() {
    for (int s = 0; s < NUM_SQUARES; s++) {
        Square sq = static_cast<Square>(s);
        Bitboard b = bit(sq);

        PAWN_ATTACKS[WHITE][s] = shift<9>(b)  | shift<7>(b);
        PAWN_ATTACKS[BLACK][s] = shift<-9>(b) | shift<-7>(b);
        KNIGHT_ATTACKS[s]      = leaper_attacks(sq, KNIGHT_DELTAS);
        KING_ATTACKS[s]        = leaper_attacks(sq, KING_DELTAS);
    }
}

// ------------------------------------------------------------------ //
//  Between / line tables
// ------------------------------------------------------------------ //

static void init_rays() {
    for (int a = 0; a < NUM_SQUARES; a++)
        for (int b = 0; b < NUM_SQUARES; b++) {
            BETWEEN_BB[a][b] = 0;
            LINE_BB[a][b]    = 0;
        }

    static constexpr bool SLIDER_KINDS[2] = { true, false };

    for (int a = 0; a < NUM_SQUARES; a++) {
        Square sa = static_cast<Square>(a);
        Bitboard ba = bit(sa);
        for (bool bishop : SLIDER_KINDS) {
            Bitboard open_from_a = slow_slider_attacks(sa, 0, bishop);
            Bitboard reachable   = open_from_a;
            while (reachable) {
                Square   b  = pop_lsb(reachable);
                Bitboard bb = bit(b);

                BETWEEN_BB[a][b] = slow_slider_attacks(sa, bb, bishop)
                                 & slow_slider_attacks(b, ba, bishop);

                LINE_BB[a][b] = (open_from_a & slow_slider_attacks(b, 0, bishop)) | ba | bb;
            }
        }
    }
}

// ------------------------------------------------------------------ //
//  Entry point
// ------------------------------------------------------------------ //

void init_bitboards() {
    init_leapers();
    init_magic(BISHOP_MAGICS, BISHOP_TABLE, BISHOP_MAGIC_NUMBERS, true);
    init_magic(ROOK_MAGICS,   ROOK_TABLE,   ROOK_MAGIC_NUMBERS,   false);
    init_rays();
}
