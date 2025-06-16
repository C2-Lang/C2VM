#include <stdint.h>
#include <stdio.h>
#include "vm.h"
#include "interrupts.h"
#include "bytecode.h"

int main(int argc, char *argv[]) {
    #if defined(C2VM_DEBUG)
        printf("C2VM starting in DEBUG mode\n");
    #endif

    if (argc != 2) {
        printf("Usage: %s <file.c2b>\n", argv[0]);
        return 1;
    }

    VM* vm = vm_create();
    vm_init_default_interrupts(vm);
    
    #if defined(C2VM_DEBUG)
        printf("Loading bytecode file: %s\n", argv[1]);
    #endif

    if (!vm_load_bytecode(vm, argv[1])) {
        printf("Error: Could not load bytecode file %s\n", argv[1]);
        vm_destroy(vm);
        return 1;
    }

    #if defined(C2VM_DEBUG)
        printf("Starting VM execution\n");
    #endif

    vm_execute(vm);
    
    #if defined(C2VM_DEBUG)
        printf("VM execution completed\n");
    #endif

    vm_destroy(vm);
    return 0;
}