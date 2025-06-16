#ifndef _BYTECODE_H
#define _BYTECODE_H

#include <stdint.h>
#include <stdbool.h>
#include "vm.h"

bool vm_load_bytecode(VM* vm, const char* path);
bool vm_save_bytecode(const uint8_t* program, size_t size, const char* path);

#endif
