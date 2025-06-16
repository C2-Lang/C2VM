#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stdint.h>

struct Registers {
    uint64_t r0;
    uint64_t r1;
    uint64_t r2;
    uint64_t r3;
    uint64_t ip;
    uint64_t sp;
    uint64_t bp;
    uint64_t flags;
};

#endif