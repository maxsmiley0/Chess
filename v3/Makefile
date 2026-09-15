CXX      := g++
CXXFLAGS := -std=c++17 -O3 -march=native -flto -fno-exceptions -fno-rtti -Wall -Wextra -pthread

TARGET      := smv3
TEST_TARGET := smv3_test

SRC_DIR  := src
TEST_DIR := test
EXTERNAL := external

ENGINE_SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
MAIN_SRC    := $(SRC_DIR)/main.cpp
TEST_SRCS   := $(wildcard $(TEST_DIR)/*.cpp)

OBJS      := $(MAIN_SRC:.cpp=.o) $(ENGINE_SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o) $(ENGINE_SRCS:.cpp=.o)

OPENINGS := $(TEST_DIR)/openings.txt
BOOK     := $(TEST_DIR)/openings.epd
GAMES    ?= 100
TC       ?= 8+0.08
OPPONENT ?=
ELO      ?= 1600

TOOLS     ?= $(HOME)/tools
STOCKFISH ?= $(TOOLS)/Stockfish/src/stockfish
LADDER    ?= 1600 2000 2400 2800

BENCH_FEN   ?= rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
BENCH_DEPTH ?= 6

.PHONY: all clean test perft openings book verify bench tools match gauntlet stockfish ladder release debug help

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

release: CXXFLAGS += -DNDEBUG
release: clean $(TARGET)

debug: CXXFLAGS := -std=c++17 -O0 -g -Wall -Wextra -DDEBUG -pthread
debug: clean $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

perft: $(TEST_TARGET)
	./$(TEST_TARGET) --perft

openings book: $(TEST_TARGET)
	./$(TEST_TARGET) --openings $(OPENINGS) $(BOOK)

verify: test openings

tools:
	./scripts/get-tools.sh

match: $(TARGET) $(BOOK)
	./scripts/match.sh -g $(GAMES) -t $(TC)

gauntlet: $(TARGET) $(BOOK)
	@if [ -z "$(OPPONENT)" ]; then \
		echo "usage: make gauntlet OPPONENT=/path/to/engine [ELO=$(ELO)]"; exit 1; fi
	./scripts/match.sh -g $(GAMES) -t $(TC) -o $(OPPONENT) -e $(ELO)

stockfish: $(TARGET) $(BOOK)
	./scripts/match.sh -g $(GAMES) -t $(TC) -o $(STOCKFISH) -e $(ELO)

ladder: $(TARGET) $(BOOK)
	./scripts/ladder.sh $(LADDER) 

bench: $(TEST_TARGET)
	@./$(TEST_TARGET) --single "$(BENCH_FEN)" $(BENCH_DEPTH)

$(BOOK): $(TEST_TARGET) $(OPENINGS)
	./$(TEST_TARGET) --openings $(OPENINGS) $(BOOK) >/dev/null

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(TARGET) $(TEST_TARGET)

help:
	@echo "build"
	@echo "  make               build $(TARGET)"
	@echo "  make release       build with NDEBUG"
	@echo "  make debug         build unoptimised with symbols"
	@echo ""
	@echo "correctness"
	@echo "  make test          perft suite (node counts, movegen vs reference, zobrist)"
	@echo "  make openings      validate opening lines ply-by-ply, emit $(BOOK)"
	@echo "  make verify        test + openings"
	@echo ""
	@echo "speed"
	@echo "  make bench         perft nps at depth $(BENCH_DEPTH) from the start position"
	@echo ""
	@echo "strength (fastchess, built outside this repo)"
	@echo "  make tools         fetch+build fastchess into $(TOOLS)"
	@echo "  make match         self-play regression baseline"
	@echo "  make gauntlet      vs external engine: OPPONENT=/path [ELO=$(ELO)]"
	@echo "  make stockfish     vs $(STOCKFISH) capped at ELO=$(ELO)"
	@echo "  make ladder        vs Stockfish at each of: $(LADDER)"
	@echo ""
	@echo "variables"
	@echo "  GAMES=$(GAMES)          number of games"
	@echo "  TC=$(TC)        time control (fastchess syntax)"
	@echo "  ELO=$(ELO)         opponent strength cap for gauntlet/stockfish"
	@echo "  LADDER=$(LADDER)"
	@echo "  STOCKFISH=$(STOCKFISH)"
	@echo "  BENCH_DEPTH=$(BENCH_DEPTH)"
