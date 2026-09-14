#pragma once

#include "position.h"
#include "search.h"

// Parse a move in UCI coordinate notation (e.g. "e2e4", "e7e8q")
// into a Move value. Returns MOVE_NONE if the string is invalid.
Move parse_uci_move(const Position& pos, const std::string& str);

// Serialize a Move to UCI coordinate notation
std::string move_to_uci(Move m);

// Main UCI loop — reads stdin, drives the engine
void uci_loop(Position& pos, Searcher& searcher);
