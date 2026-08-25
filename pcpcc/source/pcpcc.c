#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct {
	const char* path;
	FILE*       file;
	char*       data;
	size_t      len;
} SourceFile;

static void parse_args(int argc, char* argv[]);
static void open_source(SourceFile* source, const char* path);
static void read_source(SourceFile* source);
static void close_source(SourceFile* source);

static void parse_args(int argc, char* argv[])
{
	if (argc < 2)
		die(ERR_OK, "%s: usage\n%s", argv[0], PCPCC_USAGE);

	if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
		die(ERR_OK, "%s ver. %s", argv[0], PCPCC_VERSION);
}

static void open_source(SourceFile* source, const char* path)
{
	long len;
	source->path = path;
	source->file = fopen(source->path, "r");
	if (!source->file)
		die(ERR_FOPEN, "Failed to open file: %s", source->path);

	/* get file length */
	if (fseek(source->file, 0, SEEK_END) != 0)
		die(ERR_FSEEK, "Failed to seek to end of file: %s", source->path);

	len = ftell(source->file);
	if (len < 0)
		die(ERR_FTELL, "Failed to get file size: %s", source->path);
	source->len = (size_t) len;

	if (fseek(source->file, 0, SEEK_SET) != 0)
		die(ERR_FSEEK, "Failed to seek to start of file: %s", source->path);
}

static void read_source(SourceFile* source)
{
	size_t nread;

	/* copy file contents */
	source->data = malloc(source->len + 1);
	if (!source->data)
		die(ERR_ALLOC, "Failed to allocate data buffer: %s", source->path);

	/* check source->data size is same as file len */
	nread = fread(source->data, 1, source->len, source->file);
	if (nread != source->len)
		die(ERR_SRC_DATA_LEN_DIFFERENT, "Failed to read whole file: %s", source->path);

	source->data[source->len] = '\0';
}

static void close_source(SourceFile* source)
{
	if (source->file)
		fclose(source->file);

	if (source->data)
		free(source->data);
}

int main(int argc, char* argv[])
{
	SourceFile source = { 0 };

	parse_args(argc, argv);
	open_source(&source, argv[1]);
	read_source(&source);
	printf("%s", source.data);
	close_source(&source);

	return ERR_OK;
}

