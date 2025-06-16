#ifndef _C2BGEN_H
#define _C2BGEN_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef _WIN32
    #ifdef C2BGEN_EXPORTS
        #define C2BGEN_API __declspec(dllexport)
    #else
        #define C2BGEN_API __declspec(dllimport)
    #endif
#else
    #define C2BGEN_API
#endif

typedef struct BytecodeBuilder BytecodeBuilder;

C2BGEN_API BytecodeBuilder* c2b_create(void);
C2BGEN_API void c2b_destroy(BytecodeBuilder* builder);
C2BGEN_API void c2b_emit_halt(BytecodeBuilder* builder);
C2BGEN_API void c2b_emit_load(BytecodeBuilder* builder, uint8_t reg, uint8_t value);
C2BGEN_API void c2b_emit_store(BytecodeBuilder* builder, uint8_t reg, uint8_t addr);
C2BGEN_API void c2b_emit_add(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2);
C2BGEN_API void c2b_emit_sub(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2);
C2BGEN_API void c2b_emit_mul(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2);
C2BGEN_API void c2b_emit_div(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2);
C2BGEN_API void c2b_emit_push(BytecodeBuilder* builder, uint8_t reg);
C2BGEN_API void c2b_emit_pop(BytecodeBuilder* builder, uint8_t reg);
C2BGEN_API bool c2b_save(BytecodeBuilder* builder, const char* filename);
C2BGEN_API size_t c2b_get_size(BytecodeBuilder* builder);
C2BGEN_API const uint8_t* c2b_get_bytecode(BytecodeBuilder* builder);

#endif
