#ifndef IR_H
#define IR_H

#include <stddef.h>

typedef enum {
	IR_I1,
	IR_I8,
	IR_I16,
	IR_I32,
	IR_I64,

	IR_F32,
	IR_F64,

	IR_VOID,

	IR_TYPE_LAST
} IRType;

typedef enum {
	IR_OPR_ADD,
	IR_OPR_SUB,
	IR_OPR_MUL,

	IR_OPR_LAST
} IROperator;

typedef enum {
	IR_OPD_VALUE,
	IR_OPD_INTEGER,

	IR_OPD_LAST
} IROperandType;

typedef unsigned IRValue;
typedef struct {
	IROperandType type;

	union {
		IRValue value;
		long    integer;
	} data;

} IROperand;

typedef struct {
	IROperator opr;
	IRType     type;
	IRValue    res;
	IROperand  opd[2];
} IRInstruction;

typedef struct {
	const char*    name;
	IRInstruction* ins;
	size_t         cnt;
	size_t         cap;
	/* XXX: params */
} IRBlock;

typedef struct {
	const char* name;
	IRType      type;

	IRBlock* blocks;
	IRValue  nextval;
	size_t   cnt;
	size_t   cap;
} IRFunction;

IRBlock* ir_add_block(IRFunction* fn, const char* name);
IRInstruction* ir_add_instruction(IRBlock* block);
void ir_print_block(IRBlock* block);
void ir_print_function(IRFunction* fn);
void ir_print_instruction(IRInstruction* ins);

#endif /* IR_H */

