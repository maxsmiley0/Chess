#include "uci.h"
#include "movegen.h"
#include "zobrist.h"
#include <iostream>
#include <sstream>
#include <pthread.h>

std::string move_to_uci(Move m) {
    if (m == MOVE_NONE) return "0000";

    std::string text;
    text += char('a' + square_file(move_from(m)));
    text += char('1' + square_rank(move_from(m)));
    text += char('a' + square_file(move_to(m)));
    text += char('1' + square_rank(move_to(m)));

    if (move_is_promo(m)) {
        constexpr char promo_char[] = { 'n', 'b', 'r', 'q' };
        text += promo_char[promo_piece_type(m) - KNIGHT];
    }
    return text;
}

Move parse_uci_move(const Position& pos, const std::string& text) {
    if (text.size() < 4) return MOVE_NONE;

    Square from = make_square(static_cast<File>(text[0] - 'a'),
                              static_cast<Rank>(text[1] - '1'));
    Square to   = make_square(static_cast<File>(text[2] - 'a'),
                              static_cast<Rank>(text[3] - '1'));

    MoveList ml = generate_moves(pos);
    for (Move m : ml) {
        if (move_from(m) != from || move_to(m) != to) continue;

        if (move_is_promo(m)) {
            if (text.size() < 5) continue;
            PieceType wanted = text[4] == 'n' ? KNIGHT
                             : text[4] == 'b' ? BISHOP
                             : text[4] == 'r' ? ROOK
                                              : QUEEN;
            if (promo_piece_type(m) != wanted) continue;
        }
        return m;
    }
    return MOVE_NONE;
}

namespace {

// The search recurses to MAX_PLY and each frame holds one or more MoveLists,
// which at -O3 exceeds the 512KB default thread stack on macOS. std::thread
// cannot set stack size, so the search runs on a pthread with an explicit one.
constexpr size_t SEARCH_STACK_BYTES = 16u * 1024u * 1024u;

struct SearchJob {
    Searcher*    searcher;
    Position*    position;
    SearchLimits limits;
};

pthread_t search_thread;
bool      search_active = false;
SearchJob search_job;

void* search_entry(void* argument) {
    SearchJob* job = static_cast<SearchJob*>(argument);
    job->searcher->search(*job->position, job->limits);
    return nullptr;
}

void await_search() {
    if (search_active) {
        pthread_join(search_thread, nullptr);
        search_active = false;
    }
}

void launch_search(Searcher& searcher, Position& pos, const SearchLimits& limits) {
    search_job = SearchJob{ &searcher, &pos, limits };

    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    pthread_attr_setstacksize(&attributes, SEARCH_STACK_BYTES);

    if (pthread_create(&search_thread, &attributes, search_entry, &search_job) == 0)
        search_active = true;
    else
        searcher.search(pos, limits);

    pthread_attr_destroy(&attributes);
}

void parse_position(Position& pos, std::istringstream& stream) {
    std::string token;
    stream >> token;

    if (token == "startpos") {
        pos.set(START_FEN);
        stream >> token;
    } else if (token == "fen") {
        std::string fen;
        while (stream >> token && token != "moves") fen += token + " ";
        pos.set(fen);
    } else {
        return;
    }

    while (stream >> token) {
        Move m = parse_uci_move(pos, token);
        if (m == MOVE_NONE) break;
        pos.do_move(m);
        pos.set_search_root();
    }
}

void print_board(const Position& pos) {
    constexpr char GLYPH[13] = { 'P','N','B','R','Q','K','p','n','b','r','q','k','.' };

    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        std::cout << (rank + 1) << "  ";
        for (int file = FILE_A; file <= FILE_H; file++) {
            Piece p = pos.piece_on(make_square(static_cast<File>(file),
                                               static_cast<Rank>(rank)));
            std::cout << GLYPH[p] << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "\n   a b c d e f g h\n\n"
              << "fen   " << pos.fen() << '\n'
              << "key   " << pos.zobrist_key() << '\n'
              << "check " << (pos.in_check() ? "yes" : "no") << '\n'
              << "moves " << generate_moves(pos).size << std::endl;
}

SearchLimits parse_limits(std::istringstream& stream) {
    SearchLimits limits;
    std::string token;

    while (stream >> token) {
        if      (token == "depth")    stream >> limits.depth;
        else if (token == "nodes")    stream >> limits.max_nodes;
        else if (token == "movetime") stream >> limits.movetime;
        else if (token == "wtime")    stream >> limits.wtime;
        else if (token == "btime")    stream >> limits.btime;
        else if (token == "winc")     stream >> limits.winc;
        else if (token == "binc")     stream >> limits.binc;
        else if (token == "infinite") limits.infinite = true;
    }
    return limits;
}

}

void uci_loop(Position& pos, Searcher& searcher) {
    std::cout << std::unitbuf;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream stream(line);
        std::string token;
        stream >> token;

        if (token == "uci") {
            std::cout << "id name StrawberryMangoes v3\n"
                         "id author Max Smiley\n"
                         "option name MultiPV type spin default 1 min 1 max 256\n"
                         "uciok\n";

        } else if (token == "isready") {
            std::cout << "readyok\n";

        } else if (token == "setoption") {
            std::string name, value, word;
            stream >> word;
            while (stream >> word && word != "value") {
                if (!name.empty()) name += " ";
                name += word;
            }
            while (stream >> word) {
                if (!value.empty()) value += " ";
                value += word;
            }
            if (name == "MultiPV" && !value.empty())
                searcher.set_multipv(std::stoi(value));

        } else if (token == "ponderhit") {

        } else if (token == "d") {
            print_board(pos);

        } else if (token == "legalmoves") {
            MoveList ml = generate_moves(pos);
            for (Move m : ml) std::cout << move_to_uci(m) << ' ';
            std::cout << std::endl;

        } else if (token == "ucinewgame") {
            searcher.request_stop();
            await_search();
            pos.set(START_FEN);

        } else if (token == "position") {
            searcher.request_stop();
            await_search();
            parse_position(pos, stream);

        } else if (token == "go") {
            await_search();
            searcher.clear_stop();
            launch_search(searcher, pos, parse_limits(stream));

        } else if (token == "stop") {
            searcher.request_stop();

        } else if (token == "quit") {
            searcher.request_stop();
            await_search();
            break;
        }
    }

    searcher.request_stop();
    await_search();
}
