#ifndef _BYTECODE_H
#define _BYTECODE_H

#include "../core/platform.h"
#include <stdint.h>
#include <stdbool.h>
#include "../vm/vm.h"

VM_API bool VM_CALL vm_load_bytecode(VM* vm, const char* path);
VM_API bool VM_CALL vm_save_bytecode(const uint8_t* program, size_t size, const char* path);

#endif
