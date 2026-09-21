CC ?= cc
CFLAGS ?= -Wall -Wextra -Werror -std=c11 -O2

INC = -I/home/runner/work/poll_wrapper2/poll_wrapper2/include
SRC = /home/runner/work/poll_wrapper2/poll_wrapper2/src/poll_wrapper.c
TEST = /home/runner/work/poll_wrapper2/poll_wrapper2/tests/test_unix_poll_wrapper.c

.PHONY: test clean

test: /home/runner/work/poll_wrapper2/poll_wrapper2/tests/test_unix_poll_wrapper
	/home/runner/work/poll_wrapper2/poll_wrapper2/tests/test_unix_poll_wrapper

/home/runner/work/poll_wrapper2/poll_wrapper2/tests/test_unix_poll_wrapper: $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(INC) $(SRC) $(TEST) -o $@

clean:
	rm -f /home/runner/work/poll_wrapper2/poll_wrapper2/tests/test_unix_poll_wrapper
