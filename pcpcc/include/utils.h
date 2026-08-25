#ifndef UTILS_H
#define UTILS_H

#ifndef PCPCC_VERSION
#define PCPCC_VERSION "Unsure.."
#endif /* PCPCC_VERSION */

#define PCPCC_USAGE \
	"\t[-v|--version]: Show pcpcc version" \
	"\n"

typedef enum {
	ERR_OK,

	ERR_ALLOC,
	ERR_FOPEN,
	ERR_FTELL,
	ERR_FSEEK,

	ERR_SRC_DATA_LEN_DIFFERENT,

	ErrorLast
} ErrorCode;

/* print message and die with errorcode ec */
void die(ErrorCode ec, const char* fmt, ...);

#endif /* UTILS_H */
