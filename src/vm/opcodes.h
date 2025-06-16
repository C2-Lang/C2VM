#ifndef _OPCODES_H
#define _OPCODES_H

typedef enum {
    OP_HALT = 0x00,
    OP_LOAD = 0x01,
    OP_STORE = 0x02,
    OP_ADD = 0x03,
    OP_SUB = 0x04,
    OP_PUSH = 0x05,
    OP_POP = 0x06,
    OP_JMP = 0x07,
    OP_MUL = 0x08,
    OP_DIV = 0x09,
    OP_AND = 0x0A,
    OP_OR  = 0x0B,
    OP_XOR = 0x0C,
    OP_NOT = 0x0D,
    OP_EQ  = 0x0E,
    OP_LT  = 0x0F,
    OP_GT  = 0x10,
    OP_JZ  = 0x11,
    OP_JNZ = 0x12,
    OP_JGT = 0x13,
    OP_JLT = 0x14,
    OP_CALL = 0x15,
    OP_RET = 0x16,
    OP_INT = 0x17,
    OP_IRET = 0x18,
    OP_SYSCALL = 0x19  // New syscall opcode
} OpCode;

#endif