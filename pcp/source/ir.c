#include <stdio.h>
#include <stdlib.h>

#include "ir.h"
#include "utils.h"

IRBlock* ir_add_block(IRFunction* fn, const char* name)
{
	IRBlock* block;

	if (fn->cnt >= fn->cap)
		fn->blocks = list_grow(fn->blocks, sizeof(*fn->blocks), &fn->cap);

	block = &fn->blocks[fn->cnt++];
	block->name = name;
	block->ins = NULL;
	block->params.params = NULL;
	block->params.cnt = 0;
	block->params.cap = 0;
	block->term.type = IR_TERM_NONE;
	block->term.type = IR_TERM_NONE;
	block->cnt = 0;
	block->cap = 0;

	return block;
}

IRInstruction* ir_add_instruction(IRBlock* block)
{
	IRInstruction* ins;

	if (block->cnt >= block->cap)
		block->ins = list_grow(block->ins, sizeof(*block->ins), &block->cap);

	ins = &block->ins[block->cnt++];

	/* XXX: initialise instruction */

	return ins;
}

IROperand* ir_add_operand(IROperandSet* set)
{
	IROperand* opd;

	if (set->cnt >= set->cap)
		set->opds = list_grow(set->opds, sizeof(*set->opds), &set->cap);

	opd = &set->opds[set->cnt++];

	return opd;
}

IRParameter* ir_add_param(IRFunction* fn, IRParameterSet* set, IRType type)
{
	IRParameter* param;

	if (set->cnt >= set->cap)
		set->params = list_grow(set->params, sizeof(*set->params), &set->cap);

	param = &set->params[set->cnt++];
	param->type = type;
	param->value = fn->nextval++;

	return param;
}

