#include <stdlib.h>

#include "ast.h"
#include "utils.h"

AST* ast_new(ASTType type)
{
	AST* ast = calloc(1, sizeof(*ast));
	if (!ast)
		die(ERR_ALLOC, "Failed to allocate AST node");

	ast->type = type;

	return ast;
}

void ast_destroy(AST* ast)
{
	size_t i;

	if (!ast)
		return;

	switch (ast->type) {
	case AST_ROOT:
		for (i = 0; i < ast->data.root.count; i++)
			ast_destroy(ast->data.root.items[i]);
		free(ast->data.root.items);
		break;

	case AST_FUNCTION:
		ast_destroy(ast->data.function.body);
		break;

	case AST_BLOCK:
		for (i = 0; i < ast->data.block.count; i++)
			ast_destroy(ast->data.block.items[i]);
		free(ast->data.block.items);
		break;

	case AST_RETURN:
		ast_destroy(ast->data.return_st.expr);
		break;

	case AST_BINARY:
		ast_destroy(ast->data.binary.left);
		ast_destroy(ast->data.binary.right);
		break;

	case AST_IDENTIFIER:
	case AST_LITERAL:
	default:
		break;
	}

	free(ast);
}

void ast_list_append(ASTList* list, AST* node)
{
	AST** items;
	size_t capacity;

	if (list->count == list->capacity) {
		capacity = list->capacity ? list->capacity * 2 : 4;

		items = realloc(list->items, capacity * sizeof(*list->items));
		if (!items)
			die(ERR_ALLOC, "Failed to resize AST list");

		list->items = items;
		list->capacity = capacity;
	}

	list->items[list->count++] = node;
}

void ast_print(AST* ast, int depth)
{
	size_t i;
	int j;

	if (!ast)
		return;

	for (j = 0; j < depth; j++)
		printf("  ");

	switch (ast->type) {
	case AST_ROOT:
		printf("ROOT\n");
		for (i = 0; i < ast->data.root.count; i++)
			ast_print(ast->data.root.items[i], depth + 1);
		break;

	case AST_FUNCTION:
		printf("FUNCTION ");
		fwrite(ast->data.function.name.start, 1,
		       ast->data.function.name.len, stdout);
		putchar('\n');
		ast_print(ast->data.function.body, depth + 1);
		break;

	case AST_BLOCK:
		printf("BLOCK\n");
		for (i = 0; i < ast->data.block.count; i++)
			ast_print(ast->data.block.items[i], depth + 1);
		break;

	default:
		printf("UNKNOWN\n");
		break;
	}
}


