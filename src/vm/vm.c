#ifndef VM_BUILD
#define VM_BUILD
#endif

#include "vm.h"
#include "opcodes.h"
#include "../interrupts/interrupts.h"
#include "../syscalls/syscalls.h"  
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "../core/platform.h"
#include "memory.h"
#include "functions.h"

VM_API bool VM_CALL vm_add_data_section(VM* vm, uint8_t type_tag, const void* data, uint16_t size) {
    if (!vm || vm->data_section_count >= 256) return false;
    
    uint64_t offset = VM_DATA_SECTION_START;
    if (vm->data_section_count > 0) {
        DataSection* last = vm->data_sections[vm->data_section_count - 1];
        offset = ((uintptr_t)last->data - (uintptr_t)vm->memory) + last->size;
    }
    
    if (offset + size >= VM_CODE_SECTION_START) return false;
    memcpy(&vm->memory[offset], data, size);
    
    DataSection* section = malloc(sizeof(DataSection));
    if (!section) return false;
    
    section->magic = VM_DATA_SECTION_MAGIC;
    section->size = size;
    section->type_tag = type_tag;
    section->data = &vm->memory[offset];
    
    vm->data_sections[vm->data_section_count++] = section;
    return true;
}

VM_API const DataSection* VM_CALL vm_get_data_section(VM* vm, size_t index) {
    if (!vm || index >= vm->data_section_count) return NULL;
    return vm->data_sections[index];
}

VM_API size_t VM_CALL vm_get_data_section_count(VM* vm) {
    return vm ? vm->data_section_count : 0;
}

VM_API VM* VM_CALL vm_create() {
    VM* vm = (VM*)malloc(sizeof(VM));
    if (!vm) return NULL;
    
    vm->running = true;
    vm->error_code = VM_ERROR_NONE;
    memset(&vm->registers, 0, sizeof(struct Registers));
    vm->registers.sp = VM_STACK_START + 1;
    vm->registers.bp = VM_STACK_START + 1;
    memset(vm->memory, 0, VM_MEMORY_SIZE);
    memset(vm->interrupt_handlers, 0, sizeof(InterruptHandler) * VM_INTERRUPT_COUNT);
    
    if (!vm_memory_init(vm->memory)) {
        free(vm);
        return NULL;
    }
    
    vm->data_section_count = 0;
    memset(vm->data_sections, 0, sizeof(vm->data_sections));
    
    return vm;
}

VM_API void VM_CALL vm_destroy(VM* vm) {
    if (!vm) return;
    
    for (size_t i = 0; i < vm->data_section_count; i++) {
        free(vm->data_sections[i]);
    }
    
    free(vm);
}

VM_API void VM_CALL vm_register_interrupt(VM* vm, int int_num, InterruptHandler handler) {
    if (!vm) return;
    if (int_num >= VM_INTERRUPT_COUNT || int_num < 0) return;
    vm->interrupt_handlers[int_num] = handler;
}

VM_API int VM_CALL vm_get_error(VM* vm) {
    if (!vm) return VM_ERROR_NULL_POINTER;
    return vm->error_code;
}

VM_API bool VM_CALL vm_check_memory_bounds(VM* vm, uint64_t addr) {
    if (!vm) return false;
    if (addr >= VM_MEMORY_SIZE) {
        printf("Invalid memory access (address: %llu)", addr);
        fflush(stdout);
        vm->error_code = VM_ERROR_INVALID_MEMORY_ACCESS;
        vm->running = false;
        return false;
    }
    return true;
}

VM_API void* VM_CALL vm_alloc(VM* vm, uint32_t size) {
    if (!vm) return NULL;
    void* ptr = vm_memory_alloc(vm->memory, size);
    if (!ptr) {
        vm->error_code = VM_ERROR_OUT_OF_MEMORY;
    }
    return ptr;
}

VM_API void VM_CALL vm_free(VM* vm, void* ptr) {
    if (!vm) return;
    if (!vm_memory_check(vm->memory, ptr, 0)) {
        vm->error_code = VM_ERROR_INVALID_FREE;
        return;
    }
    vm_memory_free(vm->memory, ptr);
}

VM_API void VM_CALL vm_execute(VM* vm) {
    if (!vm) return;

    while (vm->running) {
        if (!vm_check_memory_bounds(vm, vm->registers.ip)) {
            vm->error_code = VM_ERROR_INVALID_MEMORY_ACCESS;
            if (vm->interrupt_handlers[INT_INVALID_MEMORY]) {
                vm->interrupt_handlers[INT_INVALID_MEMORY](vm, INT_INVALID_MEMORY);
            }
            break;
        }

        uint8_t opcode = vm->memory[vm->registers.ip++];
        
        #ifdef C2VM_DEBUG
            printf("Executing opcode: 0x%02x at IP: %llu\n", opcode, vm->registers.ip-1);
        #endif
        
        switch (opcode) {
            case OP_HALT:
                #if defined(C2VM_DEBUG)
                    printf("HALT: Stopping VM\n");
                #endif
                vm->running = false;
                break;
            case OP_LOAD: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                uint8_t value = vm->memory[vm->registers.ip++];
                TypedValue* regPtr = (TypedValue*)((uint8_t*)&vm->registers + offsetof(struct Registers, r0) + reg * sizeof(TypedValue));
                regPtr->value.u64 = value;
                #if defined(C2VM_DEBUG)
                    printf("LOAD: R%d = %d\n", reg, value);
                #endif
                break;
            }
            case OP_STORE: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                uint8_t addr = vm->memory[vm->registers.ip++];
                if (!vm_check_memory_bounds(vm, addr)) {
                    break;
                }
                uint8_t value = ((uint64_t*)&vm->registers)[reg] & 0xFF;
                vm->memory[addr] = value;
                break;
            }
            case OP_ADD: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg1] += ((uint64_t*)&vm->registers)[reg2];
                #if defined(C2VM_DEBUG)
                    printf("ADD: R%d += R%d (= %llu)\n", reg1, reg2, ((uint64_t*)&vm->registers)[reg1]);
                #endif
                break;
            }
            case OP_SUB: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg1] -= ((uint64_t*)&vm->registers)[reg2];
                #if defined(C2VM_DEBUG)
                    printf("SUB: R%d -= R%d (= %llu)\n", reg1, reg2, ((uint64_t*)&vm->registers)[reg1]);
                #endif
                break;
            }
            case OP_MUL: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg1] *= ((uint64_t*)&vm->registers)[reg2];
                #if defined(C2VM_DEBUG)
                    printf("MUL: R%d *= R%d (= %llu)\n", reg1, reg2, ((uint64_t*)&vm->registers)[reg1]);
                #endif
                break;
            }
            case OP_DIV: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                if (((uint64_t*)&vm->registers)[reg2] == 0) {
                    #if defined(C2VM_DEBUG)
                        printf("DIV: Division by zero attempted!\n");
                    #endif
                    if (vm->interrupt_handlers[INT_DIVIDE_BY_ZERO]) {
                        vm->interrupt_handlers[INT_DIVIDE_BY_ZERO](vm, INT_DIVIDE_BY_ZERO);
                    }
                    break;
                }
                ((uint64_t*)&vm->registers)[reg1] /= ((uint64_t*)&vm->registers)[reg2];
                #if defined(C2VM_DEBUG)
                    printf("DIV: R%d /= R%d (= %llu)\n", reg1, reg2, ((uint64_t*)&vm->registers)[reg1]);
                #endif
                break;
            }
            case OP_AND: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg1] &= ((uint64_t*)&vm->registers)[reg2];
                break;
            }
            case OP_OR: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg1] |= ((uint64_t*)&vm->registers)[reg2];
                break;
            }
            case OP_XOR: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg1] ^= ((uint64_t*)&vm->registers)[reg2];
                break;
            }
            case OP_NOT: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                ((uint64_t*)&vm->registers)[reg] = ~((uint64_t*)&vm->registers)[reg];
                break;
            }
            case OP_EQ: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                vm->registers.flags = (((uint64_t*)&vm->registers)[reg1] == ((uint64_t*)&vm->registers)[reg2]) ? 1 : 0;
                break;
            }
            case OP_LT: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                vm->registers.flags = (((uint64_t*)&vm->registers)[reg1] < ((uint64_t*)&vm->registers)[reg2]) ? 1 : 0;
                break;
            }
            case OP_GT: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                vm->registers.flags = (((uint64_t*)&vm->registers)[reg1] > ((uint64_t*)&vm->registers)[reg2]) ? 1 : 0;
                break;
            }
            case OP_PUSH: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                if (vm->registers.sp == 0) {
                    vm->error_code = VM_ERROR_STACK_OVERFLOW;
                    if (vm->interrupt_handlers[INT_STACK_OVERFLOW]) {
                        vm->interrupt_handlers[INT_STACK_OVERFLOW](vm, INT_STACK_OVERFLOW);
                    }
                    break;
                }
                vm->memory[vm->registers.sp--] = ((uint64_t*)&vm->registers)[reg];
                break;
            }
            case OP_POP: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                if (vm->registers.sp >= VM_STACK_START) {
                    vm->error_code = VM_ERROR_STACK_UNDERFLOW;
                    if (vm->interrupt_handlers[INT_STACK_UNDERFLOW]) {
                        vm->interrupt_handlers[INT_STACK_UNDERFLOW](vm, INT_STACK_UNDERFLOW);
                    }
                    break;
                }
                ((uint64_t*)&vm->registers)[reg] = vm->memory[++vm->registers.sp];
                break;
            }
            case OP_SYSCALL: {
                if (vm->registers.r0.type == TYPE_UINT64 && 
                    vm->registers.r0.value.u64 == SYS_PUTS && 
                    vm->registers.r1.type == TYPE_UINT64) {
                        
                    uint64_t addr = vm->registers.r1.value.u64;
                    if (addr >= VM_DATA_SECTION_START && addr < VM_CODE_SECTION_START) {
                        const char* str = (const char*)&vm->memory[addr];
                        printf("%s", str);
                        fflush(stdout);
                    }
                }
                vm_handle_syscall(vm);
                break;
            }

            case OP_LOAD_TYPE: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                uint8_t type = vm->memory[vm->registers.ip++];
                TypedValue* regPtr = (TypedValue*)((uint8_t*)&vm->registers + offsetof(struct Registers, r0) + reg * sizeof(TypedValue));
                regPtr->type = type;
                break;
            }

            case OP_CAST: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                uint8_t type = vm->memory[vm->registers.ip++];
                TypedValue* val = &((TypedValue*)&vm->registers)[reg];
                val->type = type;
                break;
            }

            case OP_TYPE_EQ: {
                uint8_t reg1 = vm->memory[vm->registers.ip++];
                uint8_t reg2 = vm->memory[vm->registers.ip++];
                TypedValue* val1 = &((TypedValue*)&vm->registers)[reg1];
                TypedValue* val2 = &((TypedValue*)&vm->registers)[reg2];
                vm->registers.flags = (val1->type == val2->type) ? 1 : 0;
                break;
            }

            case OP_JZ: {
                uint8_t offset = vm->memory[vm->registers.ip++];
                if (vm->registers.flags == 0) {
                    vm->registers.ip += offset;
                }
                break;
            }
            
            case OP_JMP: {
                uint8_t offset = vm->memory[vm->registers.ip++];
                vm->registers.ip += offset;
                break;
            }

            case OP_CALL: {
                uint64_t target = 0;
                for (int i = 0; i < 8; ++i) {
                    target |= ((uint64_t)vm->memory[vm->registers.ip++]) << (i * 8);
                }
                if (vm->registers.sp - 16 < VM_STACK_END) {
                    vm->error_code = VM_ERROR_STACK_OVERFLOW;
                    if (vm->interrupt_handlers[INT_STACK_OVERFLOW]) {
                        vm->interrupt_handlers[INT_STACK_OVERFLOW](vm, INT_STACK_OVERFLOW);
                    }
                    break;
                }
                vm_push_frame(vm, vm->registers.ip, vm->registers.bp);
                vm->registers.ip = target;
                break;
            }
            case OP_RET: {
                if (vm->registers.sp + 16 > VM_STACK_START) {
                    vm->error_code = VM_ERROR_STACK_UNDERFLOW;
                    if (vm->interrupt_handlers[INT_STACK_UNDERFLOW]) {
                        vm->interrupt_handlers[INT_STACK_UNDERFLOW](vm, INT_STACK_UNDERFLOW);
                    }
                    break;
                }
                uint64_t ret_addr = 0, old_bp = 0;
                vm_pop_frame(vm, &ret_addr, &old_bp);
                vm->registers.ip = ret_addr;
                break;
            }
            default:
                vm->error_code = VM_ERROR_INVALID_OPCODE;
                if (vm->interrupt_handlers[INT_INVALID_OPCODE]) {
                    vm->interrupt_handlers[INT_INVALID_OPCODE](vm, INT_INVALID_OPCODE);
                }
                break;
        }

        if (vm->error_code != VM_ERROR_NONE) {
            break;
        }
    }
}