#pragma once

#include "bitboard.h"
#include "types.h"
#include <string>

struct StateInfo {
    Key     zobrist;
    Square  ep_square;
    uint8_t castling;
    uint8_t fifty_move;
    Piece   captured;
};

struct Position {
    Bitboard pieces[NUM_PIECES];
    Bitboard by_color[NUM_COLORS];
    Piece    board[NUM_SQUARES];
    Color    side;

    StateInfo state[MAX_PLY + 1];
    int       ply;
    int       game_ply;

    Key key_history[MAX_HISTORY];
    int history_size;

    Bitboard occupied()                      const { return by_color[WHITE] | by_color[BLACK]; }
    Bitboard pieces_of(Piece p)              const { return pieces[p]; }
    Bitboard pieces_of(Color c)              const { return by_color[c]; }
    Bitboard pieces_of(Color c, PieceType t) const { return pieces[make_piece(c, t)]; }
    Bitboard pieces_of(PieceType t)          const { return pieces[make_piece(WHITE, t)] | pieces[make_piece(BLACK, t)]; }
    Piece    piece_on(Square s)              const { return board[s]; }
    Square   king_square(Color c)            const { return lsb(pieces[make_piece(c, KING)]); }
    Color    side_to_move()                  const { return side; }
    Square   ep_square()                     const { return state[ply].ep_square; }
    uint8_t  castling_rights()               const { return state[ply].castling; }
    uint8_t  fifty_move_clock()              const { return state[ply].fifty_move; }
    Key      zobrist_key()                   const { return key_history[history_size - 1]; }
    bool     can_castle(CastlingRight cr)    const { return castling_rights() & cr; }

    struct CheckInfo {
        Bitboard checkers;
        Bitboard pinned;
        Bitboard check_mask;
        Bitboard danger;
        Square   king;
        bool     double_check;

        Bitboard mask_for(Square from) const {
            return (pinned & bit(from) ? line(king, from) : ~0ULL) & check_mask;
        }
    };

    CheckInfo compute_check_info() const;

    Bitboard attacked_by(Color by_c, Bitboard occ) const;
    bool is_attacked(Square s, Color by_c) const;
    bool is_attacked(Square s, Color by_c, Bitboard occ) const;
    bool in_check() const;
    bool en_passant_is_legal(Square from, Square to) const;
    bool is_repetition(int threshold = 2) const;

    void set(const std::string& fen);
    void do_move(Move m);
    void undo_move(Move m);
    void do_null_move();
    void undo_null_move();

    std::string fen() const;

private:
    void put_piece(Piece p, Square s);
    void remove_piece(Square s);
    void move_piece(Square from, Square to);
};

constexpr const char* START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
