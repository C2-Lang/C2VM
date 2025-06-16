#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "../core/platform.h"
#include "../vm/vm.h"
#include "../vm/handlers.h"

#define INT_DIVIDE_BY_ZERO 0
#define INT_STACK_OVERFLOW 1
#define INT_INVALID_OPCODE 2
#define INT_STACK_UNDERFLOW 3
#define INT_INVALID_MEMORY 4

VM_API void VM_CALL vm_init_default_interrupts(VM* vm);
VM_API void VM_CALL int_handler_divide_by_zero(VM* vm, uint8_t int_num);
VM_API void VM_CALL int_handler_stack_overflow(VM* vm, uint8_t int_num);
VM_API void VM_CALL int_handler_invalid_opcode(VM* vm, uint8_t int_num);
VM_API void VM_CALL int_handler_stack_underflow(VM* vm, uint8_t int_num);
VM_API void VM_CALL int_handler_invalid_memory(VM* vm, uint8_t int_num);

#endif