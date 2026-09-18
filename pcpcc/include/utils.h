#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

typedef enum {
	ERR_OK,

	ERR_ALLOC,
	ERR_FOPEN,
	ERR_FTELL,
	ERR_FSEEK,

	ERR_SRC_DATA_LEN_DIFFERENT,

	ERR_PARSE_EXPECTED_TOKEN,

	ErrorLast
} ErrorCode;

/* print message and die with errorcode ec */
void die(ErrorCode ec, const char* fmt, ...);

/* grow (reallocate) a given list */
void* list_grow(void* list, size_t item_size, size_t* capacity);

#endif /* UTILS_H */

