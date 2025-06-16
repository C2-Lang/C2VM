#include "c2bgen.h"
#include "opcodes.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_BYTECODE_SIZE 1024 * 1024

struct BytecodeBuilder {
    uint8_t* bytecode;
    size_t size;
    size_t capacity;
};

static void c2b_emit_byte(BytecodeBuilder* builder, uint8_t byte) {
    if (builder->size < builder->capacity) {
        builder->bytecode[builder->size++] = byte;
    }
}

C2BGEN_API BytecodeBuilder* c2b_create(void) {
    BytecodeBuilder* builder = malloc(sizeof(BytecodeBuilder));
    builder->bytecode = malloc(MAX_BYTECODE_SIZE);
    builder->size = 0;
    builder->capacity = MAX_BYTECODE_SIZE;
    return builder;
}

C2BGEN_API void c2b_destroy(BytecodeBuilder* builder) {
    free(builder->bytecode);
    free(builder);
}

C2BGEN_API void c2b_emit_halt(BytecodeBuilder* builder) {
    c2b_emit_byte(builder, OP_HALT);
}

C2BGEN_API void c2b_emit_load(BytecodeBuilder* builder, uint8_t reg, uint8_t value) {
    c2b_emit_byte(builder, OP_LOAD);
    c2b_emit_byte(builder, reg);
    c2b_emit_byte(builder, value);
}

C2BGEN_API void c2b_emit_store(BytecodeBuilder* builder, uint8_t reg, uint8_t addr) {
    c2b_emit_byte(builder, OP_STORE);
    c2b_emit_byte(builder, reg);
    c2b_emit_byte(builder, addr);
}

C2BGEN_API void c2b_emit_add(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2) {
    c2b_emit_byte(builder, OP_ADD);
    c2b_emit_byte(builder, reg1);
    c2b_emit_byte(builder, reg2);
}

C2BGEN_API void c2b_emit_sub(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2) {
    c2b_emit_byte(builder, OP_SUB);
    c2b_emit_byte(builder, reg1);
    c2b_emit_byte(builder, reg2);
}

C2BGEN_API void c2b_emit_mul(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2) {
    c2b_emit_byte(builder, OP_MUL);
    c2b_emit_byte(builder, reg1);
    c2b_emit_byte(builder, reg2);
}

C2BGEN_API void c2b_emit_div(BytecodeBuilder* builder, uint8_t reg1, uint8_t reg2) {
    c2b_emit_byte(builder, OP_DIV);
    c2b_emit_byte(builder, reg1);
    c2b_emit_byte(builder, reg2);
}

C2BGEN_API void c2b_emit_push(BytecodeBuilder* builder, uint8_t reg) {
    c2b_emit_byte(builder, OP_PUSH);
    c2b_emit_byte(builder, reg);
}

C2BGEN_API void c2b_emit_pop(BytecodeBuilder* builder, uint8_t reg) {
    c2b_emit_byte(builder, OP_POP);
    c2b_emit_byte(builder, reg);
}

C2BGEN_API bool c2b_save(BytecodeBuilder* builder, const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return false;
    size_t written = fwrite(builder->bytecode, 1, builder->size, file);
    fclose(file);
    return written == builder->size;
}

C2BGEN_API size_t c2b_get_size(BytecodeBuilder* builder) {
    return builder->size;
}

C2BGEN_API const uint8_t* c2b_get_bytecode(BytecodeBuilder* builder) {
    return builder->bytecode;
}
