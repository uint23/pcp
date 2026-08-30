#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "common.h"

/* initialise parser struct */
void parser_init(SourceFile* source);

/* parse through a file and return
   complete AST */
AST* parser_parse(void);

#endif /* PARSER_H */

