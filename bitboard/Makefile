strawman:
	g++ -o strawman.out -std=c++2a -O2 src/*.cpp src/nnue/nnue.cpp -lpthread

tester: 
	g++ -o tester.out -std=c++2a -O2 src/defs.cpp src/board.cpp test/test.cpp test/main.cpp src/nnue/nnue.cpp -lpthread

clean:
	rm *.out