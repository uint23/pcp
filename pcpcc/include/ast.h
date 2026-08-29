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

		struct {
			AST**  items;
			size_t count;
		} block;

		struct {
			AST**  items;
			size_t count;
		}root;

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

#endif /* AST_H */

