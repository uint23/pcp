.POSIX:

all: compiler

compiler:
	cd pcpcc && $(MAKE) all

clean:
	cd pcpcc && $(MAKE) clean
	rm -rf build/

.PHONY: all clean compiler

