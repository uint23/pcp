#ifndef LEXER_H
#define LEXER_H

#include "common.h"

/* Every token type. Expanded into TokenType here and the name table in lexer.c. */
#define TOKEN_LIST(X) \
	X(TOK_INVALID) \
	X(TOK_EOF) \
	\
	X(TOK_IDENTIFIER) \
	X(TOK_INT_LITERAL) \
	X(TOK_FLOAT_LITERAL) \
	X(TOK_CHAR_LITERAL) \
	X(TOK_STRING_LITERAL) \
	\
	X(TOK_AUTO) \
	X(TOK_BREAK) \
	X(TOK_CASE) \
	X(TOK_CHAR) \
	X(TOK_CONST) \
	X(TOK_CONTINUE) \
	X(TOK_DEFAULT) \
	X(TOK_DO) \
	X(TOK_DOUBLE) \
	X(TOK_ELSE) \
	X(TOK_ENUM) \
	X(TOK_EXTERN) \
	X(TOK_FLOAT) \
	X(TOK_FOR) \
	X(TOK_GOTO) \
	X(TOK_IF) \
	X(TOK_INT) \
	X(TOK_LONG) \
	X(TOK_REGISTER) \
	X(TOK_RETURN) \
	X(TOK_SHORT) \
	X(TOK_SIGNED) \
	X(TOK_SIZEOF) \
	X(TOK_STATIC) \
	X(TOK_STRUCT) \
	X(TOK_SWITCH) \
	X(TOK_TYPEDEF) \
	X(TOK_UNION) \
	X(TOK_UNSIGNED) \
	X(TOK_VOID) \
	X(TOK_VOLATILE) \
	X(TOK_WHILE) \
	\
	X(TOK_BRACKL) \
	X(TOK_BRACKR) \
	X(TOK_PARENL) \
	X(TOK_PARENR) \
	X(TOK_BRACEL) \
	X(TOK_BRACER) \
	\
	X(TOK_DOT) \
	X(TOK_DOTDOTDOT) \
	X(TOK_ARROW) \
	\
	X(TOK_INCREMENT) \
	X(TOK_DECREMENT) \
	\
	X(TOK_STAR) \
	X(TOK_PLUS) \
	X(TOK_MINUS) \
	X(TOK_TILDA) \
	X(TOK_BANG) \
	X(TOK_SLASH) \
	X(TOK_PERCENT) \
	\
	X(TOK_SHL) \
	X(TOK_SHR) \
	\
	X(TOK_LT) \
	X(TOK_GT) \
	X(TOK_LE) \
	X(TOK_GE) \
	X(TOK_EQ) \
	X(TOK_NE) \
	\
	X(TOK_XOR_BIT) \
	X(TOK_OR_BIT) \
	X(TOK_AND_BIT) \
	X(TOK_OR_LOGICAL) \
	X(TOK_AND_LOGICAL) \
	\
	X(TOK_QUESTION) \
	X(TOK_COLON) \
	X(TOK_SEMICOLON) \
	\
	X(TOK_ASSIGN) \
	X(TOK_ASSIGN_MUL) \
	X(TOK_ASSIGN_DIV) \
	X(TOK_ASSIGN_MOD) \
	X(TOK_ASSIGN_ADD) \
	X(TOK_ASSIGN_SUB) \
	X(TOK_ASSIGN_SHL) \
	X(TOK_ASSIGN_SHR) \
	X(TOK_ASSIGN_XOR) \
	X(TOK_ASSIGN_AND) \
	X(TOK_ASSIGN_OR) \
	\
	X(TOK_COMMA) \
	X(TOK_HASH) \
	X(TOK_HASHHASH)

#define TOKEN_AS_ENUM(token) token,

typedef enum {
	TOKEN_LIST(TOKEN_AS_ENUM)
	TOK_UNKNOWN
} TokenType;

typedef struct {
	TokenType   type;
	const char* start;
	size_t      len;
} Token;

/* initialise internal lexer */
void lexer_init(SourceFile* source);

/* get the next token */
Token lexer_next(void);

/* return the name for token type */
const char* lexer_nametok(TokenType type);

#endif /* LEXER_H */

