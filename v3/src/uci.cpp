#include "uci.h"
#include "movegen.h"
#include "zobrist.h"
#include <iostream>
#include <sstream>

// ------------------------------------------------------------------ //
//  Move serialisation
// ------------------------------------------------------------------ //

std::string move_to_uci(Move m) {
    if (m == MOVE_NONE) return "0000";

    std::string s;
    s += char('a' + square_file(move_from(m)));
    s += char('1' + square_rank(move_from(m)));
    s += char('a' + square_file(move_to(m)));
    s += char('1' + square_rank(move_to(m)));

    if (move_is_promo(m)) {
        constexpr char promo_char[] = { 'n','b','r','q' };
        s += promo_char[promo_piece_type(m) - KNIGHT];
    }
    return s;
}

// ------------------------------------------------------------------ //
//  Move parsing
// ------------------------------------------------------------------ //

Move parse_uci_move(const Position& pos, const std::string& str) {
    if (str.size() < 4) return MOVE_NONE;

    Square from = make_square(static_cast<File>(str[0] - 'a'),
                              static_cast<Rank>(str[1] - '1'));
    Square to   = make_square(static_cast<File>(str[2] - 'a'),
                              static_cast<Rank>(str[3] - '1'));

    // Find the matching legal move to get the correct flag
    MoveList ml = generate_moves(pos);
    for (Move m : ml) {
        if (move_from(m) != from || move_to(m) != to) continue;
        if (move_is_promo(m) && str.size() == 5) {
            char pc = str[4];
            PieceType pt = pc == 'n' ? KNIGHT
                         : pc == 'b' ? BISHOP
                         : pc == 'r' ? ROOK
                                     : QUEEN;
            if (promo_piece_type(m) != pt) continue;
        }
        return m;
    }
    return MOVE_NONE;
}

// ------------------------------------------------------------------ //
//  "position" command handler
// ------------------------------------------------------------------ //

static void handle_position(Position& pos, std::istringstream& ss) {
    std::string token;
    ss >> token;

    if (token == "startpos") {
        pos.set(START_FEN);
        ss >> token;  // consume optional "moves"
    } else if (token == "fen") {
        std::string fen;
        while (ss >> token && token != "moves")
            fen += token + " ";
        pos.set(fen);
    }

    // Replay move list
    while (ss >> token) {
        Move m = parse_uci_move(pos, token);
        if (m != MOVE_NONE) pos.do_move(m);
    }
}

// ------------------------------------------------------------------ //
//  "go" command handler
// ------------------------------------------------------------------ //

static void handle_go(Position& pos, Searcher& searcher, std::istringstream& ss) {
    SearchLimits limits;
    std::string token;

    while (ss >> token) {
        if      (token == "depth")    ss >> limits.depth;
        else if (token == "movetime") ss >> limits.movetime;
        else if (token == "wtime")    ss >> limits.wtime;
        else if (token == "btime")    ss >> limits.btime;
        else if (token == "winc")     ss >> limits.winc;
        else if (token == "binc")     ss >> limits.binc;
        else if (token == "infinite") limits.infinite = true;
    }

    searcher.search(pos, limits);
}

// ------------------------------------------------------------------ //
//  Main UCI loop
// ------------------------------------------------------------------ //

void uci_loop(Position& pos, Searcher& searcher) {
    std::cout << std::unitbuf;  // disable output buffering

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "uci") {
            std::cout << "id name StrawberryMangoes v3\n"
                      << "id author Max Smiley\n"
                      << "uciok\n";

        } else if (token == "isready") {
            std::cout << "readyok\n";

        } else if (token == "ucinewgame") {
            pos.set(START_FEN);

        } else if (token == "position") {
            handle_position(pos, ss);

        } else if (token == "go") {
            handle_go(pos, searcher, ss);

        } else if (token == "stop") {
            searcher.stop();

        } else if (token == "quit") {
            break;
        }
    }
}
