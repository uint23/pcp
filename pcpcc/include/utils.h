#ifndef UTILS_H
#define UTILS_H

typedef enum {
	ERR_OK,

	ERR_SRC_FOPEN,

	ErrorLast
} ErrorCode;

/* print message and die with errorcode ec */
void die(ErrorCode ec, const char* fmt, ...);

#endif /* UTILS_H */

