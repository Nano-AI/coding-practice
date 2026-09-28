CC ?= cc
CFLAGS ?= -std=gnu11 -Wall -Wextra -g
SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
SRC := main.c ../hashmap/hashmap.c

test: $(SRC) ../hashmap/hashmap.h
	$(CC) $(CFLAGS) $(SAN) $(SRC) -o test && ./test

leaks: $(SRC) ../hashmap/hashmap.h
	$(CC) $(CFLAGS) $(SRC) -o test_leaks && MallocStackLogging=1 leaks --atExit --quiet -- ./test_leaks

clean:
	rm -rf test test_leaks *.dSYM a.out

.PHONY: test leaks clean
