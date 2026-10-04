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
} IROperatorType;

typedef enum {
	IR_TERM_NONE,
	IR_TERM_RET,
	IR_TERM_JMP,
	IR_TERM_BR,

	IR_TERM_LAST
} IRTerminatorType;

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
	IROperatorType opr;
	IRType         type;
	IRValue        res;
	IROperand      opd[2];
} IRInstruction;

typedef struct {
	IRType  type;
	IRValue value;
} IRParameter;

typedef struct {
	IRParameter* params;
	size_t       cnt;
	size_t       cap;
} IRParameterSet;

typedef struct {
	IRTerminatorType type;

	union {
		struct {
			int       hasval;
			IROperand value;
		} ret;

		/* XXX: jmp, br */
	} data;
} IRTerminator;

typedef struct {
	const char*    name;
	IRInstruction* ins;
	IRParameterSet params;
	IRTerminator   term;
	size_t         cnt;
	size_t         cap;
} IRBlock;

typedef struct {
	const char*    name;
	IRType         type;
	IRParameterSet params;
	IRBlock*       blocks;
	IRValue        nextval;
	size_t         cnt;
	size_t         cap;
} IRFunction;

IRBlock* ir_add_block(IRFunction* fn, const char* name);
IRInstruction* ir_add_instruction(IRBlock* block);
IRParameter* ir_add_param(IRFunction* fn, IRParameterSet* pset, IRType type);
void ir_print_block(IRBlock* block);
void ir_print_function(IRFunction* fn);
void ir_print_instruction(IRInstruction* ins);

#endif /* IR_H */

