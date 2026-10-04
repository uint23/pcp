#include <stdio.h>
#include <stdlib.h>

#include "ir.h"
#include "utils.h"

static void print_operand(IROperand* opd);
static void print_type(IRType type);
static void print_params(IRParameterSet* set);

static void print_operand(IROperand* opd)
{
	switch (opd->type) {
	case IR_OPD_VALUE:
		printf("_%u", opd->data.value);
		break;
	case IR_OPD_INTEGER:
		printf("%ld", opd->data.integer);
		break;
	default:
		break;
	}
}

static void print_type(IRType type)
{
	switch (type) {
	case IR_I1:   printf("i1"); break;
	case IR_I8:   printf("i8"); break;
	case IR_I16:  printf("i16"); break;
	case IR_I32:  printf("i32"); break;
	case IR_I64:  printf("i64"); break;
	case IR_F32:  printf("f32"); break;
	case IR_F64:  printf("f64"); break;
	case IR_VOID: printf("void"); break;
	default: break;
	}
}

static void print_operands(IROperandSet* set)
{
	size_t i = 0;
	for (i = 0; i < set->cnt; i++) {
		if (i)
			printf(", ");

		print_operand(&set->opds[i]);
	}
}

static void print_params(IRParameterSet* set)
{
	size_t i;

	for (i = 0; i < set->cnt; i++) {
		if (i)
			printf(", ");

		printf("_%u: ", set->params[i].value);
		print_type(set->params[i].type);
	}
}

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

void ir_print_block(IRFunction* fn, IRBlock* block)
{
	size_t i;
	for (i = 0; i < block->cnt; i++)
		ir_print_instruction(&block->ins[i]);

	/* block end */
	switch (block->term.type) {
	case IR_TERM_RET:
		printf("  ret");

		if (block->term.data.ret.hasval) {
			putchar(' ');
			print_operand(&block->term.data.ret.value);
		}

		putchar('\n');
		break;
	case IR_TERM_JMP:
		printf("  jmp %s(", fn->blocks[block->term.data.jmp.block].name);
		print_operands(&block->term.data.jmp.args);

	printf(")\n");
	break;

	case IR_TERM_NONE:
	case IR_TERM_LAST:
	default:
		break;
	}
}

void ir_print_function(IRFunction* fn)
{
	size_t i;

	printf("fn ");
	print_type(fn->type);

	printf(" %s(", fn->name);
	print_params(&fn->params);
	printf(") {\n");

	for (i = 0; i < fn->cnt; i++) {
		printf("%s(", fn->blocks[i].name);
		print_params(&fn->blocks[i].params);
		printf("):\n");
		ir_print_block(fn, &fn->blocks[i]);
	}

	printf("}\n");
}

void ir_print_instruction(IRInstruction* ins)
{
	printf("  _%u: ", ins->res);
	print_type(ins->type);
	printf(" = ");

	switch (ins->opr) {
	case IR_OPR_ADD: printf("add "); break;
	case IR_OPR_SUB: printf("sub "); break;
	case IR_OPR_MUL: printf("mul "); break;
	case IR_OPR_LAST:
	default: return;
	}

	print_operand(&ins->opd[0]);
	printf(", ");
	print_operand(&ins->opd[1]);
	putchar('\n');
}

