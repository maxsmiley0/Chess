#include "zobrist.h"
#include <cstdint>

namespace Zobrist {

Key piece[NUM_PIECES][NUM_SQUARES];
Key castling[16];
Key ep_file[8];
Key side;

// xorshift64 — fast, good distribution, no stdlib dependency
static uint64_t seed = 0x246C'CB2E'2B53'0B4DULL;

static uint64_t rand64() {
    seed ^= seed << 13;
    seed ^= seed >> 7;
    seed ^= seed << 17;
    return seed;
}

void init() {
    for (int p = 0; p < NUM_PIECES; p++)
        for (int s = 0; s < NUM_SQUARES; s++)
            piece[p][s] = rand64();

    for (int cr = 0; cr < 16; cr++)
        castling[cr] = rand64();

    for (int f = 0; f < 8; f++)
        ep_file[f] = rand64();

    side = rand64();
}

} // namespace Zobrist
