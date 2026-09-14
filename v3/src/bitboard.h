#pragma once

#include "types.h"

// ------------------------------------------------------------------ //
//  Bit manipulation primitives
// ------------------------------------------------------------------ //

constexpr Bitboard  bit(Square s)                  { return 1ULL << s; }
constexpr bool      bb_test(Bitboard b, Square s)  { return b & bit(s); }
inline void         bb_set(Bitboard& b, Square s)  { b |=  bit(s); }
inline void         bb_clr(Bitboard& b, Square s)  { b &= ~bit(s); }

inline int    popcount(Bitboard b) { return __builtin_popcountll(b); }
inline Square lsb(Bitboard b)      { return static_cast<Square>(__builtin_ctzll(b)); }
inline Square msb(Bitboard b)      { return static_cast<Square>(63 ^ __builtin_clzll(b)); }

inline Square pop_lsb(Bitboard& b) {
    Square s = lsb(b);
    b &= b - 1;
    return s;
}

// ------------------------------------------------------------------ //
//  Board masks
// ------------------------------------------------------------------ //

constexpr Bitboard FILE_BB[8] = {
    0x0101010101010101ULL, 0x0202020202020202ULL,
    0x0404040404040404ULL, 0x0808080808080808ULL,
    0x1010101010101010ULL, 0x2020202020202020ULL,
    0x4040404040404040ULL, 0x8080808080808080ULL,
};
constexpr Bitboard RANK_BB[8] = {
    0x00000000000000FFULL, 0x000000000000FF00ULL,
    0x0000000000FF0000ULL, 0x00000000FF000000ULL,
    0x000000FF00000000ULL, 0x0000FF0000000000ULL,
    0x00FF000000000000ULL, 0xFF00000000000000ULL,
};

constexpr Bitboard NOT_FILE_A = ~FILE_BB[FILE_A];
constexpr Bitboard NOT_FILE_H = ~FILE_BB[FILE_H];

// ------------------------------------------------------------------ //
//  Direction shifts — file-safe, zero cost
// ------------------------------------------------------------------ //

template<int D> constexpr Bitboard shift(Bitboard b) {
    if constexpr (D ==  8) return  b << 8;
    if constexpr (D == -8) return  b >> 8;
    if constexpr (D ==  1) return (b & NOT_FILE_H) << 1;
    if constexpr (D == -1) return (b & NOT_FILE_A) >> 1;
    if constexpr (D ==  9) return (b & NOT_FILE_H) << 9;
    if constexpr (D ==  7) return (b & NOT_FILE_A) << 7;
    if constexpr (D == -7) return (b & NOT_FILE_H) >> 7;
    if constexpr (D == -9) return (b & NOT_FILE_A) >> 9;
    return 0;
}

// Pawn push directions by color
template<Color C> constexpr Bitboard pawn_push(Bitboard b) {
    return C == WHITE ? shift<8>(b) : shift<-8>(b);
}
template<Color C> constexpr Bitboard pawn_attacks_bb(Bitboard b) {
    return C == WHITE ? shift<9>(b) | shift<7>(b)
                      : shift<-9>(b) | shift<-7>(b);
}

// ------------------------------------------------------------------ //
//  Leaper attack tables — populated by init_bitboards()
// ------------------------------------------------------------------ //

extern Bitboard PAWN_ATTACKS[NUM_COLORS][NUM_SQUARES];
extern Bitboard KNIGHT_ATTACKS[NUM_SQUARES];
extern Bitboard KING_ATTACKS[NUM_SQUARES];

// ------------------------------------------------------------------ //
//  Magic bitboard tables for sliding pieces
// ------------------------------------------------------------------ //

struct Magic {
    Bitboard  mask;
    Bitboard  number;
    Bitboard* table;
    int       shift;

    Bitboard operator[](Bitboard occ) const {
        return table[((occ & mask) * number) >> shift];
    }
};

// Known-good magic numbers stored as constexpr — 1KB total, zero binary overhead.
// Attack tables (2MB+ for rooks) are populated once at startup by init_bitboards()
// using these numbers. This avoids executable bloat while keeping lookup O(1).
constexpr Bitboard BISHOP_MAGIC_NUMBERS[NUM_SQUARES] = {
    0x40040844404084ULL,   0x2004208a004208ULL,   0x10190041080202ULL,   0x108060845042010ULL,
    0x581104180800210ULL,  0x2112080446200010ULL,  0x1080820820060210ULL, 0x3c0808410220200ULL,
    0x4050404440404ULL,    0x21001420088ULL,        0x24d0080801082102ULL, 0x1020a0a020400ULL,
    0x40308200402ULL,       0x4011002100800ULL,     0x401484104104005ULL,  0x801010402020200ULL,
    0x400210c3880100ULL,   0x404022024108200ULL,   0x810018200204102ULL,  0x4002801a02003ULL,
    0x85040820080400ULL,   0x810102c808880400ULL,  0xe900410884800ULL,    0x8002020480840102ULL,
    0x220200865090201ULL,  0x2010100a02021202ULL,  0x152048408022401ULL,  0x20080002081110ULL,
    0x4001001021004000ULL, 0x800040400a011002ULL,  0xe4004081011002ULL,   0x1c004001012080ULL,
    0x8004200962a00220ULL, 0x8422100208500202ULL,  0x2000402200300c08ULL, 0x8646020080080080ULL,
    0x80020a0200100808ULL, 0x2010004880111000ULL,  0x623000a080011400ULL, 0x42008c0340209202ULL,
    0x209188240001000ULL,  0x400408a884001800ULL,  0x110400a6080400ULL,   0x1840060a44020800ULL,
    0x90080104000041ULL,   0x201011000808101ULL,   0x1a2208080504f080ULL, 0x8012020600211212ULL,
    0x500861011240000ULL,  0x180806108200800ULL,   0x4000020e01040044ULL, 0x300000261044000aULL,
    0x802241102020002ULL,  0x20906061210001ULL,    0x5a84841004010310ULL, 0x4010801011c04ULL,
    0xa010109502200ULL,    0x4a02012000ULL,         0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL,   0x6000020202d0240ULL,   0x8918844842082200ULL, 0x4010011029020020ULL,
};

constexpr Bitboard ROOK_MAGIC_NUMBERS[NUM_SQUARES] = {
    0x8a80104000800020ULL, 0x140002000100040ULL,  0x2801880a0017001ULL,  0x100081001000420ULL,
    0x200020010080420ULL,  0x3001c0002010008ULL,  0x8480008002000100ULL, 0x2080088004402900ULL,
    0x800098204000ULL,     0x2024401000200040ULL, 0x100802000801000ULL,  0x120800800801000ULL,
    0x208808088000400ULL,  0x2802200800400ULL,    0x2200800100020080ULL, 0x801000060821100ULL,
    0x80044006422000ULL,   0x100808020004000ULL,  0x12108a0010204200ULL, 0x140848010000802ULL,
    0x481828014002800ULL,  0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL,
    0x100400080208000ULL,  0x2040002120081000ULL, 0x21200680100081ULL,   0x20100080080080ULL,
    0x2000a00200410ULL,    0x20080800400ULL,       0x80088400100102ULL,   0x80004600042881ULL,
    0x4040008040800020ULL, 0x440003000200801ULL,  0x4200011004500ULL,    0x188020010100100ULL,
    0x14800401802800ULL,   0x2080040080800200ULL, 0x124080204001001ULL,  0x200046502000484ULL,
    0x480400080088020ULL,  0x1000422010034000ULL, 0x30200100110040ULL,   0x100021010009ULL,
    0x2002080100110004ULL, 0x202008004008002ULL,  0x20020004010100ULL,   0x2048440040820001ULL,
    0x101002200408200ULL,  0x40802000401080ULL,   0x4008142004410100ULL, 0x2060820c0120200ULL,
    0x1001004080100ULL,    0x20c020080040080ULL,  0x2935610830022400ULL, 0x44440041009200ULL,
    0x280001040802101ULL,  0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL,    0x12001008414402ULL,   0x2006104900a0804ULL,  0x1004081002402ULL,
};

extern Magic BISHOP_MAGICS[NUM_SQUARES];
extern Magic ROOK_MAGICS[NUM_SQUARES];

inline Bitboard bishop_attacks(Square s, Bitboard occ) { return BISHOP_MAGICS[s][occ]; }
inline Bitboard rook_attacks(Square s, Bitboard occ)   { return ROOK_MAGICS[s][occ];   }
inline Bitboard queen_attacks(Square s, Bitboard occ)  { return BISHOP_MAGICS[s][occ] | ROOK_MAGICS[s][occ]; }

// ------------------------------------------------------------------ //
//  Ray tables
// ------------------------------------------------------------------ //

extern Bitboard BETWEEN_BB[NUM_SQUARES][NUM_SQUARES];
extern Bitboard LINE_BB[NUM_SQUARES][NUM_SQUARES];

inline Bitboard between(Square a, Square b) { return BETWEEN_BB[a][b]; }
inline Bitboard line(Square a, Square b)    { return LINE_BB[a][b]; }

// ------------------------------------------------------------------ //
//  Init
// ------------------------------------------------------------------ //

void init_bitboards();
