#include "bytecode.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

bool vm_load_bytecode(VM* vm, const char* path) {
    #if defined(C2VM_DEBUG)
        printf("Loading bytecode from: %s\n", path);
    #endif
    
    FILE* file = fopen(path, "rb");
    if (!file) {
        #if defined(C2VM_DEBUG)
            printf("Error opening file: %s (%s)\n", path, strerror(errno));
        #endif
        return false;
    }
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (size > VM_MEMORY_SIZE) {
        #if defined(C2VM_DEBUG)
            printf("Bytecode file too large: %ld bytes\n", size);
        #endif
        fclose(file);
        return false;
    }
    
    size_t read = fread(vm->memory, 1, size, file);
    fclose(file);
    
    #if defined(C2VM_DEBUG)
        printf("Loaded %zu bytes of bytecode\n", read);
    #endif
    
    return read == (size_t)size;
}

bool vm_save_bytecode(const uint8_t* program, size_t size, const char* path) {
    FILE* file = fopen(path, "wb");
    if (!file) return false;
    
    size_t written = fwrite(program, 1, size, file);
    fclose(file);
    
    return written == size;
}