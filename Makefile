CC ?= cc
CFLAGS ?= -Wall -Wextra -Werror -std=c11 -O2

INC = -Iinclude
SRC = src/poll_wrapper.c
TEST = tests/test_unix_poll_wrapper.c
TEST_BIN = tests/test_unix_poll_wrapper

.PHONY: test clean

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(INC) $(SRC) $(TEST) -o $@

clean:
	rm -f $(TEST_BIN)
