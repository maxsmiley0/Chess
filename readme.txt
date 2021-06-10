Executing "strawberrymangoes.out" in console will load a prebuilt executable of the files in the source code folder.

When prompted for input, enter the square of the piece you want to move, and the square you want to move it to. For example, moving your king pawn up two spaces for the first move is denoted "e2e4".

The keyword "#take" as input takes back the previous made move. The keyword "#exit" as input exits the program. A lowercase letter represents a black piece, while an uppercase letter represents a white piece.

Estimated elo: ~2000 (99.86 percentile, according to Chess.com)

Implemented:
Zobrist hashing
Use of bitwise and shifting operators
Legal move generation
Position and move keys
Minimax (negamax)
Alpha beta pruning
Static evaluation function
Quiescence search
Standing pat
Search extension (check extension)
Iterative deepening
Move ordering
Principal variation table
MVV LVA
Killer heuristic
History heuristic
Evaluation extension
Improved move ordering sorting algorithm

To be implemented?
Revamp board interface to be implemented with bit-boards
More efficient legal move generation (priority queue..? study this)
Opening book (run alpha beta for a long time and cache depth
Tune parameters
Transposition table
Aspiration windows
Multicut pruning
Mate killer pruning
Null move pruning
