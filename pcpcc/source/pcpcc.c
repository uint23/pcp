#include <stdio.h>
#include <string.h>

#include "utils.h"

static void parse_args(int argc, char* argv[]);
static void open_sources(void);
static void close_sources(void);

FILE* src_file;
char* src_file_path;

static void parse_args(int argc, char* argv[])
{
	if (argc < 2)
		die(ERR_OK, "%s: usage\n%s", argv[0], PCPCC_USAGE);

	if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
		die(ERR_OK, "%s ver. %s", argv[0], PCPCC_VERSION);

	src_file_path = argv[1];
}

static void open_sources(void)
{
	int c;

	src_file = fopen(src_file_path, "r");
	if (!src_file)
		die(ERR_SRC_FOPEN, "Failed to open file %s", src_file_path);

	while ((c = fgetc(src_file)) != EOF)
		putchar(c);
}

static void close_sources(void)
{
	if (src_file)
		fclose(src_file);
}

int main(int argc, char* argv[])
{
	parse_args(argc, argv);
	open_sources();
	close_sources();
	return ERR_OK;
}

