#pragma once

#include "position.h"
#include "search.h"

Move parse_uci_move(const Position& pos, const std::string& str);

std::string move_to_uci(Move m);

void uci_loop(Position& pos, Searcher& searcher);
