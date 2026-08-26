#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

typedef struct {
	const char* path;
	FILE*       file;
	char*       data;
	size_t      len;
} SourceFile;

#endif /* COMMON_H */

