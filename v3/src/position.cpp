#include "position.h"
#include "zobrist.h"
#include <cassert>
#include <cctype>
#include <sstream>

void Position::put_piece(Piece p, Square s) {
    bb_set(pieces[p], s);
    bb_set(by_color[piece_color(p)], s);
    board[s] = p;
}

void Position::remove_piece(Square s) {
    Piece p = board[s];
    bb_clr(pieces[p], s);
    bb_clr(by_color[piece_color(p)], s);
    board[s] = PIECE_NONE;
}

void Position::move_piece(Square from, Square to) {
    Piece p = board[from];
    Bitboard mask = bit(from) | bit(to);
    pieces[p]              ^= mask;
    by_color[piece_color(p)] ^= mask;
    board[from] = PIECE_NONE;
    board[to]   = p;
}

static constexpr Piece piece_from_char(char c) {
    switch (c) {
        case 'P': return W_PAWN;
        case 'N': return W_KNIGHT;
        case 'B': return W_BISHOP;
        case 'R': return W_ROOK;
        case 'Q': return W_QUEEN;
        case 'K': return W_KING;
        case 'p': return B_PAWN;
        case 'n': return B_KNIGHT;
        case 'b': return B_BISHOP;
        case 'r': return B_ROOK;
        case 'q': return B_QUEEN;
        case 'k': return B_KING;
        default:  return PIECE_NONE;
    }
}

static constexpr char PIECE_TO_CHAR[13] = {
    'P','N','B','R','Q','K',
    'p','n','b','r','q','k','.'
};

void Position::set(const std::string& fen) {
    for (auto& b  : pieces)   b  = 0;
    for (auto& b  : by_color) b  = 0;
    for (auto& sq : board)    sq = PIECE_NONE;
    ply = game_ply = 0;
    state[0] = {};
    state[0].ep_square = SQUARE_NONE;

    Key key = 0;

    std::istringstream ss(fen);
    std::string token;

    ss >> token;
    int rank = 7, file = 0;
    for (char c : token) {
        if (c == '/') { rank--; file = 0; }
        else if (c >= '1' && c <= '8') { file += c - '0'; }
        else {
            Piece p = piece_from_char(c);
            Square s = make_square(static_cast<File>(file), static_cast<Rank>(rank));
            put_piece(p, s);
            key ^= Zobrist::piece[p][s];
            file++;
        }
    }

    ss >> token;
    side = (token == "w") ? WHITE : BLACK;
    if (side == BLACK) key ^= Zobrist::side;

    ss >> token;
    state[0].castling = NO_CASTLING;
    for (char c : token) {
        switch (c) {
            case 'K': state[0].castling |= WHITE_OO;  break;
            case 'Q': state[0].castling |= WHITE_OOO; break;
            case 'k': state[0].castling |= BLACK_OO;  break;
            case 'q': state[0].castling |= BLACK_OOO; break;
        }
    }
    key ^= Zobrist::castling[state[0].castling];

    ss >> token;
    if (token != "-") {
        File ep_f = static_cast<File>(token[0] - 'a');
        Rank ep_r = static_cast<Rank>(token[1] - '1');
        state[0].ep_square = make_square(ep_f, ep_r);
        key ^= Zobrist::ep_file[ep_f];
    }

    int halfmove = 0, fullmove = 1;
    ss >> halfmove >> fullmove;
    state[0].fifty_move = static_cast<uint8_t>(halfmove);
    game_ply = (fullmove - 1) * 2 + (side == BLACK ? 1 : 0);

    key_history[0] = key;
    history_size   = 1;
}

std::string Position::fen() const {
    std::ostringstream ss;
    for (int r = 7; r >= 0; r--) {
        int empty = 0;
        for (int f = 0; f < 8; f++) {
            Square s = make_square(static_cast<File>(f), static_cast<Rank>(r));
            Piece  p = board[s];
            if (p == PIECE_NONE) { empty++; }
            else {
                if (empty) { ss << empty; empty = 0; }
                ss << PIECE_TO_CHAR[p];
            }
        }
        if (empty) ss << empty;
        if (r > 0) ss << '/';
    }
    ss << (side == WHITE ? " w " : " b ");

    std::string cr;
    if (castling_rights() & WHITE_OO)  cr += 'K';
    if (castling_rights() & WHITE_OOO) cr += 'Q';
    if (castling_rights() & BLACK_OO)  cr += 'k';
    if (castling_rights() & BLACK_OOO) cr += 'q';
    ss << (cr.empty() ? "-" : cr);

    if (ep_square() != SQUARE_NONE)
        ss << ' ' << char('a' + square_file(ep_square()))
                  << char('1' + square_rank(ep_square()));
    else
        ss << " -";

    ss << ' ' << int(fifty_move_clock())
       << ' ' << (game_ply / 2 + 1);
    return ss.str();
}

Bitboard Position::attacked_by(Color by_c, Bitboard occ) const {
    Bitboard pawns = pieces[make_piece(by_c, PAWN)];
    Bitboard attacks = by_c == WHITE ? shift<9>(pawns)  | shift<7>(pawns)
                                     : shift<-9>(pawns) | shift<-7>(pawns);

    Bitboard knights = pieces[make_piece(by_c, KNIGHT)];
    while (knights) attacks |= KNIGHT_ATTACKS[pop_lsb(knights)];

    Bitboard diagonal = pieces[make_piece(by_c, BISHOP)] | pieces[make_piece(by_c, QUEEN)];
    while (diagonal) attacks |= bishop_attacks(pop_lsb(diagonal), occ);

    Bitboard straight = pieces[make_piece(by_c, ROOK)] | pieces[make_piece(by_c, QUEEN)];
    while (straight) attacks |= rook_attacks(pop_lsb(straight), occ);

    return attacks | KING_ATTACKS[king_square(by_c)];
}

Position::CheckInfo Position::compute_check_info() const {
    CheckInfo ci{};

    Color    them = ~side;
    Square   ksq  = king_square(side);
    Bitboard occ  = occupied();

    ci.danger = attacked_by(them, occ ^ bit(ksq));

    Bitboard their_diag     = pieces[make_piece(them, BISHOP)] | pieces[make_piece(them, QUEEN)];
    Bitboard their_straight = pieces[make_piece(them, ROOK)]   | pieces[make_piece(them, QUEEN)];

    ci.king     = ksq;
    ci.checkers = (PAWN_ATTACKS[side][ksq]   & pieces[make_piece(them, PAWN)])
                | (KNIGHT_ATTACKS[ksq]       & pieces[make_piece(them, KNIGHT)])
                | (bishop_attacks(ksq, occ)  & their_diag)
                | (rook_attacks(ksq, occ)    & their_straight);

    int checker_count = popcount(ci.checkers);
    ci.double_check   = checker_count >= 2;
    ci.check_mask     = checker_count == 0 ? ~0ULL
                      : checker_count == 1 ? ci.checkers | between(ksq, lsb(ci.checkers))
                                           : 0;

    Bitboard pinners = (bishop_attacks(ksq, by_color[them]) & their_diag)
                     | (rook_attacks(ksq, by_color[them])   & their_straight);

    while (pinners) {
        Bitboard blockers = between(ksq, pop_lsb(pinners)) & by_color[side];
        if (blockers && !(blockers & (blockers - 1)))
            ci.pinned |= blockers;
    }

    return ci;
}

bool Position::is_attacked(Square s, Color by_c, Bitboard occ) const {
    return (PAWN_ATTACKS[~by_c][s]  & pieces[make_piece(by_c, PAWN)])
         | (KNIGHT_ATTACKS[s]       & pieces[make_piece(by_c, KNIGHT)])
         | (bishop_attacks(s, occ)  & (pieces[make_piece(by_c, BISHOP)] | pieces[make_piece(by_c, QUEEN)]))
         | (rook_attacks(s, occ)    & (pieces[make_piece(by_c, ROOK)]   | pieces[make_piece(by_c, QUEEN)]))
         | (KING_ATTACKS[s]         & pieces[make_piece(by_c, KING)]);
}

bool Position::is_attacked(Square s, Color by_c) const {
    return is_attacked(s, by_c, occupied());
}

bool Position::en_passant_is_legal(Square from, Square to) const {
    Color  them     = ~side;
    Square captured = make_square(square_file(to), square_rank(from));
    Square ksq      = king_square(side);

    Bitboard occ = (occupied() ^ bit(from) ^ bit(captured)) | bit(to);

    Bitboard their_pawns   = pieces[make_piece(them, PAWN)] ^ bit(captured);
    Bitboard their_diag    = pieces[make_piece(them, BISHOP)] | pieces[make_piece(them, QUEEN)];
    Bitboard their_straight = pieces[make_piece(them, ROOK)]  | pieces[make_piece(them, QUEEN)];

    return !(PAWN_ATTACKS[side][ksq]     & their_pawns)
        && !(KNIGHT_ATTACKS[ksq]         & pieces[make_piece(them, KNIGHT)])
        && !(bishop_attacks(ksq, occ)    & their_diag)
        && !(rook_attacks(ksq, occ)      & their_straight)
        && !(KING_ATTACKS[ksq]           & pieces[make_piece(them, KING)]);
}

bool Position::in_check() const {
    return is_attacked(king_square(side), ~side);
}

bool Position::is_repetition(int threshold) const {
    int count = 0;
    int last  = history_size - 1;
    int limit = std::min(last, static_cast<int>(fifty_move_clock()));
    Key current = key_history[last];

    for (int i = last - 2; i >= last - limit; i -= 2) {
        if (key_history[i] == current && ++count >= threshold)
            return true;
    }
    return false;
}

static constexpr uint8_t revoke(int rights) {
    return static_cast<uint8_t>(ALL_CASTLING & ~rights);
}

static constexpr uint8_t KEEP_ALL = ALL_CASTLING;

static constexpr uint8_t CASTLING_MASK[64] = {
    revoke(WHITE_OOO), KEEP_ALL, KEEP_ALL, KEEP_ALL,
    revoke(WHITE_OO | WHITE_OOO), KEEP_ALL, KEEP_ALL, revoke(WHITE_OO),
    KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL,
    KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL,
    KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL,
    KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL,
    KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL,
    KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL, KEEP_ALL,
    revoke(BLACK_OOO), KEEP_ALL, KEEP_ALL, KEEP_ALL,
    revoke(BLACK_OO | BLACK_OOO), KEEP_ALL, KEEP_ALL, revoke(BLACK_OO),
};

void Position::do_move(Move m) {
    assert(ply < MAX_PLY);

    StateInfo&       st   = state[ply + 1];
    const StateInfo& prev = state[ply];

    const Square   from  = move_from(m);
    const Square   to    = move_to(m);
    const MoveFlag flag  = move_flag(m);
    const Piece    moved = board[from];

    Key key = key_history[history_size - 1] ^ Zobrist::side
            ^ Zobrist::piece[moved][from] ^ Zobrist::piece[moved][to];

    if (prev.ep_square != SQUARE_NONE)
        key ^= Zobrist::ep_file[square_file(prev.ep_square)];

    st.ep_square = SQUARE_NONE;
    st.captured  = PIECE_NONE;

    uint8_t fifty = prev.fifty_move + 1;

    switch (flag) {
        case QUIET:
            move_piece(from, to);
            break;

        case DOUBLE_PUSH: {
            move_piece(from, to);
            Square skipped = static_cast<Square>((from + to) / 2);
            st.ep_square = skipped;
            key ^= Zobrist::ep_file[square_file(skipped)];
            break;
        }

        case CAPTURE: {
            Piece victim = board[to];
            st.captured  = victim;
            key ^= Zobrist::piece[victim][to];
            remove_piece(to);
            move_piece(from, to);
            fifty = 0;
            break;
        }

        case EN_PASSANT: {
            Square trailing = make_square(square_file(to), square_rank(from));
            Piece  victim   = board[trailing];
            st.captured     = victim;
            key ^= Zobrist::piece[victim][trailing];
            remove_piece(trailing);
            move_piece(from, to);
            fifty = 0;
            break;
        }

        case KINGSIDE_CASTLE: {
            move_piece(from, to);
            Square rook_from = static_cast<Square>(to + 1);
            Square rook_to   = static_cast<Square>(to - 1);
            Piece  rook      = board[rook_from];
            key ^= Zobrist::piece[rook][rook_from] ^ Zobrist::piece[rook][rook_to];
            move_piece(rook_from, rook_to);
            break;
        }

        case QUEENSIDE_CASTLE: {
            move_piece(from, to);
            Square rook_from = static_cast<Square>(to - 2);
            Square rook_to   = static_cast<Square>(to + 1);
            Piece  rook      = board[rook_from];
            key ^= Zobrist::piece[rook][rook_from] ^ Zobrist::piece[rook][rook_to];
            move_piece(rook_from, rook_to);
            break;
        }

        default: {
            if (move_is_capture(m)) {
                Piece victim = board[to];
                st.captured  = victim;
                key ^= Zobrist::piece[victim][to];
                remove_piece(to);
            }
            Piece promoted = make_piece(side, promo_piece_type(m));
            key ^= Zobrist::piece[moved][to] ^ Zobrist::piece[promoted][to];
            remove_piece(from);
            put_piece(promoted, to);
            fifty = 0;
            break;
        }
    }

    if (piece_type(moved) == PAWN) fifty = 0;
    st.fifty_move = fifty;

    uint8_t rights = prev.castling & CASTLING_MASK[from] & CASTLING_MASK[to];
    if (rights != prev.castling)
        key ^= Zobrist::castling[prev.castling] ^ Zobrist::castling[rights];
    st.castling = rights;

    side = ~side;
    ply++;
    game_ply++;
    key_history[history_size++] = key;
}

void Position::undo_move(Move m) {
    side = ~side;
    ply--;
    game_ply--;
    history_size--;

    const Square   from     = move_from(m);
    const Square   to       = move_to(m);
    const MoveFlag flag     = move_flag(m);
    const Piece    captured = state[ply + 1].captured;

    switch (flag) {
        case QUIET:
        case DOUBLE_PUSH:
            move_piece(to, from);
            break;

        case CAPTURE:
            move_piece(to, from);
            put_piece(captured, to);
            break;

        case EN_PASSANT:
            move_piece(to, from);
            put_piece(captured, make_square(square_file(to), square_rank(from)));
            break;

        case KINGSIDE_CASTLE:
            move_piece(to, from);
            move_piece(static_cast<Square>(to - 1), static_cast<Square>(to + 1));
            break;

        case QUEENSIDE_CASTLE:
            move_piece(to, from);
            move_piece(static_cast<Square>(to + 1), static_cast<Square>(to - 2));
            break;

        default:
            remove_piece(to);
            put_piece(make_piece(side, PAWN), from);
            if (move_is_capture(m)) put_piece(captured, to);
            break;
    }
}

void Position::do_null_move() {
    StateInfo& st = state[ply + 1];
    st = state[ply];

    Key key = key_history[history_size - 1] ^ Zobrist::side;

    if (st.ep_square != SQUARE_NONE) {
        key ^= Zobrist::ep_file[square_file(st.ep_square)];
        st.ep_square = SQUARE_NONE;
    }

    st.fifty_move++;
    st.captured = PIECE_NONE;

    side = ~side;
    ply++;
    game_ply++;
    key_history[history_size++] = key;
}

void Position::undo_null_move() {
    side = ~side;
    ply--;
    game_ply--;
    history_size--;
}
