#pragma once

#include "types.h"

namespace Zobrist {
    extern Key piece[NUM_PIECES][NUM_SQUARES];
    extern Key castling[16];
    extern Key ep_file[8];
    extern Key side;

    void init();
}
