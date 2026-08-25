.POSIX:

CC = cc
CFLAGS = -ansi -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude

SRC = source/pcpcc.c \
      source/utils.c
OUT = build/pcpcc

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf build

.PHONY: all clean

