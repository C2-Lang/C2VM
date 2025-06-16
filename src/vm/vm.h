#ifndef _VM_H
#define _VM_H

#include "../core/platform.h"
#include "registers.h"
#include "handlers.h"
#include <stdint.h>
#include <stdbool.h>

#define VM_MEMORY_SIZE 1024 * 1024
#define VM_STACK_START VM_MEMORY_SIZE - 1
#define VM_STACK_FRAME_SIZE 1024
#define VM_INTERRUPT_COUNT 256
#define VM_INTERRUPT_VECTOR_START 0

#define VM_ERROR_NONE 0
#define VM_ERROR_NULL_POINTER 1
#define VM_ERROR_INVALID_MEMORY_ACCESS 2
#define VM_ERROR_STACK_OVERFLOW 3
#define VM_ERROR_STACK_UNDERFLOW 4
#define VM_ERROR_INVALID_OPCODE 5
#define VM_ERROR_DIVISION_BY_ZERO 6

#ifdef C2VM_WINDOWS
    #pragma pack(push, 8)
#endif

typedef struct VM {
    struct Registers registers;
    uint8_t memory[VM_MEMORY_SIZE];
    bool running;
    InterruptHandler interrupt_handlers[VM_INTERRUPT_COUNT];
    int error_code;
} VM;

#ifdef C2VM_WINDOWS
    #pragma pack(pop)
#endif

VM_API VM* VM_CALL vm_create();
VM_API void VM_CALL vm_destroy(VM* vm);
VM_API void VM_CALL vm_execute(VM* vm);
VM_API void VM_CALL vm_register_interrupt(VM* vm, int int_num, InterruptHandler handler);
VM_API int VM_CALL vm_get_error(VM* vm);
VM_API bool VM_CALL vm_check_memory_bounds(VM* vm, uint64_t addr);

#endif