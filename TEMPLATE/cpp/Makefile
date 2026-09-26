CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wno-unused-parameter -g -fsanitize=address,undefined -fno-omit-frame-pointer
SRC := test.cpp $(wildcard solution.cpp)

test: $(SRC) solution.h
	$(CXX) $(CXXFLAGS) $(SRC) -o test && ./test ../cases.txt

clean:
	rm -rf test test.dSYM

.PHONY: test clean
