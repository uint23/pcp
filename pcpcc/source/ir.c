#include <stdio.h>

#include "ast.h"
#include "ir.h"

static void block(AST* ast);
static int expression(AST* ast);
static void function(AST* ast);
static void operand(AST* ast, int val);
static void statement(AST* ast);

static int nextval = 0;

static void block(AST* ast)
{
	size_t i;
	for (i = 0; i < ast->data.block.count; i++)
		statement(ast->data.block.items[i]);
}

/* parses expression type
   returns -1 if literal
   else it returns ssa value */
static int expression(AST* ast)
{
	int left;
	int right;
	int val;

	switch (ast->type) {
	case AST_LITERAL:
		return -1;

	case AST_BINARY:
		left = expression(ast->data.binary.left);
		right = expression(ast->data.binary.right);
		val = nextval++;

		printf("\t_%d: i32 = ", val);
		switch (ast->data.binary.op) {
		case TOK_PLUS:
			printf("add ");
			break;
		case TOK_MINUS:
			printf("sub ");
			break;
		case TOK_STAR:
			printf("mul ");
			break;
		default:
			return -1;
		}

		operand(ast->data.binary.left, left);
		printf(", ");
		operand(ast->data.binary.right, right);
		putchar('\n');

		return val;

	default:
		return -1;
	}
}

static void function(AST* ast)
{
	nextval = 0;
	printf("fn i32 ");

	fwrite(ast->data.function.name.start, 1, ast->data.function.name.len, stdout);

	printf("() {\n");
	printf("start():\n");

	block(ast->data.function.body);

	printf("}\n\n");
}

static void operand(AST* ast, int val)
{
	if (val >= 0) {
		printf("_%d", val);
		return;
	}

	fwrite(ast->data.token.start, 1, ast->data.token.len, stdout);
}

static void statement(AST* ast)
{
	int val;

	switch (ast->type) {
	case AST_RETURN:
		val = expression(ast->data.return_st.expr);
		printf("\tret ");
		operand(ast->data.return_st.expr, val);
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

