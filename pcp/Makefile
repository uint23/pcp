.POSIX:

CC = cc
CFLAGS = -ansi -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude

SRC = source/pcp.c \
      source/utils.c
HDR = include/common.h \
      include/utils.h

BUILD_DIR = ../build/pcp
OUT = $(BUILD_DIR)/pcp

all: $(OUT)

$(OUT): $(SRC) $(HDR) Makefile
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $(OUT)

debug: CFLAGS += -g -O0 -DDEBUG
debug: all

clean:
	rm -f $(OUT)

compdb:
	@emit() { \
		for f in $$1; do \
			printf '{"directory": "%s", "file": "%s", "command": "%s %s %s -c %s"},\n' \
				"$$(pwd)" "$$f" "$(CC)" "$$2" "$(CFLAGS)" "$$f"; \
		done; \
	}; \
	emit "$(SRC)" "$(CPPFLAGS)"; \
	emit "$(LEXER_TEST_SRC)" "$(TEST_CPPFLAGS)"

.PHONY: all clean compdb
