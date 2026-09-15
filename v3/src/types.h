#pragma once

#include <cstdint>

using Bitboard = uint64_t;
using Move     = uint16_t;
using Score    = int32_t;
using Key      = uint64_t;

constexpr int NUM_SQUARES = 64;
constexpr int NUM_PIECES  = 12;
constexpr int NUM_COLORS  = 2;
constexpr int MAX_MOVES   = 256;
constexpr int MAX_PLY     = 128;
constexpr int MAX_HISTORY = 2048;

constexpr Score SCORE_INFINITE = 32767;
constexpr Score SCORE_NONE     = 32766;
constexpr Score SCORE_MATE     = 32000;
constexpr Score SCORE_DRAW     = 0;
constexpr Score SCORE_MATE_MAX = SCORE_MATE - MAX_PLY;

enum Color : uint8_t { WHITE, BLACK, COLOR_NONE };

constexpr Color operator~(Color c) { return static_cast<Color>(c ^ 1); }

enum PieceType : uint8_t {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    PIECE_TYPE_NONE
};

enum Piece : uint8_t {
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
    PIECE_NONE
};

enum Square : uint8_t {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    SQUARE_NONE
};

enum File : uint8_t { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
enum Rank : uint8_t { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

constexpr Square make_square(File f, Rank r)  { return static_cast<Square>(r * 8 + f); }
constexpr File   square_file(Square s)        { return static_cast<File>(s % 8); }
constexpr Rank   square_rank(Square s)        { return static_cast<Rank>(s / 8); }
constexpr Square flip_rank(Square s)          { return static_cast<Square>(s ^ 56); }

enum CastlingRight : uint8_t {
    NO_CASTLING  = 0,
    WHITE_OO     = 1,
    WHITE_OOO    = 2,
    BLACK_OO     = 4,
    BLACK_OOO    = 8,
    ALL_CASTLING = 15
};

enum MoveFlag : uint8_t {
    QUIET            = 0,
    DOUBLE_PUSH      = 1,
    KINGSIDE_CASTLE  = 2,
    QUEENSIDE_CASTLE = 3,
    CAPTURE          = 4,
    EN_PASSANT       = 5,
    PROMO_KNIGHT     = 8,
    PROMO_BISHOP     = 9,
    PROMO_ROOK       = 10,
    PROMO_QUEEN      = 11,
    PROMO_CAP_KNIGHT = 12,
    PROMO_CAP_BISHOP = 13,
    PROMO_CAP_ROOK   = 14,
    PROMO_CAP_QUEEN  = 15,
};

constexpr Move MOVE_NONE = 0;
constexpr Move MOVE_NULL = 65;

constexpr Move      encode_move(Square from, Square to, MoveFlag flag) {
    return static_cast<Move>((flag << 12) | (to << 6) | from);
}
constexpr Square    move_from(Move m)       { return static_cast<Square>(m & 0x3F); }
constexpr Square    move_to(Move m)         { return static_cast<Square>((m >> 6) & 0x3F); }
constexpr MoveFlag  move_flag(Move m)       { return static_cast<MoveFlag>(m >> 12); }
constexpr bool      move_is_capture(Move m) { return move_flag(m) & CAPTURE; }
constexpr bool      move_is_promo(Move m)   { return move_flag(m) >= PROMO_KNIGHT; }

constexpr PieceType promo_piece_type(Move m) {
    return static_cast<PieceType>(KNIGHT + ((move_flag(m) & 3)));
}

constexpr Color     piece_color(Piece p)               { return static_cast<Color>(p / 6); }
constexpr PieceType piece_type(Piece p)                { return static_cast<PieceType>(p % 6); }
constexpr Piece     make_piece(Color c, PieceType t)   { return static_cast<Piece>(c * 6 + t); }
