#include "common.h"

/**
 * @brief Rotates three global variables in a cycle.
 *
 * Cycle: D_800DCD04 -> D_800DCD08 -> D_800DCD0C -> D_800DCD04
 */
void func_80067AE0(void) {
   s32 temp_t7;

    temp_t7 = D_800DCD08;
    D_800DCD08 = D_800DCD04;
    D_800DCD04 = D_800DCD0C;
    D_800DCD0C = temp_t7;
}

s32 func_80067B14() {
    return 0;
}

void func_80067BA8(void) {
    D_800DCCF8 = func_80076C08(D_800DCCF8);
}


s32 func_80067BD0() {
    return 0;
}

s32 func_80067C0C() {
    return 0;
}

s32 func_80067D64() {
    return 0;
}

s32 func_80067E98() {
    return 0;
}

s32 func_80068008() {
    return 0;
}
