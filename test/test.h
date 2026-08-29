#ifndef TEST_H
#define TEST_H

#include <stddef.h>

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_RESET "\x1b[0m"

#define PRINT_RUN_TEST(name) \
	printf(COLOR_GREEN "[ RUNS ]" COLOR_RESET " %s\n", name)

#define PRINT_PASS_TEST(name) \
	printf(COLOR_GREEN "[ PASS ]" COLOR_RESET " %s\n", name)

#define PRINT_FAIL_TEST(name) \
	printf(COLOR_RED "[ FAIL ]" COLOR_RESET " %s\n", name)

#define PRINT_SKIP_TEST(name) \
	printf(COLOR_YELLOW "[ SKIP ]" COLOR_RESET " %s\n", name)

#define PRINT_ASSERT_FAILED(file, line, expression) \
	printf("\t%s:%d: assertion failed: %s\n", file, line, expression)

#define PRINT_SUMMARY(total, passed, failed) \
	printf("%s%lu tests: %lu passed, %lu failed" COLOR_RESET "\n", \
	       (failed) ? COLOR_RED : COLOR_GREEN, (unsigned long)(total), \
	       (unsigned long)(passed), (unsigned long)(failed))

typedef struct {
	const char* name;
	int         failed;
} TestContext;

typedef void (*TestFunction)(TestContext* test);
typedef struct {
	const char*  name;
	TestFunction function;
} Test;

extern Test tests[];
extern size_t test_count;

void test_assert(TestContext* test, int condition, const char* expression,
                 const char* file, int line);
int run_test(const char* name, TestFunction function);

#define TEST(name) static void name(TestContext* test)

#define TEST_DECLARE(name) static void name(TestContext* test);
#define TEST_ENTRY(name) {#name, name},

#define REGISTER_TESTS() \
	TEST_LIST(TEST_DECLARE) \
	Test tests[] = {TEST_LIST(TEST_ENTRY)}; \
	size_t test_count = sizeof(tests) / sizeof(tests[0]);

#define ASSERT(expression) \
	test_assert(test, (expression) != 0, #expression, __FILE__, __LINE__);
#define EXPECT_EQ(expression1, expression2) \
	ASSERT((expression1) == (expression2))
#define EXPECT_NE(expression1, expression2) \
	ASSERT((expression1) != (expression2))
#define TEST_FAILED(message) test_assert(test, 0, message, __FILE__, __LINE__);

#endif /* TEST_H */

