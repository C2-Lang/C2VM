#ifndef VM_BUILD
#define VM_BUILD
#endif
#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>

static void sys_putchar(VM* vm) {
    // r1 contains the character to print (not r0)
    putchar((char)vm->registers.r1);
}

static void sys_getchar(VM* vm) {
    vm->registers.r0 = getchar();
}

static void sys_puts(VM* vm) {
    uint64_t addr = vm->registers.r0;
    while (addr < VM_MEMORY_SIZE && vm->memory[addr] != 0) {
        putchar(vm->memory[addr++]);
    }
}

static void sys_gets(VM* vm) {

    uint64_t addr = vm->registers.r0;
    uint64_t maxlen = vm->registers.r1;
    
    if (addr + maxlen >= VM_MEMORY_SIZE) {
        vm->error_code = VM_ERROR_INVALID_MEMORY_ACCESS;
        return;
    }

    uint64_t i = 0;
    int c;
    while (i < maxlen - 1 && (c = getchar()) != '\n' && c != EOF) {
        vm->memory[addr + i++] = (uint8_t)c;
    }
    vm->memory[addr + i] = 0;  
    vm->registers.r0 = i;  
}

static void sys_exit(VM* vm) {
    // r0 contains exit code
    vm->running = false;
}

VM_API void VM_CALL vm_handle_syscall(VM* vm) {
    uint8_t syscall_num = vm->registers.r0;
    switch (syscall_num) { 
        case SYS_EXIT:
            #ifdef C2VM_DEBUG
                printf("SYS_EXIT\n");
            #endif
            sys_exit(vm);
            break;
        case SYS_PUTCHAR:
            #ifdef C2VM_DEBUG
                printf("SYS_PUTCHAR('%c')\n", (char)vm->registers.r1);
            #endif
            sys_putchar(vm);
            fflush(stdout);
            break;
        case SYS_GETCHAR:
            sys_getchar(vm);
            break;
        case SYS_PUTS:
            sys_puts(vm);
            break;
        case SYS_GETS:
            sys_gets(vm);
            break;
        default:
            vm->error_code = VM_ERROR_INVALID_OPCODE;
            break;
    }
}

VM_API void VM_CALL vm_init_syscalls(VM* vm) {
    // Nothing to initialize yet
}
