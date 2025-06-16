#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "vm.h"
#include "handlers.h"

#define INT_DIVIDE_BY_ZERO 0
#define INT_STACK_OVERFLOW 1
#define INT_INVALID_OPCODE 2

void vm_init_default_interrupts(VM* vm);
void int_handler_divide_by_zero(VM* vm, uint8_t int_num);
void int_handler_stack_overflow(VM* vm, uint8_t int_num);
void int_handler_invalid_opcode(VM* vm, uint8_t int_num);

#endif