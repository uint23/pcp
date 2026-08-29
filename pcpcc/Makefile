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
	             tests/lexer_test.c
TEST_HDR = ../test/test.h

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

.PHONY: all clean test

