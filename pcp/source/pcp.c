#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "ir.h"
#include "print.h"
#include "utils.h"

#ifndef PCP_VERSION
#define PCP_VERSION "Unsure.."
#endif /* PCP_VERSION */

#define PCP_USAGE \
	"\t[-v|--version]: Show pcp version" \
	"\n"

static void parse_args(int argc, char* argv[]);
static void open_source(SourceFile* source, const char* path);
static void read_source(SourceFile* source);
static void close_source(SourceFile* source);

static void parse_args(int argc, char* argv[])
{
	if (argc < 2)
		die(ERR_OK, "%s: usage\n%s", argv[0], PCP_USAGE);

	if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
		die(ERR_OK, "%s ver. %s", argv[0], PCP_VERSION);
}

static void open_source(SourceFile* source, const char* path)
{
	long len;
	source->path = path;
	source->file = fopen(source->path, "rb");
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
	IRFunction fn = { 0 };
	IRBlockRef startref;
	IRBlockRef endref;
	IRBlock* start;
	IRBlock* end;
	IROperand* opd;

	fn.name = "main";
	fn.type = IR_I32;

	startref = fn.cnt;
	ir_add_block(&fn, "start");

	endref = fn.cnt;
	ir_add_block(&fn, "end");

	start = &fn.blocks[startref];
	end = &fn.blocks[endref];

	ir_add_param(&fn, &end->params, IR_I32);

	start->term.type = IR_TERM_JMP;
	start->term.data.jmp.block = endref;
	start->term.data.jmp.args.opds = NULL;
	start->term.data.jmp.args.cnt = 0;
	start->term.data.jmp.args.cap = 0;

	opd = ir_add_operand(&start->term.data.jmp.args);
	opd->type = IR_OPD_INTEGER;
	opd->data.integer = 123;

	end->term.type = IR_TERM_RET;
	end->term.data.ret.hasval = 1;
	end->term.data.ret.value.type = IR_OPD_VALUE;
	end->term.data.ret.value.data.value = end->params.params[0].value;

	print_function(&fn);

	free(start->term.data.jmp.args.opds);
	free(end->params.params);
	free(fn.blocks);

	return ERR_OK;
	(void) parse_args;
	(void) open_source;
	(void) read_source;
	(void) close_source;
	(void) argc;
	(void) argv;
#if 0
	SourceFile source = { 0 };

	parse_args(argc, argv);
	open_source(&source, argv[1]);
	read_source(&source);

	close_source(&source);
	return ERR_OK;
#endif
}

