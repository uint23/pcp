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
	IRBlockRef yesref;
	IRBlockRef noref;
	IRBlock* start;
	IRBlock* yes;
	IRBlock* no;

	(void) argc;
	(void) argv;

	fn.name = "main";
	fn.type = IR_I32;

	/* fn i32 main(_0: i1) */
	ir_add_param(&fn, &fn.params, IR_I1);

	startref = fn.cnt;
	ir_add_block(&fn, "start");

	yesref = fn.cnt;
	ir_add_block(&fn, "yes");

	noref = fn.cnt;
	ir_add_block(&fn, "no");

	/* Reacquire after all block allocations. */
	start = &fn.blocks[startref];
	yes = &fn.blocks[yesref];
	no = &fn.blocks[noref];

	/* br _0, yes(), no() */
	start->term.type = IR_TERM_BR;

	start->term.data.br.cond.type = IR_OPD_VALUE;
	start->term.data.br.cond.data.value = fn.params.params[0].value;

	start->term.data.br.yes.block = yesref;
	start->term.data.br.yes.args.opds = NULL;
	start->term.data.br.yes.args.cnt = 0;
	start->term.data.br.yes.args.cap = 0;

	start->term.data.br.no.block = noref;
	start->term.data.br.no.args.opds = NULL;
	start->term.data.br.no.args.cnt = 0;
	start->term.data.br.no.args.cap = 0;

	/* yes(): ret 1 */
	yes->term.type = IR_TERM_RET;
	yes->term.data.ret.hasval = 1;
	yes->term.data.ret.value.type = IR_OPD_INTEGER;
	yes->term.data.ret.value.data.integer = 1;

	/* no(): ret 0 */
	no->term.type = IR_TERM_RET;
	no->term.data.ret.hasval = 1;
	no->term.data.ret.value.type = IR_OPD_INTEGER;
	no->term.data.ret.value.data.integer = 0;

	print_function(&fn);

	free(fn.params.params);
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

