#ifndef _MEMORY_H
#define _MEMORY_H

#include "../core/platform.h"
#include <stdint.h>
#include <stdbool.h>

#define MEM_PAGE_SIZE 4096
#define MEM_MAX_PAGES 256

typedef struct {
    bool allocated;
    uint32_t size;
} MemoryBlock;

VM_API bool VM_CALL vm_memory_init(uint8_t* memory);
VM_API void* VM_CALL vm_memory_alloc(uint8_t* memory, uint32_t size);
VM_API void VM_CALL vm_memory_free(uint8_t* memory, void* ptr);
VM_API bool VM_CALL vm_memory_check(uint8_t* memory, void* ptr, uint32_t size);
VM_API void VM_CALL vm_memory_cleanup(uint8_t* memory);

#endif
