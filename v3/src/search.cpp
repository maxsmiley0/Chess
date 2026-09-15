#include "search.h"
#include "eval.h"
#include "movegen.h"
#include "uci.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>

static constexpr int MVV_LVA[6][6] = {
    { 15, 25, 35, 45, 55, 0 },
    { 14, 24, 34, 44, 54, 0 },
    { 13, 23, 33, 43, 53, 0 },
    { 12, 22, 32, 42, 52, 0 },
    { 11, 21, 31, 41, 51, 0 },
    { 10, 20, 30, 40, 50, 0 },
};

static std::string format_score(Score score) {
    if (score >= SCORE_MATE_MAX)
        return "mate " + std::to_string((SCORE_MATE - score + 1) / 2);
    if (score <= -SCORE_MATE_MAX)
        return "mate " + std::to_string(-(SCORE_MATE + score + 1) / 2);
    return "cp " + std::to_string(score);
}

static int64_t now_ms() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void Searcher::init_timer(const Position& pos, const SearchLimits& limits) {
    start_ms   = now_ms();
    stopped    = false;
    nodes      = 0;
    node_limit = limits.max_nodes;

    if (limits.infinite) {
        alloc_ms = 0;
        return;
    }
    if (limits.movetime > 0) {
        alloc_ms = std::max(1, limits.movetime - 10);
        return;
    }

    int remaining = pos.side_to_move() == WHITE ? limits.wtime : limits.btime;
    int increment = pos.side_to_move() == WHITE ? limits.winc  : limits.binc;

    if (remaining > 0) {
        alloc_ms = std::max(1, remaining / 20 + increment / 2);
    } else if (limits.depth >= MAX_PLY && limits.max_nodes == 0) {
        alloc_ms = DEFAULT_MOVETIME_MS;
    } else {
        alloc_ms = 0;
    }
}

int64_t Searcher::elapsed_ms() const {
    return now_ms() - start_ms;
}

bool Searcher::check_time() {
    if (stopped) return true;
    if (node_limit && nodes >= node_limit) {
        stopped = true;
        return true;
    }
    if ((nodes & 1023) != 0) return false;

    if (stop_requested.load(std::memory_order_relaxed)) {
        stopped = true;
        return true;
    }
    if (alloc_ms > 0 && elapsed_ms() >= alloc_ms) {
        stopped = true;
        return true;
    }
    return false;
}

int Searcher::move_score(const Position& pos, Move m) const {
    if (move_flag(m) == EN_PASSANT) return 100000 + MVV_LVA[PAWN][PAWN];

    if (move_is_capture(m)) {
        PieceType attacker = piece_type(pos.piece_on(move_from(m)));
        PieceType victim   = piece_type(pos.piece_on(move_to(m)));
        return 100000 + MVV_LVA[attacker][victim];
    }

    if (move_is_promo(m)) return 90000 + promo_piece_type(m);

    return 0;
}

void Searcher::order_moves(const Position& pos, MoveList& ml) const {
    std::stable_sort(ml.begin(), ml.end(), [&](Move a, Move b) {
        return move_score(pos, a) > move_score(pos, b);
    });
}

std::string Searcher::format_pv(Move first) const {
    std::string line = move_to_uci(first);
    for (int i = 1; i < pv_length[1]; i++)
        line += " " + move_to_uci(pv_table[1][i]);
    return line;
}

void Searcher::report(int depth, int line, Score score, Move first) const {
    int64_t  elapsed = elapsed_ms();
    uint64_t nps     = elapsed > 0 ? (nodes * 1000) / static_cast<uint64_t>(elapsed) : 0;

    std::cout << "info depth " << depth
              << " multipv "   << line
              << " score "     << format_score(score)
              << " nodes "     << nodes
              << " nps "       << nps
              << " time "      << elapsed
              << " pv "        << format_pv(first)
              << std::endl;
}

Score Searcher::quiesce(Position& pos, Score alpha, Score beta, int ply) {
    nodes++;
    if (check_time()) return 0;
    if (ply >= MAX_PLY) return evaluate(pos);

    Score stand_pat = evaluate(pos);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    MoveList ml = generate_captures(pos);
    order_moves(pos, ml);

    for (Move m : ml) {
        pos.do_move(m);
        Score score = -quiesce(pos, -beta, -alpha, ply + 1);
        pos.undo_move(m);

        if (stopped) return 0;
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    return alpha;
}

Score Searcher::negamax(Position& pos, Score alpha, Score beta, int depth, int ply) {
    pv_length[ply] = ply;

    nodes++;
    if (check_time()) return 0;
    if (ply >= MAX_PLY) return evaluate(pos);

    if (pos.is_repetition() || pos.fifty_move_clock() >= 100)
        return SCORE_DRAW;

    if (depth <= 0) return quiesce(pos, alpha, beta, ply);

    MoveList ml = generate_moves(pos);
    if (ml.size == 0)
        return pos.in_check() ? -SCORE_MATE + ply : SCORE_DRAW;

    order_moves(pos, ml);

    Score best_score = -SCORE_INFINITE;

    for (Move m : ml) {
        pos.do_move(m);
        Score score = -negamax(pos, -beta, -alpha, depth - 1, ply + 1);
        pos.undo_move(m);

        if (stopped) return 0;

        if (score > best_score) best_score = score;

        if (score > alpha) {
            alpha = score;

            pv_table[ply][ply] = m;
            for (int i = ply + 1; i < pv_length[ply + 1]; i++)
                pv_table[ply][i] = pv_table[ply + 1][i];
            pv_length[ply] = pv_length[ply + 1];
        }

        if (alpha >= beta) break;
    }

    return best_score;
}

void Searcher::search(Position& pos, const SearchLimits& limits) {
    init_timer(pos, limits);

    MoveList root_moves = generate_moves(pos);
    if (root_moves.size == 0) {
        std::cout << "bestmove 0000" << std::endl;
        return;
    }

    order_moves(pos, root_moves);
    root_best_move = root_moves.moves[0];

    const int lines = std::min(multipv, root_moves.size);

    for (int depth = 1; depth <= limits.depth && depth < MAX_PLY; depth++) {
        Move chosen[MAX_MOVES];
        int  chosen_count = 0;
        bool aborted      = false;

        for (int line = 1; line <= lines; line++) {
            Move  line_move  = MOVE_NONE;
            Score line_score = -SCORE_INFINITE;
            Score alpha      = -SCORE_INFINITE;

            Move line_pv[MAX_PLY + 1];
            int  line_pv_length = 1;

            for (Move m : root_moves) {
                bool already_reported = false;
                for (int i = 0; i < chosen_count; i++)
                    if (chosen[i] == m) { already_reported = true; break; }
                if (already_reported) continue;

                pos.do_move(m);
                Score score = -negamax(pos, -SCORE_INFINITE, -alpha, depth - 1, 1);
                pos.undo_move(m);

                if (stopped) { aborted = true; break; }

                if (score > line_score) {
                    line_score     = score;
                    line_move      = m;
                    line_pv_length = pv_length[1];
                    for (int i = 1; i < pv_length[1]; i++) line_pv[i] = pv_table[1][i];
                    if (lines == 1) alpha = score;
                }
            }

            if (aborted || line_move == MOVE_NONE) {
                aborted = true;
                break;
            }

            chosen[chosen_count++] = line_move;

            pv_length[1] = line_pv_length;
            for (int i = 1; i < line_pv_length; i++) pv_table[1][i] = line_pv[i];
            report(depth, line, line_score, line_move);

            if (line == 1) root_best_move = line_move;
        }

        if (aborted) break;
        if (alloc_ms > 0 && elapsed_ms() >= alloc_ms / 2) break;
    }

    std::cout << "bestmove " << move_to_uci(root_best_move) << std::endl;
}
