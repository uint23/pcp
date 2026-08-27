#include <ctype.h>
#include <string.h>

#include "lexer.h"

static char advance(void);
static int eof(void);
static Token identifier(void);
static Token integar(void);
static Token mktok(TokenType type);
static char peek(void);
static void skipws(void);

static struct {
	SourceFile* source;
	size_t      start;
	size_t      pos;
} lexer;

static const struct {
	const char* name;
	TokenType   type;
} keywords[] = {
	{ "auto", TOK_AUTO },
	{ "break", TOK_BREAK },
	{ "case", TOK_CASE },
	{ "char", TOK_CHAR },
	{ "const", TOK_CONST },
	{ "continue", TOK_CONTINUE },
	{ "default", TOK_DEFAULT },
	{ "do", TOK_DO },
	{ "double", TOK_DOUBLE },
	{ "else", TOK_ELSE },
	{ "enum", TOK_ENUM },
	{ "extern", TOK_EXTERN },
	{ "float", TOK_FLOAT },
	{ "for", TOK_FOR },
	{ "goto", TOK_GOTO },
	{ "if", TOK_IF },
	{ "int", TOK_INT },
	{ "long", TOK_LONG },
	{ "register", TOK_REGISTER },
	{ "return", TOK_RETURN },
	{ "short", TOK_SHORT },
	{ "signed", TOK_SIGNED },
	{ "sizeof", TOK_SIZEOF },
	{ "static", TOK_STATIC },
	{ "struct", TOK_STRUCT },
	{ "switch", TOK_SWITCH },
	{ "typedef", TOK_TYPEDEF },
	{ "union", TOK_UNION },
	{ "unsigned", TOK_UNSIGNED },
	{ "void", TOK_VOID },
	{ "volatile", TOK_VOLATILE },
	{ "while", TOK_WHILE }
};

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

/* lex an identifier */
static Token identifier(void)
{
	size_t toklen = lexer.pos - lexer.start;
	size_t wordlen;
	int wordeq;
	size_t i;

	while (isalnum((unsigned char)peek()) || peek() == '_')
		advance();

	/* find keyword type */
	for (i = 0; i < LENGTH(keywords); i++) {
		wordlen = strlen(keywords[i].name);
		wordeq = strncmp(lexer.source->data + lexer.start, keywords[i].name, toklen) == 0;
		if (wordlen == toklen && wordeq)
			return mktok(keywords[i].type);
	}

	return mktok(TOK_IDENTIFIER);
}

/* lex an integar literal
   TODO error handling */
static Token integer(void)
{
	/* literal formats */
	if (lexer.source->data[lexer.start] == '0') {
		if (peek() == 'x' || peek() == 'X')
			for (advance(); isxdigit((unsigned char)peek()); advance())
				;
		else
			while (peek() >= '0' && peek() <= '7')
				advance();
	}
	else {
		while (isdigit((unsigned char)peek()))
			advance();
	}

	/* suffixes */
	if (peek() == 'u' || peek() == 'U') {
		advance();

		if (peek() == 'l' || peek() == 'L')
			advance();
	}
	else if (peek() == 'l' || peek() == 'L') {
		advance();

		if (peek() == 'u' || peek() == 'U')
			advance();
	}

	return mktok(TOK_INT_LITERAL);
}

/* convery token type to real token */
static Token mktok(TokenType type)
{
	Token t;

	t.type = type;
	t.start = lexer.source->data + lexer.start;
	t.len = lexer.pos - lexer.start;

	return t;
}

static char peek(void)
{
	if (eof())
		return '\0';

	return lexer.source->data[lexer.pos];
}

/* advance until next non-whitespace */
static void skipws(void)
{
	while (isspace((unsigned char)peek()))
		advance();
}

void lexer_init(SourceFile* source)
{
	lexer.source = source;
	lexer.start = 0;
	lexer.pos = 0;
}

Token lexer_next(void)
{
	char c;

	skipws();
	lexer.start = lexer.pos;
	if (eof())
		return mktok(TOK_EOF);

	c = advance();
	if (isalpha((unsigned char)c) || c == '_')
		return identifier();

	if (isdigit((unsigned char)c))
		return integar();

	switch (c) {
	case '[': return mktok(TOK_BRACKL);
	case ']': return mktok(TOK_BRACKR);
	case '(': return mktok(TOK_PARENL);
	case ')': return mktok(TOK_PARENR);
	case '{': return mktok(TOK_BRACEL);
	case '}': return mktok(TOK_BRACER);

	case '.': return mktok(TOK_DOT);

	case '&': return mktok(TOK_AMPERSAND);
	case '*': return mktok(TOK_STAR);
	case '+': return mktok(TOK_PLUS);
	case '-': return mktok(TOK_MINUS);
	case '~': return mktok(TOK_TILDA);
	case '!': return mktok(TOK_BANG);
	case '/': return mktok(TOK_SLASH);
	case '%': return mktok(TOK_PERCENT);

	case '<': return mktok(TOK_LT);
	case '>': return mktok(TOK_GT);

	case '^': return mktok(TOK_CARET);
	case '|': return mktok(TOK_PIPE);

	case '?': return mktok(TOK_QUESTION);
	case ':': return mktok(TOK_COLON);
	case ';': return mktok(TOK_SEMICOLON);

	case '=': return mktok(TOK_ASSIGN);

	case ',': return mktok(TOK_COMMA);
	case '#': return mktok(TOK_HASH);

	default:  return mktok(TOK_INVALID);
	}
}

const char* lexer_nametok(TokenType type)
{
	switch (type) {
	case TOK_INVALID: return "TOK_INVALID";
	case TOK_EOF: return "TOK_EOF";
	case TOK_IDENTIFIER: return "TOK_IDENTIFIER";
	case TOK_INT_LITERAL: return "TOK_INT_LITERAL";
	case TOK_FLOAT_LITERAL: return "TOK_FLOAT_LITERAL";
	case TOK_CHAR_LITERAL: return "TOK_CHAR_LITERAL";
	case TOK_STRING_LITERAL: return "TOK_STRING_LITERAL";
	case TOK_AUTO: return "TOK_AUTO";
	case TOK_BREAK: return "TOK_BREAK";
	case TOK_CASE: return "TOK_CASE";
	case TOK_CHAR: return "TOK_CHAR";
	case TOK_CONST: return "TOK_CONST";
	case TOK_CONTINUE: return "TOK_CONTINUE";
	case TOK_DEFAULT: return "TOK_DEFAULT";
	case TOK_DO: return "TOK_DO";
	case TOK_DOUBLE: return "TOK_DOUBLE";
	case TOK_ELSE: return "TOK_ELSE";
	case TOK_ENUM: return "TOK_ENUM";
	case TOK_EXTERN: return "TOK_EXTERN";
	case TOK_FLOAT: return "TOK_FLOAT";
	case TOK_FOR: return "TOK_FOR";
	case TOK_GOTO: return "TOK_GOTO";
	case TOK_IF: return "TOK_IF";
	case TOK_INT: return "TOK_INT";
	case TOK_LONG: return "TOK_LONG";
	case TOK_REGISTER: return "TOK_REGISTER";
	case TOK_RETURN: return "TOK_RETURN";
	case TOK_SHORT: return "TOK_SHORT";
	case TOK_SIGNED: return "TOK_SIGNED";
	case TOK_SIZEOF: return "TOK_SIZEOF";
	case TOK_STATIC: return "TOK_STATIC";
	case TOK_STRUCT: return "TOK_STRUCT";
	case TOK_SWITCH: return "TOK_SWITCH";
	case TOK_TYPEDEF: return "TOK_TYPEDEF";
	case TOK_UNION: return "TOK_UNION";
	case TOK_UNSIGNED: return "TOK_UNSIGNED";
	case TOK_VOID: return "TOK_VOID";
	case TOK_VOLATILE: return "TOK_VOLATILE";
	case TOK_WHILE: return "TOK_WHILE";
	case TOK_BRACKL: return "TOK_BRACKL";
	case TOK_BRACKR: return "TOK_BRACKR";
	case TOK_PARENL: return "TOK_PARENL";
	case TOK_PARENR: return "TOK_PARENR";
	case TOK_BRACEL: return "TOK_BRACEL";
	case TOK_BRACER: return "TOK_BRACER";
	case TOK_DOT: return "TOK_DOT";
	case TOK_DOTDOTDOT: return "TOK_DOTDOTDOT";
	case TOK_ARROW: return "TOK_ARROW";
	case TOK_INCREMENT: return "TOK_INCREMENT";
	case TOK_DECREMENT: return "TOK_DECREMENT";
	case TOK_AMPERSAND: return "TOK_AMPERSAND";
	case TOK_STAR: return "TOK_STAR";
	case TOK_PLUS: return "TOK_PLUS";
	case TOK_MINUS: return "TOK_MINUS";
	case TOK_TILDA: return "TOK_TILDA";
	case TOK_BANG: return "TOK_BANG";
	case TOK_SLASH: return "TOK_SLASH";
	case TOK_PERCENT: return "TOK_PERCENT";
	case TOK_SHL: return "TOK_SHL";
	case TOK_SHR: return "TOK_SHR";
	case TOK_LT: return "TOK_LT";
	case TOK_GT: return "TOK_GT";
	case TOK_LE: return "TOK_LE";
	case TOK_GE: return "TOK_GE";
	case TOK_EQ: return "TOK_EQ";
	case TOK_NE: return "TOK_NE";
	case TOK_CARET: return "TOK_CARET";
	case TOK_PIPE: return "TOK_PIPE";
	case TOK_AND: return "TOK_AND";
	case TOK_OR: return "TOK_OR";
	case TOK_QUESTION: return "TOK_QUESTION";
	case TOK_COLON: return "TOK_COLON";
	case TOK_SEMICOLON: return "TOK_SEMICOLON";
	case TOK_ASSIGN: return "TOK_ASSIGN";
	case TOK_ASSIGN_MUL: return "TOK_ASSIGN_MUL";
	case TOK_ASSIGN_DIV: return "TOK_ASSIGN_DIV";
	case TOK_ASSIGN_MOD: return "TOK_ASSIGN_MOD";
	case TOK_ASSIGN_ADD: return "TOK_ASSIGN_ADD";
	case TOK_ASSIGN_SUB: return "TOK_ASSIGN_SUB";
	case TOK_ASSIGN_SHL: return "TOK_ASSIGN_SHL";
	case TOK_ASSIGN_SHR: return "TOK_ASSIGN_SHR";
	case TOK_ASSIGN_XOR: return "TOK_ASSIGN_XOR";
	case TOK_ASSIGN_AND: return "TOK_ASSIGN_AND";
	case TOK_ASSIGN_OR: return "TOK_ASSIGN_OR";
	case TOK_COMMA: return "TOK_COMMA";
	case TOK_HASH: return "TOK_HASH";
	case TOK_HASHHASH: return "TOK_HASHHASH";
	case TOK_LAST: return "TOK_LAST";
	}
}

