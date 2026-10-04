#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

void die(ErrorCode ec, const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fputc('\n', stderr);
	fflush(stderr);

	exit(ec);
}

void* list_grow(void* list, size_t item_size, size_t* capacity)
{
	void* res;
	size_t newcap;

	newcap = *capacity ? *capacity * 2 : 8;

	res = realloc(list, newcap * item_size);
	if (!res)
		die(ERR_ALLOC, "Failed to grow array");

	*capacity = newcap;
	return res;
}

