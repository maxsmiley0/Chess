CXX      := g++
CXXFLAGS := -std=c++17 -O3 -march=native -flto -fno-exceptions -fno-rtti -Wall -Wextra

TARGET      := smv3
TEST_TARGET := smv3_test

SRC_DIR  := src
TEST_DIR := test

ENGINE_SRCS := $(filter-out $(SRC_DIR)/main.cpp, $(wildcard $(SRC_DIR)/*.cpp))
MAIN_SRC    := $(SRC_DIR)/main.cpp
TEST_SRCS   := $(wildcard $(TEST_DIR)/*.cpp)

OBJS      := $(MAIN_SRC:.cpp=.o) $(ENGINE_SRCS:.cpp=.o)
TEST_OBJS := $(TEST_SRCS:.cpp=.o) $(ENGINE_SRCS:.cpp=.o)

.PHONY: all clean test perft bench release debug

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

release: CXXFLAGS += -DNDEBUG
release: clean $(TARGET)

debug: CXXFLAGS := -std=c++17 -O0 -g -Wall -Wextra -DDEBUG
debug: clean $(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

perft: $(TEST_TARGET)
	./$(TEST_TARGET) --perft

bench: $(TARGET)
	./scripts/bench.sh

clean:
	rm -f $(SRC_DIR)/*.o $(TEST_DIR)/*.o $(TARGET) $(TEST_TARGET)
