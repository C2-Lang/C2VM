#ifndef _VM_H
#define _VM_H

#include "../core/platform.h"
#include "registers.h"
#include "handlers.h"
#include "types.h"
#include <stdint.h>
#include <stdbool.h>

#define VM_MEMORY_SIZE 1024 * 1024
#define VM_DATA_SECTION_START 0x1000
#define VM_CODE_SECTION_START 0x2000
#define VM_STACK_SIZE 4096
#define VM_STACK_START (VM_MEMORY_SIZE - 1)
#define VM_STACK_END   (VM_MEMORY_SIZE - VM_STACK_SIZE)
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
#define VM_ERROR_OUT_OF_MEMORY 7
#define VM_ERROR_INVALID_FREE 8

#define VM_DATA_SECTION_MAGIC 0xDA7A
#define VM_DATA_SECTION_ALIGN 16

#ifdef C2VM_WINDOWS
    #pragma pack(push, 8)
#endif

typedef struct VM {
    struct Registers registers;
    uint8_t memory[VM_MEMORY_SIZE];
    bool running;
    InterruptHandler interrupt_handlers[VM_INTERRUPT_COUNT];
    int error_code;
    TypedValue* stack;
    size_t stack_size;
    DataSection* data_sections[256];  
    size_t data_section_count;
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
VM_API void* VM_CALL vm_alloc(VM* vm, uint32_t size);
VM_API void VM_CALL vm_free(VM* vm, void* ptr);
VM_API bool VM_CALL vm_add_data_section(VM* vm, uint8_t type_tag, const void* data, uint16_t size);
VM_API const DataSection* VM_CALL vm_get_data_section(VM* vm, size_t index);
VM_API size_t VM_CALL vm_get_data_section_count(VM* vm);

#endif