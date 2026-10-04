#ifndef PRINT_H
#define PRINT_H

#include "ir.h"

void print_block(IRFunction* fn, IRBlock* block);
void print_function(IRFunction* fn);
void print_instruction(IRInstruction* ins);
void print_operand(IROperand* opd);
void print_operands(IROperandSet* set);
void print_params(IRParameterSet* set);
void print_type(IRType type);

#endif /* PRINT_H */

