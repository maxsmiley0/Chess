strawman:
	g++ -o strawman.out -std=c++2a -O2 src/*.cpp -lpthread

tester: 
	g++ -o tester.out -std=c++2a -O2 src/defs.cpp src/board.cpp test/test.cpp test/main.cpp -lpthread

clean:
	rm *.out