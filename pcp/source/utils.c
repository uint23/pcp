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

