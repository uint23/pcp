#ifndef AST_H
#define AST_H

#include "lexer.h"

typedef enum {
	AST_ROOT,
	AST_FUNCTION,
	AST_BLOCK,
	AST_RETURN,
	AST_IDENTIFIER,
	AST_LITERAL,
	AST_BINARY,

	AST_LAST
} ASTType;

typedef struct AST AST;
typedef struct ASTList ASTList;

struct ASTList {
	AST**  items;
	size_t count;
	size_t capacity;
};

struct AST {
	ASTType type;

	union {
		Token token;

		struct {
			TokenType op;
			AST*      left;
			AST*      right;
		} binary;

		struct {
			AST* expr;
		} return_st;

		ASTList root;
		ASTList block;

		struct {
			Token name;
			AST*  body;
		} function;
	} data;
};

/* allocate a new ast node */
AST* ast_new(ASTType type);

/* recursively destroy an ast tree from
   node provided */
void ast_destroy(AST* ast);

/* append a node to an ASTList */
void ast_list_append(ASTList* list, AST* node);

/* print AST at depth to stdout */
void ast_print(AST* ast, int depth);

#endif /* AST_H */

