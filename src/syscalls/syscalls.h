#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include "../core/platform.h"
#include "../vm/vm.h"
#include <stdint.h>

// Syscall numbers
#define SYS_EXIT     0x00
#define SYS_PUTCHAR  0x01
#define SYS_GETCHAR  0x02
#define SYS_PUTS     0x03
#define SYS_GETS     0x04

VM_API void VM_CALL vm_init_syscalls(VM* vm);
VM_API void VM_CALL vm_handle_syscall(VM* vm);

#endif
