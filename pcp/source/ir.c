#include <stdio.h>
#include <stdlib.h>

#include "ir.h"
#include "utils.h"

static void print_operand(IROperand* opd);
static void print_type(IRType type);

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

IRBlock* ir_add_block(IRFunction* fn, const char* name)
{
	IRBlock* block;

	if (fn->cnt >= fn->cap)
		fn->blocks = list_grow(fn->blocks, sizeof(*fn->blocks), &fn->cap);

	block = &fn->blocks[fn->cnt++];
	block->name = name;
	block->ins = NULL;
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

	/* XXX: initialise instruction here */

	return ins;
}

void ir_print_block(IRBlock* block)
{
	size_t i;

	for (i = 0; i < block->cnt; i++)
		ir_print_instruction(&block->ins[i]);
}

void ir_print_function(IRFunction* fn)
{
	size_t i;

	printf("fn ");
	print_type(fn->type);
	printf(" %s() {\n", fn->name);

	for (i = 0; i < fn->cnt; i++) {
		printf("%s():\n", fn->blocks[i].name);
		ir_print_block(&fn->blocks[i]);
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

