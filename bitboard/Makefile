strawman: main.o board.o defs.o test.o
	g++ -std=c++2a -O2 *.cpp -lpthread

board.o: board.cpp board.h
	g++ -c -std=c++0x -O2 board.cpp -lpthread

defs.o: defs.cpp defs.h 
	g++ -c -std=c++0x -O2 defs.cpp -lpthread

main.o: main.cpp
	g++ -c -std=c++0x -O2 main.cpp -lpthread

test.o: test.h test.cpp 
	g++ -c -std=c++0x -O2 test.cpp -lpthread

clean:
	rm *.o a.out