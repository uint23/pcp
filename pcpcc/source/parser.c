#include "ast.h"
#include "lexer.h"
#include "parser.h"

static void advance(void);
static int match(TokenType type);

static struct {
	Token cur;
	Token prv;
} parser;

static void advance(void)
{
	parser.prv = parser.cur;
	parser.cur = lexer_next();
}

static int match(TokenType type)
{
	if (type != parser.cur.type)
		return 0;

	advance();
	return 1;
}

void parser_init(SourceFile* source)
{
	lexer_init(source);
	parser.cur = lexer_next();
}

AST* parser_parse(void)
{
	AST* root = ast_new(AST_ROOT);
	return root;
}

