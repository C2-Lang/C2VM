#ifndef _VM_H
#define _VM_H

#include "registers.h"
#include "handlers.h"
#include <stdint.h>
#include <stdbool.h>

#define VM_MEMORY_SIZE 1024 * 1024
#define VM_STACK_START VM_MEMORY_SIZE - 1
#define VM_STACK_FRAME_SIZE 1024
#define VM_INTERRUPT_COUNT 256
#define VM_INTERRUPT_VECTOR_START 0

typedef struct VM {
    struct Registers registers;
    uint8_t memory[VM_MEMORY_SIZE];
    bool running;
    InterruptHandler interrupt_handlers[VM_INTERRUPT_COUNT];
} VM;

VM* vm_create();
void vm_destroy(VM* vm);
void vm_execute(VM* vm);
void vm_register_interrupt(VM* vm, uint8_t int_num, InterruptHandler handler);

#endif