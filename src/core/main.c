#include "../vm/vm.h"
#include "../vm/opcodes.h"
#include "../interrupts/interrupts.h"
#include "../syscalls/syscalls.h"
#include "../bytecode/bytecode.h"
#include "version.h"
#include "args.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    if(ck_args(argc, argv) == 1) return 1;
    printf("C2VM %s\n", c2vmVer());
    
    if (argc != 2) {
        printf("Usage: %s <bytecode_file>\n", argv[0]);
        return 1;
    }
    
    VM* vm = vm_create();
    if (!vm) {
        printf("Failed to create VM\n");
        return 1;
    }

    vm_init_default_interrupts(vm);
    vm_init_syscalls(vm);

    if (!vm_load_bytecode(vm, argv[1])) {
        printf("Failed to load bytecode from %s\n", argv[1]);
        vm_destroy(vm);
        return 1;
    }

    vm_execute(vm);
    
    int error = vm_get_error(vm);
    if (error != VM_ERROR_NONE) fflush(stdout);  
    
    vm_destroy(vm);
    return error;
}
