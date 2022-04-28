#include "defs.h"

void print_bitboard(map bitboard)
{
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            //Convert file & rank into square index
            int square = rank * 8 + file;

            //print ranks
            if (!file)
            {
                std::cout << "  " << 8 - rank << "  ";
            }

            //shifts left to get the nth bit value, prints 1 if hits 0 if misses
            std::cout << " ";
            get_bit(bitboard, square) ? std::cout << 1 : std::cout << 0;
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl << "      a  b  c  d  e  f  g  h" << std::endl << std::endl;
    std::cout << "      Bitboard: " << bitboard << std::endl << std::endl;
}