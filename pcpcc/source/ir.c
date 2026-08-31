#include <stdio.h>

#include "ast.h"
#include "ir.h"

static void block(AST* ast);
static void expression(AST* ast);
static void function(AST* ast);
static void statement(AST* ast);

static void block(AST* ast)
{
	size_t i;
	for (i = 0; i < ast->data.block.count; i++)
		statement(ast->data.block.items[i]);
}

static void expression(AST* ast)
{
	switch (ast->type) {
	case AST_LITERAL:
		fwrite(ast->data.token.start, 1, ast->data.token.len, stdout);
		break;

	default:
		break;
	}
}

static void function(AST* ast)
{
	printf("fn i32 ");

	fwrite(ast->data.function.name.start, 1, ast->data.function.name.len, stdout);

	printf("() {\n");
	printf("start():\n");

	block(ast->data.function.body);

	printf("}\n\n");
}

static void statement(AST* ast)
{
	switch (ast->type) {
	case AST_RETURN:
		printf("\tret ");
		expression(ast->data.return_st.expr);
		putchar('\n');
		break;

	default:
		break;
	}
}

void ir_emit(AST* ast)
{
	size_t i;

	if (!ast || ast->type != AST_ROOT)
		return;

	for (i = 0; i < ast->data.root.count; i++)
		function(ast->data.root.items[i]);
}

