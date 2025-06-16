import sys
import os
from enum import IntEnum
import argparse

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

def generate_bytecode(instructions):
    bytecode = bytearray()
    for instr in instructions:
        if isinstance(instr, (list, tuple)):
            op = instr[0]
            args = instr[1:]
            bytecode.append(op)
            for arg in args:
                bytecode.append(arg if isinstance(arg, int) else int(arg, 16))
        else:
            bytecode.append(instr if isinstance(instr, int) else int(instr, 16))
    return bytecode
