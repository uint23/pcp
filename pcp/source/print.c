#include <stdio.h>

#include "ir.h"
#include "print.h"

void print_block(IRFunction* fn, IRBlock* block)
{
	size_t i;
	for (i = 0; i < block->cnt; i++)
		print_instruction(&block->ins[i]);

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
		break;

	case IR_TERM_BR:
		printf("  br ");
		print_operand(&block->term.data.br.cond);

		printf(", %s(", fn->blocks[block->term.data.br.yes.block].name);
		print_operands(&block->term.data.br.yes.args);

		printf("), %s(", fn->blocks[block->term.data.br.no.block].name);
		print_operands(&block->term.data.br.no.args);

		printf(")\n");
		break;

		printf(")\n");
		break;

	case IR_TERM_NONE:
	case IR_TERM_LAST:
	default:
		break;
	}
}

void print_function(IRFunction* fn)
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
		print_block(fn, &fn->blocks[i]);
	}

	printf("}\n");
}

void print_instruction(IRInstruction* ins)
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

void print_operand(IROperand* opd)
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

void print_operands(IROperandSet* set)
{
	size_t i = 0;
	for (i = 0; i < set->cnt; i++) {
		if (i)
			printf(", ");

		print_operand(&set->opds[i]);
	}
}

void print_params(IRParameterSet* set)
{
	size_t i;

	for (i = 0; i < set->cnt; i++) {
		if (i)
			printf(", ");

		printf("_%u: ", set->params[i].value);
		print_type(set->params[i].type);
	}
}

void print_type(IRType type)
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

