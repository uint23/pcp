#include <string.h>

#include "common.h"
#include "lexer.h"
#include "test.h"

#define TOKEN_CASES(F, X) \
	F(X, keyword_auto, "auto", TOK_AUTO) \
	F(X, keyword_break, "break", TOK_BREAK) \
	F(X, keyword_case, "case", TOK_CASE) \
	F(X, keyword_char, "char", TOK_CHAR) \
	F(X, keyword_const, "const", TOK_CONST) \
	F(X, keyword_continue, "continue", TOK_CONTINUE) \
	F(X, keyword_default, "default", TOK_DEFAULT) \
	F(X, keyword_do, "do", TOK_DO) \
	F(X, keyword_double, "double", TOK_DOUBLE) \
	F(X, keyword_else, "else", TOK_ELSE) \
	F(X, keyword_enum, "enum", TOK_ENUM) \
	F(X, keyword_extern, "extern", TOK_EXTERN) \
	F(X, keyword_float, "float", TOK_FLOAT) \
	F(X, keyword_for, "for", TOK_FOR) \
	F(X, keyword_goto, "goto", TOK_GOTO) \
	F(X, keyword_if, "if", TOK_IF) \
	F(X, keyword_int, "int", TOK_INT) \
	F(X, keyword_long, "long", TOK_LONG) \
	F(X, keyword_register, "register", TOK_REGISTER) \
	F(X, keyword_return, "return", TOK_RETURN) \
	F(X, keyword_short, "short", TOK_SHORT) \
	F(X, keyword_signed, "signed", TOK_SIGNED) \
	F(X, keyword_sizeof, "sizeof", TOK_SIZEOF) \
	F(X, keyword_static, "static", TOK_STATIC) \
	F(X, keyword_struct, "struct", TOK_STRUCT) \
	F(X, keyword_switch, "switch", TOK_SWITCH) \
	F(X, keyword_typedef, "typedef", TOK_TYPEDEF) \
	F(X, keyword_union, "union", TOK_UNION) \
	F(X, keyword_unsigned, "unsigned", TOK_UNSIGNED) \
	F(X, keyword_void, "void", TOK_VOID) \
	F(X, keyword_volatile, "volatile", TOK_VOLATILE) \
	F(X, keyword_while, "while", TOK_WHILE) \
\
	F(X, identifier, "foo", TOK_IDENTIFIER) \
	F(X, identifier_underscore, "_bar2", TOK_IDENTIFIER) \
\
	F(X, decimal, "67", TOK_INT_LITERAL) \
	F(X, hexadecimal, "0x67", TOK_INT_LITERAL) \
	F(X, octal_looking, "067", TOK_INT_LITERAL) \
	F(X, long_suffix, "67L", TOK_INT_LITERAL) \
	F(X, unsigned_suffix, "67u", TOK_INT_LITERAL) \
	F(X, unsigned_long_suffix, "67UL", TOK_INT_LITERAL) \
	F(X, float_literal, "10.1", TOK_FLOAT_LITERAL) \
	F(X, float_leading_dot, ".5", TOK_FLOAT_LITERAL) \
	F(X, float_exponent, "1e3", TOK_FLOAT_LITERAL) \
	F(X, float_suffix_f, "10.1f", TOK_FLOAT_LITERAL) \
	F(X, float_suffix_l, "10.1l", TOK_FLOAT_LITERAL) \
\
	F(X, char_literal, "'a'", TOK_CHAR_LITERAL) \
	F(X, char_escaped, "'\\n'", TOK_CHAR_LITERAL) \
	F(X, string_literal, "\"hi\"", TOK_STRING_LITERAL) \
	F(X, string_escaped_quote, "\"a\\\"b\"", TOK_STRING_LITERAL) \
\
	F(X, bracket_l, "[", TOK_BRACKL) \
	F(X, bracket_r, "]", TOK_BRACKR) \
	F(X, paren_l, "(", TOK_PARENL) \
	F(X, paren_r, ")", TOK_PARENR) \
	F(X, brace_l, "{", TOK_BRACEL) \
	F(X, brace_r, "}", TOK_BRACER) \
\
	F(X, dot, ".", TOK_DOT) \
	F(X, ellipsis, "...", TOK_DOTDOTDOT) \
	F(X, arrow, "->", TOK_ARROW) \
\
	F(X, increment, "++", TOK_INCREMENT) \
	F(X, decrement, "--", TOK_DECREMENT) \
\
	F(X, star, "*", TOK_STAR) \
	F(X, plus, "+", TOK_PLUS) \
	F(X, minus, "-", TOK_MINUS) \
	F(X, tilda, "~", TOK_TILDA) \
	F(X, bang, "!", TOK_BANG) \
	F(X, slash, "/", TOK_SLASH) \
	F(X, percent, "%", TOK_PERCENT) \
\
	F(X, shl, "<<", TOK_SHL) \
	F(X, shr, ">>", TOK_SHR) \
\
	F(X, lt, "<", TOK_LT) \
	F(X, gt, ">", TOK_GT) \
	F(X, le, "<=", TOK_LE) \
	F(X, ge, ">=", TOK_GE) \
	F(X, eq, "==", TOK_EQ) \
	F(X, ne, "!=", TOK_NE) \
\
	F(X, xor_bit, "^", TOK_XOR_BIT) \
	F(X, or_bit, "|", TOK_OR_BIT) \
	F(X, and_bit, "&", TOK_AND_BIT) \
	F(X, or_logical, "||", TOK_OR_LOGICAL) \
	F(X, and_logical, "&&", TOK_AND_LOGICAL) \
\
	F(X, question, "?", TOK_QUESTION) \
	F(X, colon, ":", TOK_COLON) \
	F(X, semicolon, ";", TOK_SEMICOLON) \
\
	F(X, assign, "=", TOK_ASSIGN) \
	F(X, assign_mul, "*=", TOK_ASSIGN_MUL) \
	F(X, assign_div, "/=", TOK_ASSIGN_DIV) \
	F(X, assign_mod, "%=", TOK_ASSIGN_MOD) \
	F(X, assign_add, "+=", TOK_ASSIGN_ADD) \
	F(X, assign_sub, "-=", TOK_ASSIGN_SUB) \
	F(X, assign_shl, "<<=", TOK_ASSIGN_SHL) \
	F(X, assign_shr, ">>=", TOK_ASSIGN_SHR) \
	F(X, assign_xor, "^=", TOK_ASSIGN_XOR) \
	F(X, assign_and, "&=", TOK_ASSIGN_AND) \
	F(X, assign_or, "|=", TOK_ASSIGN_OR) \
\
	F(X, comma, ",", TOK_COMMA) \
	F(X, hash, "#", TOK_HASH) \
	F(X, hashhash, "##", TOK_HASHHASH)

#define TOKEN_CASE_NAME(X, name, input, type) X(name)
#define TOKEN_CASE_TEST(X, name, input, type) \
	TEST(name) \
	{ \
		expect_token(test, input, type); \
	}

#define TEST_LIST(X) \
	TOKEN_CASES(TOKEN_CASE_NAME, X) \
	X(lex_numbers) \
	X(lex_strings) \
	X(two_dots_are_separate) \
	X(comments_are_skipped) \
	X(unterminated_comment) \
	X(invalid_character) \
	X(unterminated_char_literal) \
	X(keyword_prefix_identifier) \
	X(empty_input)

REGISTER_TESTS()

void lex(char* text)
{
	static SourceFile source = {0};
	source.data = text;
	source.len = strlen(text);
	lexer_init(&source);
}

static void expect_token(TestContext* test, char* input, TokenType type)
{
	lex(input);
	EXPECT_EQ(lexer_next().type, type);
	EXPECT_EQ(lexer_next().type, TOK_EOF);
}

TOKEN_CASES(TOKEN_CASE_TEST, 0)

TEST(lex_numbers)
{
	lex("123 0x1A 3.14 .5 1e3 10UL");

	EXPECT_EQ(lexer_next().type, TOK_INT_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_INT_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_FLOAT_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_FLOAT_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_FLOAT_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_INT_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_EOF);

	lex("0x1A");
	EXPECT_EQ(lexer_next().len, 4);
}

TEST(lex_strings)
{
	lex("\"hello\" \"a\\\"b\" 'a' '\\n'");

	EXPECT_EQ(lexer_next().type, TOK_STRING_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_STRING_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_CHAR_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_CHAR_LITERAL);
	EXPECT_EQ(lexer_next().type, TOK_EOF);

	lex("\"unterminated");
	EXPECT_EQ(lexer_next().type, TOK_INVALID);
}

TEST(two_dots_are_separate)
{
	lex("..");
	EXPECT_EQ(lexer_next().type, TOK_DOT);
	EXPECT_EQ(lexer_next().type, TOK_DOT);
	EXPECT_EQ(lexer_next().type, TOK_EOF);
}

TEST(comments_are_skipped)
{
	lex("a /* comment */ b");
	EXPECT_EQ(lexer_next().type, TOK_IDENTIFIER);
	EXPECT_EQ(lexer_next().type, TOK_IDENTIFIER);
	EXPECT_EQ(lexer_next().type, TOK_EOF);
}

TEST(unterminated_comment)
{
	lex("a /* unterminated");
	EXPECT_EQ(lexer_next().type, TOK_IDENTIFIER);
	EXPECT_EQ(lexer_next().type, TOK_INVALID);
}

TEST(invalid_character)
{
	lex("@");
	EXPECT_EQ(lexer_next().type, TOK_INVALID);
}

TEST(unterminated_char_literal)
{
	lex("'a");
	EXPECT_EQ(lexer_next().type, TOK_INVALID);
}

TEST(keyword_prefix_identifier)
{
	lex("intx");
	EXPECT_EQ(lexer_next().type, TOK_IDENTIFIER);
	EXPECT_EQ(lexer_next().type, TOK_EOF);
}

TEST(empty_input)
{
	lex("");
	EXPECT_EQ(lexer_next().type, TOK_EOF);
}

