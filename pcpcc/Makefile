.POSIX:

CC = cc
CFLAGS = -ansi -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude

SRC = source/pcpcc.c \
      source/utils.c
HDR = include/utils.h

BUILD_DIR = ../build
OUT = $(BUILD_DIR)/pcpcc

all: $(OUT)

$(OUT): $(SRC) $(HDR) Makefile
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)

.PHONY: all clean

