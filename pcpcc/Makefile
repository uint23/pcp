.POSIX:

CC = cc
CFLAGS = -ansi -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude

SRC = source/pcpcc.c \
      source/lexer.c \
      source/utils.c
HDR = include/common.h \
      include/lexer.h \
      include/utils.h

BUILD_DIR = ../build
OUT = $(BUILD_DIR)/pcpcc

all: $(OUT)

$(OUT): $(SRC) $(HDR) Makefile
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean

