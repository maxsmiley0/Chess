Executing "strawberrymangoes.out" in console will load a prebuilt executable of the files in the source code folder.

When prompted for input, enter the square of the piece you want to move, and the square you want to move it to. For example, moving your king pawn up two spaces for the first move is denoted "e2e4".

The keyword "#exit" as input exits the program. A lowercase letter represents a black piece, while an uppercase letter represents a white piece.

Debug Mode: Setting this equal to true displays additional information pertaining to the board / search, such as nodes searched, castle / en passant permissions, etc.

Computer Mode: Two instances of strawberry mangoes with computer mode set to true, and inverted in / out sockets with a game of opposite colors and same start fen will automatically play each other.

Ponder Mode: Setting this equal to true will make the computer use the time the human spends thinking to perform additional calculations.

Estimated elo: ~2400

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
Pondering
NNUE evaluation

To be implemented?
Revamp board interface to be implemented with bit-boards
More efficient legal move generation
Opening book (run alpha beta for a long time and cache depth)
Transposition table
Aspiration windows
Multicut pruning
Mate killer pruning
Null move pruning
