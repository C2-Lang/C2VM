#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../vm/vm.h"
#include "../interrupts/interrupts.h"
#include "../bytecode/bytecode.h"
#include "version.h"

int main(int argc, char *argv[]) {
    printf("C2VM %s\n", c2vmVer());
    
    if (argc < 2) {
        printf("\x1b[33mUsage: %s <file.c2b>\x1b[0m\n", argv[0]);
        return 1;
    }

    if(strcmp(argv[1], "--version") == 0) {
        printf("\x1b[32mC to the power of 2 Virtual Machine Version: %s\x1b[0m\n", c2vmVer());
        return 0;
    }

    #ifdef C2VM_DEBUG
        printf("C2VM starting in DEBUG mode\n");
    #endif

    if (argc != 2) {
        printf("\x1b[33mUsage: %s <file.c2b>\x1b[0m\n", argv[0]);
        return 1;
    }

    VM* vm = vm_create();
    vm_init_default_interrupts(vm);
    
    #ifdef C2VM_DEBUG
        printf("Loading bytecode file: %s\n", argv[1]);
    #endif

    if (!vm_load_bytecode(vm, argv[1])) {
        printf("Error: Could not load bytecode file %s\n", argv[1]);
        vm_destroy(vm);
        return 1;
    }

    #ifdef C2VM_DEBUG
        printf("Starting VM execution\n");
    #endif

    vm_execute(vm);
    
    #ifdef C2VM_DEBUG
        printf("VM execution completed\n");
    #endif

    vm_destroy(vm);
    return 0;
}