#ifndef _REGISTERS_H
#define _REGISTERS_H

#include <stdint.h>
#include "types.h"

struct Registers {
    TypedValue r0;
    TypedValue r1;
    TypedValue r2;
    TypedValue r3;
    uint64_t ip;
    uint64_t sp;
    uint64_t bp;
    uint64_t flags;
};

#endif