#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

#include "vm.h"

#ifdef __cplusplus
extern "C" {
#endif

void vm_push_frame(VM* vm, uint64_t ret_addr, uint64_t old_bp);
void vm_pop_frame(VM* vm, uint64_t* ret_addr, uint64_t* old_bp);

#ifdef __cplusplus
}
#endif

#endif
