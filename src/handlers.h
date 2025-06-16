#ifndef _HANDLERS_H
#define _HANDLERS_H

#include <stdint.h>

struct VM;
typedef void (*InterruptHandler)(struct VM* vm, uint8_t int_num);

#endif
