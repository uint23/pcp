#include <stdio.h>

#include "test.h"

void test_assert(TestContext* test, int condition, const char* expression,
                 const char* file, int line)
{
	if (condition)
		return;

	test->failed = 1;
	PRINT_ASSERT_FAILED(file, line, expression);
}

int run_test(const char* name, TestFunction function)
{
	TestContext test;

	test.name = name;
	test.failed = 0;

	PRINT_RUN_TEST(name);
	function(&test);

	if (test.failed) {
		PRINT_FAIL_TEST(name);
		return 1;
	}
	else {
		PRINT_PASS_TEST(name);
		return 0;
	}
}

int main(void)
{
	size_t i;
	size_t failed = 0;

	for (i = 0; i < test_count; i++)
		failed += run_test(tests[i].name, tests[i].function);

	putchar('\n');
	PRINT_SUMMARY(test_count, test_count - failed, failed);
	return (int)failed;
}

