CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -g
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
SRC := hashmap.c main.c

test: $(SRC) hashmap.h
	$(CC) $(CFLAGS) $(SAN) $(SRC) -o test && ./test

leaks: $(SRC) hashmap.h
	$(CC) $(CFLAGS) $(SRC) -o test_leaks && MallocStackLogging=1 leaks --atExit --quiet -- ./test_leaks

clean:
	rm -rf test test_leaks *.dSYM *.pch a.out

.PHONY: test leaks clean
