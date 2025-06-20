#include "memory.h"
#include "vm.h"
#include <string.h>

#define MEMORY_HEADER_SIZE sizeof(MemoryBlock)
#define MEMORY_ALIGN(size) ((size + 7) & ~7)

VM_API bool VM_CALL vm_memory_init(uint8_t* memory) {
    if (!memory) return false;
    
    MemoryBlock* first = (MemoryBlock*)memory;
    first->allocated = false;
    first->size = VM_MEMORY_SIZE - MEMORY_HEADER_SIZE;
    
    return true;
}

VM_API void* VM_CALL vm_memory_alloc(uint8_t* memory, uint32_t size) {
    if (!memory || size == 0) return NULL;
    
    size = MEMORY_ALIGN(size);
    MemoryBlock* current = (MemoryBlock*)memory;
    
    while ((uint8_t*)current < memory + VM_MEMORY_SIZE) {
        if (!current->allocated && current->size >= size) {
            if (current->size > size + MEMORY_HEADER_SIZE + 8) {
                MemoryBlock* next = (MemoryBlock*)((uint8_t*)current + MEMORY_HEADER_SIZE + size);
                next->allocated = false;
                next->size = current->size - size - MEMORY_HEADER_SIZE;
                current->size = size;
            }
            
            current->allocated = true;
            return (uint8_t*)current + MEMORY_HEADER_SIZE;
        }
        
        current = (MemoryBlock*)((uint8_t*)current + MEMORY_HEADER_SIZE + current->size);
    }
    
    return NULL;
}

VM_API void VM_CALL vm_memory_free(uint8_t* memory, void* ptr) {
    if (!memory || !ptr) return;
    
    MemoryBlock* block = (MemoryBlock*)((uint8_t*)ptr - MEMORY_HEADER_SIZE);
    block->allocated = false;
    
    MemoryBlock* next = (MemoryBlock*)((uint8_t*)block + MEMORY_HEADER_SIZE + block->size);
    if ((uint8_t*)next < memory + VM_MEMORY_SIZE && !next->allocated) {
        block->size += MEMORY_HEADER_SIZE + next->size;
    }
}

VM_API bool VM_CALL vm_memory_check(uint8_t* memory, void* ptr, uint32_t size) {
    if (!memory || !ptr) return false;
    
    uint8_t* p = (uint8_t*)ptr;
    return (p >= memory && p + size <= memory + VM_MEMORY_SIZE);
}

VM_API void VM_CALL vm_memory_cleanup(uint8_t* memory) {
    if (!memory) return;
    vm_memory_init(memory);
}
