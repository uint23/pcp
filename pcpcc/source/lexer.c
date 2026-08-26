#include "lexer.h"

static char advance(void);
static int eof(void);
static char peek(void);

static struct {
	SourceFile* source;
	size_t      start;
	size_t      pos;
} lexer;

static char advance(void)
{
	char c = peek();

	if (!eof())
		lexer.pos++;

	return c;
}

static int eof(void)
{
	return lexer.pos >= lexer.source->len;
}

static char peek(void)
{
	if (eof())
		return '\0';

	return lexer.source->data[lexer.pos];
}

void lexer_init(SourceFile* source)
{
	lexer.source = source;
	lexer.start = 0;
	lexer.pos = 0;
}

