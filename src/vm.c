#include "vm.h"
#include "opcodes.h"
#include "interrupts.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

VM* vm_create() {
    VM* vm = (VM*)malloc(sizeof(VM));
    vm->running = true;
    memset(&vm->registers, 0, sizeof(struct Registers));
    vm->registers.sp = VM_STACK_START;
    vm->registers.bp = VM_STACK_START;
    memset(vm->memory, 0, VM_MEMORY_SIZE);
    return vm;
}

void vm_destroy(VM* vm) {
    free(vm);
}

void vm_register_interrupt(VM* vm, uint8_t int_num, InterruptHandler handler) {
    vm->interrupt_handlers[int_num] = handler;
}

void vm_execute(VM* vm) {
    while (vm->running) {
        uint8_t opcode = vm->memory[vm->registers.ip++];
        #if defined(C2VM_DEBUG)
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
                ((uint64_t*)&vm->registers)[reg] = value;
                #if defined(C2VM_DEBUG)
                    printf("LOAD: R%d = %d\n", reg, value);
                #endif
                break;
            }
            case OP_STORE: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                uint8_t addr = vm->memory[vm->registers.ip++];
                vm->memory[addr] = ((uint64_t*)&vm->registers)[reg];
                #if defined(C2VM_DEBUG)
                    printf("STORE: [%d] = R%d (value: %llu)\n", addr, reg, ((uint64_t*)&vm->registers)[reg]);
                #endif
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
                if (vm->registers.sp > 0) {
                    vm->memory[vm->registers.sp--] = ((uint64_t*)&vm->registers)[reg];
                    #if defined(C2VM_DEBUG)
                        printf("PUSH: R%d (value: %llu) to stack\n", reg, ((uint64_t*)&vm->registers)[reg]);
                    #endif
                } else {
                    vm->running = false;
                }
                break;
            }
            case OP_POP: {
                uint8_t reg = vm->memory[vm->registers.ip++];
                if (vm->registers.sp < VM_STACK_START) {
                    ((uint64_t*)&vm->registers)[reg] = vm->memory[++vm->registers.sp];
                    #if defined(C2VM_DEBUG)
                        printf("POP: Stack to R%d (value: %llu)\n", reg, ((uint64_t*)&vm->registers)[reg]);
                    #endif
                } else {
                    vm->running = false;
                }
                break;
            }
            default:
                break;
        }
    }
}
