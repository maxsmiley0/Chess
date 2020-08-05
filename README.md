# Chess
A chess engine coded from scratch, in c++

Skills Demonstrated: Memory management, custom data structures implementation, legal and pseudolegal move generation, class invariants, minimax, alpha-beta pruning, priority move ordering, transposition tables, zobrist hashing

Front End:
Upon booting up the program, the user will be prompted to pick a side, "White" or "Black". The board is represented by text characters in an 8x8 grid. For example, a white bishop is represented as "WB".
   
To input a move, type in four characters, where the first two represent the coordinate of the piece you want to move, and the last two represent the coordinate you want to move that piece to. Press enter to submit your move. If this is an illegal move, you will be asked again to input another move. If you type in 'exit', the game quits. Example moves: "e2e4" "g1f3" "h2b8"

Unimplemented Features: 
En passant does not exist in this version of chess
My engine will not recognize a draw after 3 repetitions, nor does it adhere to the FIDE 50-move rule
