#include "bytecode.h"
#include "../core/platform.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifndef VM_BUILD
#define VM_BUILD
#endif

VM_API bool VM_CALL vm_load_bytecode(VM* vm, const char* path) {
    #ifdef C2VM_DEBUG
        printf("Analying ByteCode from: %s\n", path);
    #endif
    
    FILE* file;
    #ifdef C2VM_WINDOWS
        errno_t err = fopen_s(&file, path, "rb");
        if (err != 0) {
            #if C2VM_DEBUG
                printf("Error opening file: %s (error: %d)\n", path, err);
            #endif
            return false;
        }
    #else
        file = fopen(path, "rb");
        if (!file) {
            #if C2VM_DEBUG
                printf("Error opening file: %s (%s)\n", path, strerror(errno));
            #endif
            return false;
        }
    #endif
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (size > VM_MEMORY_SIZE) {
        #ifdef C2VM_DEBUG
            printf("Bytecode file too large: %ld bytes\n", size);
        #endif
        fclose(file);
        return false;
    }
    
    size_t read = fread(vm->memory, 1, size, file);
    fclose(file);
    
    #ifdef C2VM_DEBUG
        printf("Loaded %zu bytes of bytecode\n", read);
    #endif
    
    return read == (size_t)size;
}

VM_API bool VM_CALL vm_save_bytecode(const uint8_t* program, size_t size, const char* path) {
    FILE* file;
    #ifdef C2VM_WINDOWS
        errno_t err = fopen_s(&file, path, "wb");
        if (err != 0) return false;
    #else
        file = fopen(path, "wb");
        if (!file) return false;
    #endif
    
    size_t written = fwrite(program, 1, size, file);
    fclose(file);
    return written == size;
}