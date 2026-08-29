.POSIX:

all: compiler

compiler:
	cd pcpcc && $(MAKE) all

test:
	cd pcpcc && $(MAKE) test

clean:
	cd pcpcc && $(MAKE) clean
	rm -rf build/

.PHONY: all clean compiler test

