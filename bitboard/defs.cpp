#include "defs.h"
#include "board.h"


//Prints board to cout
void print_board(Brd brd)
{
    //Prints ranks, pieces, files
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
            std::cout << pce_on_square(square, brd);
            //get_bit(bitboard, square) ? std::cout << 1 : std::cout << 0;
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    //Prints auxiliary information
    std::cout << std::endl << "      a  b  c  d  e  f  g  h" << std::endl << std::endl;
    brd.color ? std::cout << "      Color: Black" : std::cout << "      Color: White" << std::endl;
    std::cout << "Castle Perms: ";
    if (brd.castleperms & WKCA)
    {
        std::cout << "K";
    }
    if (brd.castleperms & WQCA)
    {
        std::cout << "Q";
    }
    if (brd.castleperms & BKCA)
    {
        std::cout << "k";
    }
    if (brd.castleperms & BQCA)
    {
        std::cout << "q";
    }
    std::cout << std::endl;
    std::cout << "Enpas: " << brd.enpas << std::endl << std::endl;
}

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

void print_move(int move)
{
    std::cout << "From: " << square_to_coordinates[get_move_source(move)] << std::endl;
    std::cout << "To: " << square_to_coordinates[get_move_target(move)] << std::endl;
    std::cout << "Piece: " << pce_char[get_move_piece(move)] << std::endl;
    std::cout << "Move Key " << move << std::endl << std::endl;
}

void print_moves(std::list<int> moves)
{
    for (std::list<int>::iterator itr = moves.begin(); itr != moves.end(); itr++)
    {
        print_move(*itr);
    }
}