#include "bytecode.h"
#include "../core/platform.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

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
        if (err != 0) return false;
    #else
        file = fopen(path, "rb");
        if (!file) return false;
    #endif
    
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    uint8_t* buffer = malloc(size);
    if (!buffer) {
        fclose(file);
        return false;
    }
    
    size_t bytes_read = fread(buffer, 1, size, file);
    fclose(file);
    
    uint8_t* ptr = buffer;
    uint8_t* end = buffer + bytes_read;
    uint8_t* code_start = ptr;
    
    while (ptr < end - 4) {
        if (ptr[0] == 0xDA && ptr[1] == 0x7A) {
            uint16_t section_size = (uint16_t)(ptr[2] | (ptr[3] << 8));
            if (ptr + 5 + section_size > end) break;
            
            uint8_t type_tag = ptr[4];
            uint8_t* data = ptr + 5;
            
            if (!vm_add_data_section(vm, type_tag, data, section_size)) {
                free(buffer);
                return false;
            }
            ptr = data + section_size;
            code_start = ptr;
        } else break;
    }
    
    size_t code_size = end - code_start;
    if (code_size > 0) {
        memcpy(&vm->memory[VM_CODE_SECTION_START], code_start, code_size);
        vm->registers.ip = VM_CODE_SECTION_START;
    }
    
    free(buffer);
    return true;
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