#include "board.h"
#include "nnue/nnue.h"

int main()
{
    init_keys();
    nnue_init("src/nnue/nn-04cf2b4ed1da.nnue");

    Brd brd = parse_fen("2k4r/p1pb1ppp/1pnp1n2/1B2p2q/2P1P2r/2RP1N2/PP2QPPP/1NB2RK1 b k - 0 1");
    std::cout << static_eval(brd) << std::endl;
}