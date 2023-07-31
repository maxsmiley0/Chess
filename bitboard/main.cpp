#include "board.h"

int main()
{
    Brd brd = parse_fen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    std::cout << brd.castleperms << std::endl;
    Timer t1;
    t1.start();
    std::cout << perft_driver(brd, 5) << std::endl;
    std::cout << t1.elapsed() << std::endl;
}