.POSIX:

ROOT_BUILD_DIR = $(CURDIR)/build

all: pcpcc

pcpcc:
	cd pcpcc && $(MAKE) BUILD_DIR="$(ROOT_BUILD_DIR)" all

clean:
	cd pcpcc && $(MAKE) BUILD_DIR="$(ROOT_BUILD_DIR)" clean
	rm -rf "$(ROOT_BUILD_DIR)"

.PHONY: all clean pcpcc
