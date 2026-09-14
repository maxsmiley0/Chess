#include "movegen.h"

namespace {

using CheckInfo = Position::CheckInfo;

void add_moves(MoveList& ml, Square from, Bitboard targets, MoveFlag flag) {
    while (targets) ml.add(encode_move(from, pop_lsb(targets), flag));
}

void add_promotions(MoveList& ml, Square from, Square to, bool capture) {
    ml.add(encode_move(from, to, capture ? PROMO_CAP_QUEEN  : PROMO_QUEEN));
    ml.add(encode_move(from, to, capture ? PROMO_CAP_KNIGHT : PROMO_KNIGHT));
    ml.add(encode_move(from, to, capture ? PROMO_CAP_ROOK   : PROMO_ROOK));
    ml.add(encode_move(from, to, capture ? PROMO_CAP_BISHOP : PROMO_BISHOP));
}

template<Color Us, bool CapturesOnly>
void gen_pawns(MoveList& ml, const Position& pos, const CheckInfo& ci) {
    constexpr Color Them     = ~Us;
    constexpr int   Up       = Us == WHITE ? 8 : -8;
    constexpr int   UpLeft   = Us == WHITE ? 7 : -9;
    constexpr int   UpRight  = Us == WHITE ? 9 : -7;
    constexpr Rank  LastRank = Us == WHITE ? RANK_8 : RANK_1;
    constexpr Rank  PushRank = Us == WHITE ? RANK_3 : RANK_6;

    const Bitboard promo_rank = RANK_BB[LastRank];
    const Bitboard pawns      = pos.pieces_of(Us, PAWN);
    const Bitboard empty      = ~pos.occupied();
    const Bitboard enemies    = pos.pieces_of(Them) ^ pos.pieces_of(Them, KING);

    const Bitboard free_pawns   = pawns & ~ci.pinned;
    const Bitboard pinned_pawns = pawns &  ci.pinned;

    if (!CapturesOnly) {
        Bitboard pushed  = shift<Up>(free_pawns) & empty;
        Bitboard doubled = shift<Up>(pushed & RANK_BB[PushRank]) & empty & ci.check_mask;
        pushed &= ci.check_mask;

        Bitboard quiet = pushed & ~promo_rank;
        while (quiet) {
            Square to = pop_lsb(quiet);
            ml.add(encode_move(static_cast<Square>(to - Up), to, QUIET));
        }

        Bitboard promoted = pushed & promo_rank;
        while (promoted) {
            Square to = pop_lsb(promoted);
            add_promotions(ml, static_cast<Square>(to - Up), to, false);
        }

        while (doubled) {
            Square to = pop_lsb(doubled);
            ml.add(encode_move(static_cast<Square>(to - 2 * Up), to, DOUBLE_PUSH));
        }
    }

    Bitboard left  = shift<UpLeft>(free_pawns)  & enemies & ci.check_mask;
    Bitboard right = shift<UpRight>(free_pawns) & enemies & ci.check_mask;

    Bitboard left_quiet = left & ~promo_rank;
    while (left_quiet) {
        Square to = pop_lsb(left_quiet);
        ml.add(encode_move(static_cast<Square>(to - UpLeft), to, CAPTURE));
    }
    Bitboard left_promo = left & promo_rank;
    while (left_promo) {
        Square to = pop_lsb(left_promo);
        add_promotions(ml, static_cast<Square>(to - UpLeft), to, true);
    }

    Bitboard right_quiet = right & ~promo_rank;
    while (right_quiet) {
        Square to = pop_lsb(right_quiet);
        ml.add(encode_move(static_cast<Square>(to - UpRight), to, CAPTURE));
    }
    Bitboard right_promo = right & promo_rank;
    while (right_promo) {
        Square to = pop_lsb(right_promo);
        add_promotions(ml, static_cast<Square>(to - UpRight), to, true);
    }

    Bitboard pinned = pinned_pawns;
    while (pinned) {
        Square   from = pop_lsb(pinned);
        Bitboard mask = ci.mask_for(from);

        if (!CapturesOnly) {
            Bitboard pushed = shift<Up>(bit(from)) & empty;
            Bitboard doubled = square_rank(static_cast<Square>(from + Up)) == PushRank
                             ? shift<Up>(pushed) & empty & mask
                             : 0;
            pushed &= mask;

            if (pushed & promo_rank) add_promotions(ml, from, lsb(pushed), false);
            else                     add_moves(ml, from, pushed, QUIET);
            add_moves(ml, from, doubled, DOUBLE_PUSH);
        }

        Bitboard captures = PAWN_ATTACKS[Us][from] & enemies & mask;
        Bitboard promoted = captures & promo_rank;
        add_moves(ml, from, captures & ~promo_rank, CAPTURE);
        while (promoted) add_promotions(ml, from, pop_lsb(promoted), true);
    }

    Square target = pos.ep_square();
    if (target != SQUARE_NONE) {
        Bitboard candidates = pawns & PAWN_ATTACKS[Them][target];
        while (candidates) {
            Square from = pop_lsb(candidates);
            if (pos.en_passant_is_legal(from, target))
                ml.add(encode_move(from, target, EN_PASSANT));
        }
    }
}

template<PieceType Pt, Color Us, bool CapturesOnly>
void gen_piece(MoveList& ml, const Position& pos, const CheckInfo& ci) {
    static_assert(Pt != PAWN && Pt != KING);

    constexpr Color Them = ~Us;

    const Bitboard occ     = pos.occupied();
    const Bitboard enemies = pos.pieces_of(Them) ^ pos.pieces_of(Them, KING);
    const Bitboard movers  = pos.pieces_of(Us, Pt);

    auto emit = [&](Square from, Bitboard attacks) {
        add_moves(ml, from, attacks & enemies, CAPTURE);
        if (!CapturesOnly) add_moves(ml, from, attacks & ~occ, QUIET);
    };

    auto attacks_from = [&](Square from) {
        if constexpr (Pt == KNIGHT) return KNIGHT_ATTACKS[from];
        if constexpr (Pt == BISHOP) return bishop_attacks(from, occ);
        if constexpr (Pt == ROOK)   return rook_attacks(from, occ);
        if constexpr (Pt == QUEEN)  return queen_attacks(from, occ);
    };

    Bitboard free_movers = movers & ~ci.pinned;
    while (free_movers) {
        Square from = pop_lsb(free_movers);
        emit(from, attacks_from(from) & ci.check_mask);
    }

    if constexpr (Pt != KNIGHT) {
        Bitboard pinned_movers = movers & ci.pinned;
        while (pinned_movers) {
            Square from = pop_lsb(pinned_movers);
            emit(from, attacks_from(from) & ci.check_mask & line(ci.king, from));
        }
    }
}

template<Color Us, bool CapturesOnly>
void gen_king(MoveList& ml, const Position& pos, const CheckInfo& ci) {
    constexpr Color Them = ~Us;

    const Square   from    = pos.king_square(Us);
    const Bitboard occ     = pos.occupied();
    const Bitboard enemies = pos.pieces_of(Them) ^ pos.pieces_of(Them, KING);
    const Bitboard attacks = KING_ATTACKS[from] & ~pos.pieces_of(Us) & ~ci.danger;

    add_moves(ml, from, attacks & enemies, CAPTURE);

    if (CapturesOnly) return;

    add_moves(ml, from, attacks & ~occ, QUIET);

    if (!ci.checkers) {
        constexpr Square Origin           = Us == WHITE ? E1 : E8;
        constexpr Square KingsideTransit  = Us == WHITE ? F1 : F8;
        constexpr Square KingsideTarget   = Us == WHITE ? G1 : G8;
        constexpr Square QueensideTransit = Us == WHITE ? D1 : D8;
        constexpr Square QueensideTarget  = Us == WHITE ? C1 : C8;
        constexpr Square QueensideSpare   = Us == WHITE ? B1 : B8;
        constexpr CastlingRight Kingside  = Us == WHITE ? WHITE_OO  : BLACK_OO;
        constexpr CastlingRight Queenside = Us == WHITE ? WHITE_OOO : BLACK_OOO;

        constexpr Bitboard KingsidePath  = bit(KingsideTransit) | bit(KingsideTarget);
        constexpr Bitboard QueensidePath = bit(QueensideSpare) | bit(QueensideTarget)
                                         | bit(QueensideTransit);

        constexpr Bitboard KingsideSafe  = bit(KingsideTransit) | bit(KingsideTarget);
        constexpr Bitboard QueensideSafe = bit(QueensideTransit) | bit(QueensideTarget);

        if (pos.can_castle(Kingside)
            && !(occ & KingsidePath) && !(ci.danger & KingsideSafe))
            ml.add(encode_move(Origin, KingsideTarget, KINGSIDE_CASTLE));

        if (pos.can_castle(Queenside)
            && !(occ & QueensidePath) && !(ci.danger & QueensideSafe))
            ml.add(encode_move(Origin, QueensideTarget, QUEENSIDE_CASTLE));
    }
}

template<Color Us, bool CapturesOnly>
void generate_all(MoveList& ml, const Position& pos) {
    CheckInfo ci = pos.compute_check_info();

    if (ci.double_check) {
        gen_king<Us, CapturesOnly>(ml, pos, ci);
        return;
    }

    gen_pawns<Us, CapturesOnly>(ml, pos, ci);
    gen_piece<KNIGHT, Us, CapturesOnly>(ml, pos, ci);
    gen_piece<BISHOP, Us, CapturesOnly>(ml, pos, ci);
    gen_piece<ROOK,   Us, CapturesOnly>(ml, pos, ci);
    gen_piece<QUEEN,  Us, CapturesOnly>(ml, pos, ci);
    gen_king<Us, CapturesOnly>(ml, pos, ci);
}

}

void generate_moves(const Position& pos, MoveList& ml) {
    if (pos.side_to_move() == WHITE) generate_all<WHITE, false>(ml, pos);
    else                             generate_all<BLACK, false>(ml, pos);
}

void generate_captures(const Position& pos, MoveList& ml) {
    if (pos.side_to_move() == WHITE) generate_all<WHITE, true>(ml, pos);
    else                             generate_all<BLACK, true>(ml, pos);
}

MoveList generate_moves(const Position& pos) {
    MoveList ml;
    generate_moves(pos, ml);
    return ml;
}

MoveList generate_captures(const Position& pos) {
    MoveList ml;
    generate_captures(pos, ml);
    return ml;
}
