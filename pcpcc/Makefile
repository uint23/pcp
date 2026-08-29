.POSIX:

CC = cc
CFLAGS = -ansi -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude
TEST_CPPFLAGS = $(CPPFLAGS) -I../test

SRC = source/pcpcc.c \
      source/lexer.c \
      source/utils.c
HDR = include/common.h \
      include/lexer.h \
      include/utils.h

LEXER_TEST_SRC = ../test/test.c \
                 source/lexer.c \
                 tests/lexer_test.c
TEST_HDR = ../test/test.h \
		   include/common.h

BUILD_DIR = ../build/pcpcc
OUT = $(BUILD_DIR)/pcpcc
LEXER_TEST_OUT = $(BUILD_DIR)/lexer_test

all: $(OUT)

$(OUT): $(SRC) $(HDR) Makefile
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(OUT)

$(LEXER_TEST_OUT): $(LEXER_TEST_SRC) $(TEST_HDR) Makefile
	mkdir -p $(BUILD_DIR)
	$(CC) $(TEST_CPPFLAGS) $(CFLAGS) $(LEXER_TEST_SRC) -o $(LEXER_TEST_OUT)

test: $(LEXER_TEST_OUT)
	$(LEXER_TEST_OUT)

clean:
	rm -f $(OUT) $(LEXER_TEST_OUT)

compdb:
	@emit() { \
		for f in $$1; do \
			printf '{"directory": "%s", "file": "%s", "command": "%s %s %s -c %s"},\n' \
				"$$(pwd)" "$$f" "$(CC)" "$$2" "$(CFLAGS)" "$$f"; \
		done; \
	}; \
	emit "$(SRC)" "$(CPPFLAGS)"; \
	emit "$(LEXER_TEST_SRC)" "$(TEST_CPPFLAGS)"

.PHONY: all clean test compdb

