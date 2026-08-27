#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>

#define LENGTH(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct {
	const char* path;
	FILE*       file;
	char*       data;
	size_t      len;
} SourceFile;

#endif /* COMMON_H */

