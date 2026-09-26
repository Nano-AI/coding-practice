CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wno-unused-parameter -g -fsanitize=address,undefined -fno-omit-frame-pointer

test: test.cpp solution.h
	$(CXX) $(CXXFLAGS) test.cpp -o test && ./test ../cases.txt

clean:
	rm -rf test test.dSYM

.PHONY: test clean
