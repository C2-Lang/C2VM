#ifndef VM_BUILD
#define VM_BUILD
#endif
#include "syscalls.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_OPEN_FILES 256

static FILE* open_files[MAX_OPEN_FILES] = {0};

static int get_free_fd() {
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (open_files[i] == NULL) return i;
    }
    return -1;
}

static void sys_putchar(VM* vm) {
    putchar((char)vm->registers.r1.value.i64);
}

static void sys_getchar(VM* vm) {
    vm->registers.r0.type = TYPE_INT64;
    vm->registers.r0.value.i64 = getchar();
}

static void sys_puts(VM* vm) {
    uint64_t addr = vm->registers.r1.value.u64;
    while (addr < VM_MEMORY_SIZE && vm->memory[addr]) {
        putchar(vm->memory[addr++]);
    }
    fflush(stdout);
}

static void sys_gets(VM* vm) {
    uint64_t addr = vm->registers.r0.value.u64;
    uint64_t maxlen = vm->registers.r1.value.u64;

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
    vm->registers.r0.type = TYPE_UINT64;
    vm->registers.r0.value.u64 = i;
}

static void sys_exit(VM* vm) {
    vm->running = false;
}

static void sys_alloc(VM* vm) {
    uint32_t size = (uint32_t)vm->registers.r1.value.u64;
    void* ptr = vm_alloc(vm, size);
    vm->registers.r0.type = TYPE_UINT64;
    vm->registers.r0.value.u64 = (uint64_t)((uintptr_t)ptr - (uintptr_t)vm->memory);
}

static void sys_free(VM* vm) {
    uint64_t addr = vm->registers.r1.value.u64;
    void* ptr = &vm->memory[addr];
    vm_free(vm, ptr);
}

static void sys_open(VM* vm) {
    uint64_t path_addr = vm->registers.r1.value.u64;
    uint64_t mode = vm->registers.r2.value.u64;
    
    char path[256];
    size_t i = 0;
    while (i < 255 && vm->memory[path_addr + i]) {
        path[i] = vm->memory[path_addr + i];
        i++;
    }
    path[i] = '\0';
    
    FILE* f = fopen(path, mode == 0 ? "rb" : "wb");
    if (!f) {
        vm->registers.r0.type = TYPE_INT64;
        vm->registers.r0.value.i64 = -1;
        return;
    }
    
    int fd = get_free_fd();
    if (fd == -1) {
        fclose(f);
        vm->registers.r0.type = TYPE_INT64;
        vm->registers.r0.value.i64 = -1;
        return;
    }
    
    open_files[fd] = f;
    vm->registers.r0.type = TYPE_INT64;
    vm->registers.r0.value.i64 = fd;
}

static void sys_close(VM* vm) {
    int fd = (int)vm->registers.r1.value.i64;
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd]) {
        return;
    }
    fclose(open_files[fd]);
    open_files[fd] = NULL;
}

static void sys_read(VM* vm) {
    int fd = (int)vm->registers.r1.value.i64;
    uint64_t buf_addr = vm->registers.r2.value.u64;
    uint64_t size = vm->registers.r3.value.u64;
    
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd] || 
        !vm_check_memory_bounds(vm, buf_addr + size - 1)) {
        vm->registers.r0.type = TYPE_UINT64;
        vm->registers.r0.value.u64 = 0;
        return;
    }
    
    vm->registers.r0.type = TYPE_UINT64;
    vm->registers.r0.value.u64 = fread(&vm->memory[buf_addr], 1, size, open_files[fd]);
}

static void sys_write(VM* vm) {
    int fd = (int)vm->registers.r1.value.i64;
    uint64_t buf_addr = vm->registers.r2.value.u64;
    uint64_t size = vm->registers.r3.value.u64;
    
    if (fd < 0 || fd >= MAX_OPEN_FILES || !open_files[fd]) {
        vm->registers.r0.type = TYPE_UINT64;
        vm->registers.r0.value.u64 = 0;
        return;
    }
    
    size_t written = fwrite(&vm->memory[buf_addr], 1, size, open_files[fd]);
    fflush(open_files[fd]);
    vm->registers.r0.type = TYPE_UINT64;
    vm->registers.r0.value.u64 = written;
}

VM_API void VM_CALL vm_handle_syscall(VM* vm) {
    uint8_t syscall_num = (uint8_t)vm->registers.r0.value.u64;

    switch (syscall_num) { 
        case SYS_EXIT:
            #ifdef C2VM_DEBUG
                printf("SYS_EXIT\n");
            #endif
            sys_exit(vm);
            break;
        case SYS_PUTCHAR:
            #ifdef C2VM_DEBUG
                printf("SYS_PUTCHAR('%c')\n", (char)vm->registers.r1.value.i64);
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
        case SYS_ALLOC:
            sys_alloc(vm);
            break;
        case SYS_FREE:
            sys_free(vm);
            break;
        case SYS_OPEN:
            sys_open(vm);
            break;
        case SYS_CLOSE:
            sys_close(vm);
            break;
        case SYS_READ:
            sys_read(vm);
            break;
        case SYS_WRITE:
            sys_write(vm);
            break;
        default:
            vm->error_code = VM_ERROR_INVALID_OPCODE;
            break;
    }
}

VM_API void VM_CALL vm_init_syscalls(VM* vm) {
    (void)vm; 
    memset(open_files, 0, sizeof(open_files));
    open_files[0] = stdin;
    open_files[1] = stdout;
    open_files[2] = stderr;
}