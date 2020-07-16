#ifndef Game_h
#define Game_h

class Board;

class Game
{
public:
    Game(Board* b);
    ~Game();
     
    Board* getBoard() const;
    //returns board
    void play();
    //loops into play
    //bool reccomendMove(int n);
    //reccomends a move by evaluating every possible position n moves ahead
    void getMove();
    //allows player to input move
    bool gameOver();
    //if in check and no legal moves, then the game is over
    
    
private:
    struct Tuple
    {
        double value;
        double time;
    };
    
    Board* mBoard;
    bool isValid(string s); //returns true if it is a valid move (syntactically!)
    char playerColor;
    
    Coord convert(char c1, char c2); //converts into a coord
    //a list of tuples used for my analysis of algorithm time
    list<Tuple> data;
};

#endif /* Game_h */
