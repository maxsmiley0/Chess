#include "../src/bitboard.h"
#include "../src/movegen.h"
#include "../src/position.h"
#include "../src/uci.h"
#include "../src/zobrist.h"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

namespace {

struct PerftCase {
    const char* name;
    const char* fen;
    int         depth;
    uint64_t    expected;
};

const PerftCase CASES[] = {
    { "startpos",       "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1,        20 },
    { "startpos",       "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2,       400 },
    { "startpos",       "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3,      8902 },
    { "startpos",       "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4,    197281 },
    { "startpos",       "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5,   4865609 },
    { "startpos",       "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6, 119060324 },

    { "kiwipete",       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 1,        48 },
    { "kiwipete",       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 2,      2039 },
    { "kiwipete",       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 3,     97862 },
    { "kiwipete",       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 4,   4085603 },
    { "kiwipete",       "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1", 5, 193690690 },

    { "position3",      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 1,       14 },
    { "position3",      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 2,      191 },
    { "position3",      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 3,     2812 },
    { "position3",      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 4,    43238 },
    { "position3",      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 5,   674624 },
    { "position3",      "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1", 6, 11030083 },

    { "position4",      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1,        6 },
    { "position4",      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2,      264 },
    { "position4",      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3,     9467 },
    { "position4",      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4,   422333 },
    { "position4",      "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15833292 },

    { "position4mirror","r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 1,        6 },
    { "position4mirror","r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 2,      264 },
    { "position4mirror","r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 3,     9467 },
    { "position4mirror","r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1", 4,   422333 },

    { "position5",      "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1,       44 },
    { "position5",      "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2,     1486 },
    { "position5",      "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3,    62379 },
    { "position5",      "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4,  2103487 },
    { "position5",      "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 5, 89941194 },

    { "position6",      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 1,       46 },
    { "position6",      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 2,     2079 },
    { "position6",      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 3,    89890 },
    { "position6",      "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10", 4,  3894594 },

    { "ep-pin-rank",    "3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1", 6,  1134888 },
    { "ep-pin-diag",    "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1", 6,  1015133 },
    { "ep-capture-chk", "8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1", 6, 1440467 },
    { "ep-avoid-check", "5k2/8/8/8/8/8/8/4K2R w K - 0 1", 6,       661072 },
    { "castle-queen",   "3k4/8/8/8/8/8/8/R3K3 w Q - 0 1", 6,       803711 },
    { "castle-rights",  "r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1", 4, 1274206 },
    { "promo-check",    "2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1", 6,    3821001 },
    { "discover-check", "8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1", 5,  1004658 },
    { "promo-queen",    "4k3/1P6/8/8/8/8/K7/8 w - - 0 1", 6,        217342 },
    { "underpromote",   "8/P1k5/K7/8/8/8/8/8 w - - 0 1", 6,          92683 },
    { "self-stalemate", "K1k5/8/P7/8/8/8/8/8 w - - 0 1", 6,           2217 },
    { "stalemate-checkmate", "8/k1P5/8/1K6/8/8/8/8 w - - 0 7", 7,   567584 },
    { "double-check",   "8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1", 4,      23527 },
};

MoveList reference_moves(Position& pos) {
    Color    us      = pos.side_to_move();
    Color    them    = ~us;
    Bitboard occ     = pos.occupied();
    Bitboard own     = pos.pieces_of(us);
    Bitboard takeable = pos.pieces_of(them) ^ pos.pieces_of(them, KING);

    MoveList candidates;

    int  up        = us == WHITE ? 8 : -8;
    Rank start     = us == WHITE ? RANK_2 : RANK_7;
    Rank last      = us == WHITE ? RANK_8 : RANK_1;

    Bitboard pawns = pos.pieces_of(us, PAWN);
    while (pawns) {
        Square from = pop_lsb(pawns);
        Square one  = static_cast<Square>(from + up);

        if (!(occ & bit(one))) {
            if (square_rank(one) == last) {
                candidates.add(encode_move(from, one, PROMO_QUEEN));
                candidates.add(encode_move(from, one, PROMO_ROOK));
                candidates.add(encode_move(from, one, PROMO_BISHOP));
                candidates.add(encode_move(from, one, PROMO_KNIGHT));
            } else {
                candidates.add(encode_move(from, one, QUIET));
                Square two = static_cast<Square>(one + up);
                if (square_rank(from) == start && !(occ & bit(two)))
                    candidates.add(encode_move(from, two, DOUBLE_PUSH));
            }
        }

        Bitboard caps = PAWN_ATTACKS[us][from] & takeable;
        while (caps) {
            Square to = pop_lsb(caps);
            if (square_rank(to) == last) {
                candidates.add(encode_move(from, to, PROMO_CAP_QUEEN));
                candidates.add(encode_move(from, to, PROMO_CAP_ROOK));
                candidates.add(encode_move(from, to, PROMO_CAP_BISHOP));
                candidates.add(encode_move(from, to, PROMO_CAP_KNIGHT));
            } else {
                candidates.add(encode_move(from, to, CAPTURE));
            }
        }

        Square ep = pos.ep_square();
        if (ep != SQUARE_NONE && (PAWN_ATTACKS[us][from] & bit(ep)))
            candidates.add(encode_move(from, ep, EN_PASSANT));
    }

    for (PieceType pt : { KNIGHT, BISHOP, ROOK, QUEEN, KING }) {
        Bitboard movers = pos.pieces_of(us, pt);
        while (movers) {
            Square from = pop_lsb(movers);
            Bitboard attacks = pt == KNIGHT ? KNIGHT_ATTACKS[from]
                             : pt == BISHOP ? bishop_attacks(from, occ)
                             : pt == ROOK   ? rook_attacks(from, occ)
                             : pt == QUEEN  ? queen_attacks(from, occ)
                                            : KING_ATTACKS[from];
            attacks &= ~own & ~pos.pieces_of(them, KING);
            while (attacks) {
                Square to = pop_lsb(attacks);
                candidates.add(encode_move(from, to, occ & bit(to) ? CAPTURE : QUIET));
            }
        }
    }

    if (!pos.is_attacked(pos.king_square(us), them)) {
        Square origin = us == WHITE ? E1 : E8;
        if (pos.king_square(us) == origin) {
            Square f = us == WHITE ? F1 : F8;
            Square g = us == WHITE ? G1 : G8;
            Square d = us == WHITE ? D1 : D8;
            Square c = us == WHITE ? C1 : C8;
            Square b = us == WHITE ? B1 : B8;
            CastlingRight ks = us == WHITE ? WHITE_OO  : BLACK_OO;
            CastlingRight qs = us == WHITE ? WHITE_OOO : BLACK_OOO;

            if (pos.can_castle(ks) && !(occ & (bit(f) | bit(g)))
                && !pos.is_attacked(f, them) && !pos.is_attacked(g, them))
                candidates.add(encode_move(origin, g, KINGSIDE_CASTLE));

            if (pos.can_castle(qs) && !(occ & (bit(b) | bit(c) | bit(d)))
                && !pos.is_attacked(d, them) && !pos.is_attacked(c, them))
                candidates.add(encode_move(origin, c, QUEENSIDE_CASTLE));
        }
    }

    MoveList legal;
    for (Move m : candidates) {
        pos.do_move(m);
        bool exposed = pos.is_attacked(pos.king_square(us), them);
        pos.undo_move(m);
        if (!exposed) legal.add(m);
    }
    return legal;
}


bool zobrist_matches_scratch(const Position& pos) {
    Position fresh;
    fresh.set(pos.fen());
    return fresh.zobrist_key() == pos.zobrist_key();
}

bool validate(Position& pos, int depth, std::string& trail) {
    if (!zobrist_matches_scratch(pos)) {
        std::cout << "ZOBRIST MISMATCH after: " << (trail.empty() ? "(root)" : trail) << '\n'
                  << "  fen: " << pos.fen() << '\n';
        return false;
    }

    MoveList fast = generate_moves(pos);
    MoveList ref  = reference_moves(pos);

    std::vector<Move> a(fast.begin(), fast.end());
    std::vector<Move> b(ref.begin(), ref.end());
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    if (a != b) {
        std::cout << "MISMATCH after: " << (trail.empty() ? "(root)" : trail) << '\n'
                  << "  fen: " << pos.fen() << '\n'
                  << "  generated " << a.size() << ", reference " << b.size() << '\n';

        for (Move m : b)
            if (std::find(a.begin(), a.end(), m) == a.end())
                std::cout << "  MISSING  " << move_to_uci(m)
                          << "  flag " << int(move_flag(m)) << '\n';
        for (Move m : a)
            if (std::find(b.begin(), b.end(), m) == b.end())
                std::cout << "  EXTRA    " << move_to_uci(m)
                          << "  flag " << int(move_flag(m)) << '\n';
        return false;
    }

    if (depth <= 1) return true;

    for (Move m : ref) {
        size_t mark = trail.size();
        trail += move_to_uci(m);
        trail += ' ';
        pos.do_move(m);
        bool ok = validate(pos, depth - 1, trail);
        pos.undo_move(m);
        trail.resize(mark);
        if (!ok) return false;
    }
    return true;
}

uint64_t perft(Position& pos, int depth) {
    MoveList moves = generate_moves(pos);

    if (depth == 1) return moves.size;

    uint64_t nodes = 0;
    for (Move m : moves) {
        pos.do_move(m);
        nodes += perft(pos, depth - 1);
        pos.undo_move(m);
    }
    return nodes;
}

void divide(Position& pos, int depth) {
    MoveList moves = generate_moves(pos);
    uint64_t total = 0;

    for (Move m : moves) {
        pos.do_move(m);
        uint64_t nodes = depth == 1 ? 1 : perft(pos, depth - 1);
        pos.undo_move(m);
        std::cout << move_to_uci(m) << ": " << nodes << '\n';
        total += nodes;
    }
    std::cout << "total: " << total << std::endl;
}

int run_suite(int max_depth) {
    int failures = 0;
    const char* current = nullptr;

    for (const PerftCase& c : CASES) {
        if (c.depth > max_depth) continue;

        if (current == nullptr || std::strcmp(current, c.name) != 0) {
            current = c.name;
            std::cout << '\n' << c.name << '\n';
        }

        Position pos;
        pos.set(c.fen);

        auto start = std::chrono::steady_clock::now();
        uint64_t nodes = perft(pos, c.depth);
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - start).count();

        bool ok = nodes == c.expected;
        if (!ok) failures++;

        std::cout << (ok ? "  pass  " : "  FAIL  ")
                  << "depth " << c.depth
                  << "  got " << std::setw(10) << nodes
                  << "  want " << std::setw(10) << c.expected;

        if (elapsed > 0) {
            std::cout << "  " << (nodes / static_cast<uint64_t>(elapsed)) << " kn/s";
        }
        std::cout << std::endl;
    }

    std::cout << '\n' << (failures == 0 ? "all passed" : "failures: " + std::to_string(failures))
              << std::endl;
    return failures;
}


}

namespace {

Move match_san(const Position& pos, const std::string& raw) {
    std::string token;
    for (char c : raw)
        if (c != '+' && c != '#' && c != '!' && c != '?') token += c;
    if (token.empty()) return MOVE_NONE;

    MoveList legal = generate_moves(pos);

    if (token == "O-O" || token == "O-O-O") {
        MoveFlag want = token == "O-O" ? KINGSIDE_CASTLE : QUEENSIDE_CASTLE;
        for (Move m : legal)
            if (move_flag(m) == want) return m;
        return MOVE_NONE;
    }

    PieceType promotion = PIECE_TYPE_NONE;
    size_t equals = token.find('=');
    if (equals != std::string::npos) {
        char c = token[equals + 1];
        promotion = c == 'N' ? KNIGHT : c == 'B' ? BISHOP : c == 'R' ? ROOK : QUEEN;
        token.erase(equals);
    }

    if (token.size() < 2) return MOVE_NONE;

    Square target = make_square(static_cast<File>(token[token.size() - 2] - 'a'),
                                static_cast<Rank>(token[token.size() - 1] - '1'));
    token.erase(token.size() - 2);

    PieceType mover = PAWN;
    if (!token.empty() && std::isupper(static_cast<unsigned char>(token[0]))) {
        char c = token[0];
        mover = c == 'N' ? KNIGHT : c == 'B' ? BISHOP : c == 'R' ? ROOK
              : c == 'Q' ? QUEEN  : KING;
        token.erase(0, 1);
    }

    int from_file = -1, from_rank = -1;
    for (char c : token) {
        if (c >= 'a' && c <= 'h') from_file = c - 'a';
        else if (c >= '1' && c <= '8') from_rank = c - '1';
    }

    for (Move m : legal) {
        if (move_to(m) != target) continue;
        if (piece_type(pos.piece_on(move_from(m))) != mover) continue;

        if (promotion != PIECE_TYPE_NONE) {
            if (!move_is_promo(m) || promo_piece_type(m) != promotion) continue;
        } else if (move_is_promo(m)) {
            continue;
        }

        Square from = move_from(m);
        if (from_file >= 0 && square_file(from) != from_file) continue;
        if (from_rank >= 0 && square_rank(from) != from_rank) continue;

        return m;
    }
    return MOVE_NONE;
}

int pgn_to_fens(const char* path, int limit) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "cannot open " << path << std::endl;
        return 1;
    }

    Position pos;
    std::string line;
    std::string movetext;
    int emitted = 0;
    int rejected = 0;

    auto flush_game = [&]() {
        if (movetext.empty()) return;

        pos.set(START_FEN);
        std::istringstream stream(movetext);
        std::string token;
        bool ok = true;

        while (stream >> token) {
            if (token.back() == '.') continue;
            if (!token.empty() && std::isdigit(static_cast<unsigned char>(token[0]))
                && token.find('.') != std::string::npos) continue;
            if (token == "1-0" || token == "0-1" || token == "1/2-1/2" || token == "*") break;

            size_t dot = token.find_last_of('.');
            std::string san = dot == std::string::npos ? token : token.substr(dot + 1);
            if (san.empty()) continue;

            Move m = match_san(pos, san);
            if (m == MOVE_NONE) { ok = false; break; }
            pos.do_move(m);
        }

        if (ok) { std::cout << pos.fen() << '\n'; emitted++; }
        else      rejected++;

        movetext.clear();
    };

    while (std::getline(file, line)) {
        if (!line.empty() && line[0] == '[') {
            flush_game();
            if (limit > 0 && emitted >= limit) break;
            continue;
        }
        if (line.empty()) continue;
        movetext += line;
        movetext += ' ';
    }
    if (limit <= 0 || emitted < limit) flush_game();

    std::cerr << "emitted " << emitted << " rejected " << rejected << std::endl;
    return rejected == 0 ? 0 : 1;
}

}

void run_single(const char* fen, int depth) {
    Position pos;
    pos.set(fen);

    auto start = std::chrono::steady_clock::now();
    uint64_t nodes = perft(pos, depth);
    auto micros = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::steady_clock::now() - start).count();

    double seconds = static_cast<double>(micros) / 1e6;

    std::cout << "nodes   " << nodes << '\n'
              << "seconds " << std::fixed << std::setprecision(3) << seconds << '\n'
              << "nps     " << static_cast<uint64_t>(nodes / seconds) << std::endl;
}

int check_openings(const char* path, const char* epd_out) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "cannot open " << path << std::endl;
        return 1;
    }

    std::ofstream epd;
    if (epd_out) {
        epd.open(epd_out);
        if (!epd) {
            std::cerr << "cannot write " << epd_out << std::endl;
            return 1;
        }
    }

    int failures = 0;
    int lines    = 0;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        std::string name;
        stream >> name;

        Position pos;
        pos.set(START_FEN);

        std::string token;
        std::string played;
        bool ok = true;
        int  plies = 0;

        while (stream >> token) {
            Move m = parse_uci_move(pos, token);
            if (m == MOVE_NONE) {
                std::cout << "  ILLEGAL  " << name << "  at ply " << (plies + 1)
                          << " move " << token << '\n'
                          << "    after: " << played << '\n'
                          << "    fen:   " << pos.fen() << '\n';
                ok = false;
                break;
            }
            pos.do_move(m);
            pos.set_search_root();
            played += token + " ";
            plies++;

            std::string trail = name + ": " + played;
            if (!validate(pos, 1, trail)) { ok = false; break; }
        }

        lines++;
        if (!ok) {
            failures++;
        } else {
            std::cout << "  ok  " << std::setw(20) << std::left << name
                      << plies << " plies   " << pos.fen() << '\n';
            if (epd) epd << pos.fen() << '\n';
        }
    }

    std::cout << '\n' << lines << " openings, "
              << (failures == 0 ? "all legal" : std::to_string(failures) + " bad");
    if (epd_out && failures == 0) std::cout << ", book -> " << epd_out;
    std::cout << std::endl;
    return failures;
}

int main(int argc, char* argv[]) {
    Zobrist::init();
    init_bitboards();

    std::string mode = argc > 1 ? argv[1] : "--perft";

    if (mode == "--openings") {
        const char* path = argc > 2 ? argv[2] : "test/openings.txt";
        const char* epd  = argc > 3 ? argv[3] : nullptr;
        return check_openings(path, epd) == 0 ? 0 : 1;
    }

    if (mode == "--single") {
        if (argc < 4) {
            std::cerr << "usage: --single <fen> <depth>" << std::endl;
            return 1;
        }
        run_single(argv[2], std::atoi(argv[3]));
        return 0;
    }

    if (mode == "--divide") {
        if (argc < 4) {
            std::cerr << "usage: --divide <fen> <depth>" << std::endl;
            return 1;
        }
        Position pos;
        pos.set(argv[2]);
        divide(pos, std::atoi(argv[3]));
        return 0;
    }


    if (mode == "--validate") {
        if (argc < 4) {
            std::cerr << "usage: --validate <fen> <depth>" << std::endl;
            return 1;
        }
        Position pos;
        pos.set(argv[2]);
        std::string trail;
        bool ok = validate(pos, std::atoi(argv[3]), trail);
        if (ok) std::cout << "movegen matches reference" << std::endl;
        return ok ? 0 : 1;
    }

    if (mode == "--pgn-fens") {
        if (argc < 3) {
            std::cerr << "usage: --pgn-fens <file.pgn> [limit]" << std::endl;
            return 1;
        }
        return pgn_to_fens(argv[2], argc > 3 ? std::atoi(argv[3]) : 0);
    }

    int max_depth = argc > 2 ? std::atoi(argv[2]) : 4;
    return run_suite(max_depth) == 0 ? 0 : 1;
}
