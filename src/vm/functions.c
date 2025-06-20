#include "functions.h"

void vm_push_frame(VM* vm, uint64_t ret_addr, uint64_t old_bp) {
    for (int i = 0; i < 8; ++i)
        vm->memory[vm->registers.sp--] = (old_bp >> (i * 8)) & 0xFF;
    for (int i = 0; i < 8; ++i)
        vm->memory[vm->registers.sp--] = (ret_addr >> (i * 8)) & 0xFF;
    vm->registers.bp = vm->registers.sp + 1;
}

void vm_pop_frame(VM* vm, uint64_t* ret_addr, uint64_t* old_bp) {
    *ret_addr = 0;
    *old_bp = 0;
    vm->registers.sp++;
    for (int i = 0; i < 8; ++i)
        *ret_addr |= ((uint64_t)vm->memory[vm->registers.sp++]) << (i * 8);
    for (int i = 0; i < 8; ++i)
        *old_bp |= ((uint64_t)vm->memory[vm->registers.sp++]) << (i * 8);
    vm->registers.bp = *old_bp;
}
