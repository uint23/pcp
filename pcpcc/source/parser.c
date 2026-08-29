#include <stdlib.h>

#include "ast.h"
#include "lexer.h"
#include "parser.h"
#include "utils.h"

static void advance(void);
static AST* block(void);
static void expect(TokenType type);
static AST* function(void);
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

static AST* block(void)
{
	AST* node = ast_new(AST_BLOCK);

	expect(TOK_BRACEL);
	expect(TOK_BRACER);

	return node;
}

static void expect(TokenType type)
{
	if (parser.cur.type != type)
		die(ERR_PARSE_EXPECTED_TOKEN, "Expected %s, got %s",
		    lexer_nametok(type), lexer_nametok(parser.cur.type));

	advance();
}

static AST* function(void)
{
	AST* node = ast_new(AST_FUNCTION);

	/* TODO not just int main(void) */
	expect(TOK_INT);
	node->data.function.name = parser.cur;
	expect(TOK_IDENTIFIER);
	expect(TOK_PARENL);
	expect(TOK_VOID);
	expect(TOK_PARENR);

	node->data.function.body = block();

	return node;
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
	AST* node = NULL;

	while (parser.cur.type != TOK_EOF) {
		node = function();
		ast_list_append(&root->data.root, node);
	}

	return root;
}

