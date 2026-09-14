#include "bitboard.h"
#include "position.h"
#include "search.h"
#include "uci.h"
#include "zobrist.h"

int main() {
    Zobrist::init();
    init_bitboards();

    Position pos;
    pos.set(START_FEN);

    Searcher searcher;
    uci_loop(pos, searcher);

    return 0;
}
