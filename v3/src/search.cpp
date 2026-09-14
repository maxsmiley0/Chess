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
    start_ms = now_ms();
    stopped  = false;
    nodes    = 0;

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

    alloc_ms = remaining > 0 ? std::max(1, remaining / 20 + increment / 2) : 0;
}

bool Searcher::check_time() {
    if (alloc_ms == 0) return false;
    if ((nodes & 2047) == 0 && (now_ms() - start_ms) >= alloc_ms) {
        stopped = true;
        return true;
    }
    return stopped;
}

int Searcher::move_score(const Position& pos, Move m, Move first_move) const {
    if (m == first_move) return 1000000;

    if (move_flag(m) == EN_PASSANT) return 100000 + MVV_LVA[PAWN][PAWN];

    if (move_is_capture(m)) {
        PieceType attacker = piece_type(pos.piece_on(move_from(m)));
        PieceType victim   = piece_type(pos.piece_on(move_to(m)));
        return 100000 + MVV_LVA[attacker][victim];
    }

    if (move_is_promo(m)) return 90000 + promo_piece_type(m);

    return 0;
}

void Searcher::order_moves(const Position& pos, MoveList& ml, Move first_move) const {
    std::sort(ml.begin(), ml.end(), [&](Move a, Move b) {
        return move_score(pos, a, first_move) > move_score(pos, b, first_move);
    });
}

Score Searcher::quiesce(Position& pos, Score alpha, Score beta, int ply) {
    nodes++;
    if (check_time()) return 0;
    if (ply >= MAX_PLY) return evaluate(pos);

    Score stand_pat = evaluate(pos);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    MoveList ml = generate_captures(pos);
    order_moves(pos, ml, MOVE_NONE);

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
    nodes++;
    if (check_time()) return 0;

    if (ply > 0 && (pos.is_repetition() || pos.fifty_move_clock() >= 100))
        return SCORE_DRAW;

    if (depth <= 0) return quiesce(pos, alpha, beta, ply);

    MoveList ml = generate_moves(pos);

    if (ml.size == 0)
        return pos.in_check() ? -SCORE_MATE + ply : SCORE_DRAW;

    Move  best_move  = MOVE_NONE;
    Score best_score = -SCORE_INFINITE;

    order_moves(pos, ml, ply == 0 ? root_best_move : MOVE_NONE);

    for (Move m : ml) {
        pos.do_move(m);
        Score score = -negamax(pos, -beta, -alpha, depth - 1, ply + 1);
        pos.undo_move(m);

        if (stopped) return 0;

        if (score > best_score) {
            best_score = score;
            best_move   = m;
        }
        if (score > alpha) alpha = score;
        if (alpha >= beta) break;
    }

    if (ply == 0 && best_move != MOVE_NONE) {
        root_best_move  = best_move;
        root_best_score = best_score;
    }

    return best_score;
}

void Searcher::search(Position& pos, const SearchLimits& limits) {
    init_timer(pos, limits);
    root_best_move  = MOVE_NONE;
    root_best_score = 0;

    Move  final_move  = MOVE_NONE;
    Score final_score = 0;

    for (int depth = 1; depth <= limits.depth && depth < MAX_PLY; depth++) {
        Score score = negamax(pos, -SCORE_INFINITE, SCORE_INFINITE, depth, 0);

        if (stopped) break;

        final_move  = root_best_move;
        final_score = score;

        int64_t elapsed = now_ms() - start_ms;
        int64_t nps     = elapsed > 0 ? (nodes * 1000) / elapsed : 0;

        std::cout << "info depth " << depth
                  << " score "     << format_score(final_score)
                  << " nodes "     << nodes
                  << " nps "       << nps
                  << " time "      << elapsed
                  << " pv "        << move_to_uci(final_move)
                  << std::endl;

        if (alloc_ms > 0 && (now_ms() - start_ms) >= alloc_ms / 2) break;
    }

    if (final_move == MOVE_NONE) final_move = root_best_move;

    std::cout << "bestmove " << move_to_uci(final_move) << std::endl;
}
