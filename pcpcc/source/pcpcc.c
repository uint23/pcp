#include <string.h>

#include "utils.h"

static void parse_args(int argc, char* argv[]);

static void parse_args(int argc, char* argv[])
{
	if (argc > 1) {
		if (strcmp(argv[1], "-v") == 0 ||
		    strcmp(argv[1], "--version") == 0)
			die(ERR_OK, "%s ver. %s", argv[0], PCPCC_VERSION);

		die(ERR_OK, "%s: usage\n%s", argv[0], PCPCC_USAGE);
	}
}

int main(int argc, char* argv[])
{
	parse_args(argc, argv);
	return ERR_OK;
}

