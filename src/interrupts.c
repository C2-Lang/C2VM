#include "interrupts.h"
#include <stdio.h>

void int_handler_divide_by_zero(VM* vm, uint8_t int_num) {
    printf("Error: Division by zero (interrupt %d)\n", int_num);
    vm->running = false;
}

void int_handler_stack_overflow(VM* vm, uint8_t int_num) {
    printf("Error: Stack overflow (interrupt %d)\n", int_num);
    vm->running = false;
}

void int_handler_invalid_opcode(VM* vm, uint8_t int_num) {
    printf("Error: Invalid opcode (interrupt %d)\n", int_num);
    vm->running = false;
}

void vm_init_default_interrupts(VM* vm) {
    vm_register_interrupt(vm, INT_DIVIDE_BY_ZERO, int_handler_divide_by_zero);
    vm_register_interrupt(vm, INT_STACK_OVERFLOW, int_handler_stack_overflow);
    vm_register_interrupt(vm, INT_INVALID_OPCODE, int_handler_invalid_opcode);
}
