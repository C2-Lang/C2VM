#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "../core/platform.h"
#include "../vm/vm.h"
#include <stdint.h>

#define SYS_EXIT     0x00
#define SYS_PUTCHAR  0x01
#define SYS_GETCHAR  0x02
#define SYS_PUTS     0x03
#define SYS_GETS     0x04

#define SYS_ALLOC    0x05
#define SYS_FREE     0x06
#define SYS_OPEN     0x07
#define SYS_CLOSE    0x08
#define SYS_READ     0x09
#define SYS_WRITE    0x0A
#define SYS_SEEK     0x0B

VM_API void VM_CALL vm_init_syscalls(VM* vm);
VM_API void VM_CALL vm_handle_syscall(VM* vm);

#endif
