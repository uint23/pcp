#include "test.h"

#define TEST_LIST(X) \
	X(lex_numbers) \
	X(lex_strings)

REGISTER_TESTS()

TEST(lex_numbers)
{
	/* body */
}

TEST(lex_strings)
{
	/* body */
	TEST_FAILED("Not implemented")
}

