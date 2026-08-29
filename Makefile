.POSIX:

SUBPROJECTS = pcpcc

all: compiler

compiler:
	cd pcpcc && $(MAKE) all

test:
	cd pcpcc && $(MAKE) test

compile_commands.json:
	@( for dir in $(SUBPROJECTS); do \
		(cd $$dir && $(MAKE) compdb) | grep '^{'; \
	done ) | sed '$$ s/,$$//' | ( printf '[\n'; cat; printf '\n]\n' ) > compile_commands.json

clean:
	cd pcpcc && $(MAKE) clean
	rm -rf build/

.PHONY: all clean compiler test compile_commands.json

