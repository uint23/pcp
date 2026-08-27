#include <ctype.h>
#include <string.h>

#include "lexer.h"

#define TOKEN_NAME_CASE(token) case token: return #token

static char advance(void);
static Token character(void);
static void comment(void);
static int eof(void);
static Token identifier(void);
static int match(char c);
static Token mktok(TokenType type);
static Token number(void);
static char peek(void);
static void skipws(void);
static Token string(void);

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

/* lex character literals */
static Token character(void)
{
	while (!eof() && peek() != '\'') {
		if (peek() == '\\')
			advance();

		if (!eof())
			advance();
	}

	if (eof())
		return mktok(TOK_INVALID);

	advance();
	return mktok(TOK_CHAR_LITERAL);
}

/* get end half of a comment; the first half is
   handled in lexer_next */
static void comment(void)
{
	advance(); /* consoom '*' */

	while (!eof()) {
		if (peek() == '*' && lexer.source->data[lexer.pos + 1] == '/') {
			advance();
			advance();
			return;
		}

		advance();
	}
}

static int eof(void)
{
	return lexer.pos >= lexer.source->len;
}

/* lex an identifier */
static Token identifier(void)
{
	size_t toklen;
	size_t wordlen;
	int wordeq;
	size_t i;

	while (isalnum((unsigned char)peek()) || peek() == '_')
		advance();

	/* find keyword type */
	toklen = lexer.pos - lexer.start;
	for (i = 0; i < LENGTH(keywords); i++) {
		wordlen = strlen(keywords[i].name);
		wordeq = strncmp(lexer.source->data + lexer.start, keywords[i].name, toklen) == 0;
		if (wordlen == toklen && wordeq)
			return mktok(keywords[i].type);
	}

	return mktok(TOK_IDENTIFIER);
}

/* check if current char is c */
static int match(char c)
{
	if (peek() != c)
		return 0;

	advance();
	return 1;
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

/* lex a number (ints or floats)
   TODO bad value handling */
static Token number(void)
{
	int isfloat = 0;

	/* hexadecimal */
	if (lexer.source->data[lexer.start] == '0' &&
	   (peek() == 'x' || peek() == 'X')) {
		advance();

		while (isxdigit((unsigned char)peek()))
			advance();
	}
	else {
		/* started with '.' */
		if (lexer.source->data[lexer.start] == '.')
			isfloat = 1;

		while (isdigit((unsigned char)peek()))
			advance();

		if (peek() == '.') {
			isfloat = 1;
			advance();

			while (isdigit((unsigned char)peek()))
				advance();
		}

		/* scientific */
		if (peek() == 'e' || peek() == 'E') {
			isfloat = 1;
			advance();

			if (peek() == '+' || peek() == '-')
				advance();

			while (isdigit((unsigned char)peek()))
				advance();
		}
	}

	/* floating point suffixes */
	if (isfloat) {
		if (peek() == 'f' || peek() == 'F' ||
		    peek() == 'l' || peek() == 'L')
			advance();

		return mktok(TOK_FLOAT_LITERAL);
	}

	/* integar suffixes */
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

static Token string(void)
{
	while (!eof() && peek() != '"') {
		/* ignore escaped " */
		if (peek() == '\\')
			advance();

		if (!eof())
			advance();
	}

	if (eof())
		return mktok(TOK_INVALID);

	advance();
	return mktok(TOK_STRING_LITERAL);
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
		return number();

	switch (c) {

	/* single character tokens */
	case '[': return mktok(TOK_BRACKL);
	case ']': return mktok(TOK_BRACKR);
	case '(': return mktok(TOK_PARENL);
	case ')': return mktok(TOK_PARENR);
	case '{': return mktok(TOK_BRACEL);
	case '}': return mktok(TOK_BRACER);

	case '~': return mktok(TOK_TILDA);

	case '?': return mktok(TOK_QUESTION);
	case ':': return mktok(TOK_COLON);
	case ';': return mktok(TOK_SEMICOLON);

	case ',': return mktok(TOK_COMMA);

	case '\'': return character();
	case '"':  return string();

	/* multi character tokens */
	case '.':
		/* .N format float */
		if (isdigit((unsigned char)peek()))
			return number();

		if (peek() == '.' && lexer.pos + 1 < lexer.source->len &&
		    lexer.source->data[lexer.pos + 1] == '.') {
			advance();
			advance();
			return mktok(TOK_DOTDOTDOT);
		}

		return mktok(TOK_DOT);

	case '&':
		if (match('&'))
			return mktok(TOK_AND_LOGICAL);
		if (match('='))
			return mktok(TOK_ASSIGN_AND);
		return mktok(TOK_AND_BIT);

	case '*':
		if (match('='))
			return mktok(TOK_ASSIGN_MUL);
		return mktok(TOK_STAR);

	case '+':
		if (match('+'))
			return mktok(TOK_INCREMENT);
		if (match('='))
			return mktok(TOK_ASSIGN_ADD);
		return mktok(TOK_PLUS);

	case '-':
		if (match('-'))
			return mktok(TOK_DECREMENT);
		if (match('>'))
			return mktok(TOK_ARROW);
		if (match('='))
			return mktok(TOK_ASSIGN_SUB);
		return mktok(TOK_MINUS);

	case '!':
		if (match('='))
			return mktok(TOK_NE);
		return mktok(TOK_BANG);

	case '/':
		if (match('='))
			return mktok(TOK_ASSIGN_DIV);
		if (peek() == '*') {
			comment();
			/* skip */
			return lexer_next();
		}
		return mktok(TOK_SLASH);

	case '%':
		if (match('='))
			return mktok(TOK_ASSIGN_MOD);
		return mktok(TOK_PERCENT);

	case '<':
		if (match('<')) {
			if (match('='))
				return mktok(TOK_ASSIGN_SHL);
			return mktok(TOK_SHL);
		}
		if (match('='))
			return mktok(TOK_LE);
		return mktok(TOK_LT);

	case '>':
		if (match('>')) {
			if (match('='))
				return mktok(TOK_ASSIGN_SHR);
			return mktok(TOK_SHR);
		}
		if (match('='))
			return mktok(TOK_GE);
		return mktok(TOK_GT);

	case '^':
		if (match('='))
			return mktok(TOK_ASSIGN_XOR);
		return mktok(TOK_XOR_BIT);

	case '|':
		if (match('|'))
			return mktok(TOK_OR_LOGICAL);
		if (match('='))
			return mktok(TOK_ASSIGN_OR);
		return mktok(TOK_OR_BIT);

	case '=':
		if (match('='))
			return mktok(TOK_EQ);
		return mktok(TOK_ASSIGN);

	case '#':
		if (match('#'))
			return mktok(TOK_HASHHASH);
		return mktok(TOK_HASH);

	default:
		return mktok(TOK_INVALID);
	}
}

const char* lexer_nametok(TokenType type)
{
	switch (type) {
	TOKEN_NAME_CASE(TOK_INVALID);
	TOKEN_NAME_CASE(TOK_EOF);
	TOKEN_NAME_CASE(TOK_IDENTIFIER);
	TOKEN_NAME_CASE(TOK_INT_LITERAL);
	TOKEN_NAME_CASE(TOK_FLOAT_LITERAL);
	TOKEN_NAME_CASE(TOK_CHAR_LITERAL);
	TOKEN_NAME_CASE(TOK_STRING_LITERAL);
	TOKEN_NAME_CASE(TOK_AUTO);
	TOKEN_NAME_CASE(TOK_BREAK);
	TOKEN_NAME_CASE(TOK_CASE);
	TOKEN_NAME_CASE(TOK_CHAR);
	TOKEN_NAME_CASE(TOK_CONST);
	TOKEN_NAME_CASE(TOK_CONTINUE);
	TOKEN_NAME_CASE(TOK_DEFAULT);
	TOKEN_NAME_CASE(TOK_DO);
	TOKEN_NAME_CASE(TOK_DOUBLE);
	TOKEN_NAME_CASE(TOK_ELSE);
	TOKEN_NAME_CASE(TOK_ENUM);
	TOKEN_NAME_CASE(TOK_EXTERN);
	TOKEN_NAME_CASE(TOK_FLOAT);
	TOKEN_NAME_CASE(TOK_FOR);
	TOKEN_NAME_CASE(TOK_GOTO);
	TOKEN_NAME_CASE(TOK_IF);
	TOKEN_NAME_CASE(TOK_INT);
	TOKEN_NAME_CASE(TOK_LONG);
	TOKEN_NAME_CASE(TOK_REGISTER);
	TOKEN_NAME_CASE(TOK_RETURN);
	TOKEN_NAME_CASE(TOK_SHORT);
	TOKEN_NAME_CASE(TOK_SIGNED);
	TOKEN_NAME_CASE(TOK_SIZEOF);
	TOKEN_NAME_CASE(TOK_STATIC);
	TOKEN_NAME_CASE(TOK_STRUCT);
	TOKEN_NAME_CASE(TOK_SWITCH);
	TOKEN_NAME_CASE(TOK_TYPEDEF);
	TOKEN_NAME_CASE(TOK_UNION);
	TOKEN_NAME_CASE(TOK_UNSIGNED);
	TOKEN_NAME_CASE(TOK_VOID);
	TOKEN_NAME_CASE(TOK_VOLATILE);
	TOKEN_NAME_CASE(TOK_WHILE);
	TOKEN_NAME_CASE(TOK_BRACKL);
	TOKEN_NAME_CASE(TOK_BRACKR);
	TOKEN_NAME_CASE(TOK_PARENL);
	TOKEN_NAME_CASE(TOK_PARENR);
	TOKEN_NAME_CASE(TOK_BRACEL);
	TOKEN_NAME_CASE(TOK_BRACER);
	TOKEN_NAME_CASE(TOK_DOT);
	TOKEN_NAME_CASE(TOK_DOTDOTDOT);
	TOKEN_NAME_CASE(TOK_ARROW);
	TOKEN_NAME_CASE(TOK_INCREMENT);
	TOKEN_NAME_CASE(TOK_DECREMENT);
	TOKEN_NAME_CASE(TOK_STAR);
	TOKEN_NAME_CASE(TOK_PLUS);
	TOKEN_NAME_CASE(TOK_MINUS);
	TOKEN_NAME_CASE(TOK_TILDA);
	TOKEN_NAME_CASE(TOK_BANG);
	TOKEN_NAME_CASE(TOK_SLASH);
	TOKEN_NAME_CASE(TOK_PERCENT);
	TOKEN_NAME_CASE(TOK_SHL);
	TOKEN_NAME_CASE(TOK_SHR);
	TOKEN_NAME_CASE(TOK_LT);
	TOKEN_NAME_CASE(TOK_GT);
	TOKEN_NAME_CASE(TOK_LE);
	TOKEN_NAME_CASE(TOK_GE);
	TOKEN_NAME_CASE(TOK_EQ);
	TOKEN_NAME_CASE(TOK_NE);
	TOKEN_NAME_CASE(TOK_XOR_BIT);
	TOKEN_NAME_CASE(TOK_OR_BIT);
	TOKEN_NAME_CASE(TOK_AND_BIT);
	TOKEN_NAME_CASE(TOK_OR_LOGICAL);
	TOKEN_NAME_CASE(TOK_AND_LOGICAL);
	TOKEN_NAME_CASE(TOK_QUESTION);
	TOKEN_NAME_CASE(TOK_COLON);
	TOKEN_NAME_CASE(TOK_SEMICOLON);
	TOKEN_NAME_CASE(TOK_ASSIGN);
	TOKEN_NAME_CASE(TOK_ASSIGN_MUL);
	TOKEN_NAME_CASE(TOK_ASSIGN_DIV);
	TOKEN_NAME_CASE(TOK_ASSIGN_MOD);
	TOKEN_NAME_CASE(TOK_ASSIGN_ADD);
	TOKEN_NAME_CASE(TOK_ASSIGN_SUB);
	TOKEN_NAME_CASE(TOK_ASSIGN_SHL);
	TOKEN_NAME_CASE(TOK_ASSIGN_SHR);
	TOKEN_NAME_CASE(TOK_ASSIGN_XOR);
	TOKEN_NAME_CASE(TOK_ASSIGN_AND);
	TOKEN_NAME_CASE(TOK_ASSIGN_OR);
	TOKEN_NAME_CASE(TOK_COMMA);
	TOKEN_NAME_CASE(TOK_HASH);
	TOKEN_NAME_CASE(TOK_HASHHASH);

	default:
		return "TOK_UNKNOWN";
	}
}

