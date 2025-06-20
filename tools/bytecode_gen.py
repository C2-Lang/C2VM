import sys
import os
from enum import IntEnum
import argparse

# Syscall numbers
SYS_EXIT = 0x00
SYS_PUTCHAR = 0x01
SYS_GETCHAR = 0x02
SYS_PUTS = 0x03
SYS_GETS = 0x04
SYS_ALLOC = 0x05
SYS_FREE = 0x06
SYS_OPEN = 0x07
SYS_CLOSE = 0x08
SYS_READ = 0x09
SYS_WRITE = 0x0A
SYS_SEEK = 0x0B

# Data section type tags
TYPE_TAG_VOID     = 0x00
TYPE_TAG_INT8     = 0x01
TYPE_TAG_INT16    = 0x02
TYPE_TAG_INT32    = 0x03
TYPE_TAG_INT64    = 0x04
TYPE_TAG_UINT8    = 0x05
TYPE_TAG_UINT16   = 0x06
TYPE_TAG_UINT32   = 0x07
TYPE_TAG_UINT64   = 0x08
TYPE_TAG_FLOAT32  = 0x09
TYPE_TAG_FLOAT64  = 0x0A
TYPE_TAG_BOOL     = 0x0B

class OpCode(IntEnum):
    HALT = 0x00
    LOAD = 0x01
    STORE = 0x02
    ADD = 0x03
    SUB = 0x04
    PUSH = 0x05
    POP = 0x06
    JMP = 0x07
    MUL = 0x08
    DIV = 0x09
    AND = 0x0A
    OR = 0x0B
    XOR = 0x0C
    NOT = 0x0D
    EQ = 0x0E
    LT = 0x0F
    GT = 0x10
    JZ = 0x11
    JNZ = 0x12
    JGT = 0x13
    JLT = 0x14
    CALL = 0x15
    RET = 0x16
    INT = 0x17
    IRET = 0x18
    SYSCALL = 0x19  # Add SYSCALL opcode
    LOAD_TYPE = 0x1A
    CAST = 0x1B
    TYPE_EQ = 0x1C

class ValueType(IntEnum):
    VOID = 0
    INT8 = 1
    INT16 = 2
    INT32 = 3
    INT64 = 4
    UINT8 = 5
    UINT16 = 6
    UINT32 = 7
    UINT64 = 8
    FLOAT32 = 9
    FLOAT64 = 10
    BOOL = 11

def generate_bytecode(instructions):
    bytecode = bytearray()
    for instr in instructions:
        if isinstance(instr, (bytes, bytearray)):
            bytecode.extend(instr)
        elif isinstance(instr, (list, tuple)):
            op = instr[0]
            args = instr[1:]
            if isinstance(op, int):
                bytecode.append(op)
            for arg in args:
                if isinstance(arg, bytes):
                    bytecode.extend(arg)
                elif isinstance(arg, int):
                    bytecode.append(arg & 0xFF)
                else:
                    bytecode.append(int(arg, 16) & 0xFF)
    return bytecode