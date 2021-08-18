strawman: main.o board.o defs.o game.o main.o movegen.o searcher.o
	g++ -std=c++0x -O2 *.o -lpthread

board.o: board.cpp board.h
	g++ -c -std=c++0x -O2 board.cpp -lpthread

defs.o: defs.cpp defs.h 
	g++ -c -std=c++0x -O2 defs.cpp -lpthread

game.o: game.cpp game.h 
	g++ -c -std=c++0x -O2 game.cpp -lpthread

main.o: main.cpp
	g++ -c -std=c++0x -O2 main.cpp -lpthread

movegen.o: movegen.cpp movegen.h 
	g++ -c -std=c++0x -O2 movegen.cpp -lpthread

searcher.o: searcher.cpp searcher.h
	g++ -c -std=c++0x -O2 searcher.cpp -lpthread

clean:
	rm *.o a.out