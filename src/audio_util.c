#include "common.h"

s32 func_8007A440() {
    return 0;
}

s32 func_8007A59C() {
    return 0;
}

s32 func_8007A828() {
    return 0;
}

s32 func_8007AB88() {
    return 0;
}

s32 func_8007ABA4() {
    return 0;
}

s32 func_8007AC48() {
    return 0;
}

s32 func_8007AD44() {
    return 0;
}

s32 func_8007AE70() {
    return 0;
}

s32 func_8007AE8C() {
    return 0;
}

s32 func_8007AF40() {
    return 0;
}

s32 func_8007AFF4() {
    return 0;
}

s32 func_8007B0A8() {
    return 0;
}

s32 func_8007B14C() {
    return 0;
}

s32 func_8007CDB0() {
    return 0;
}

s32 func_8007D9D0() {
    return 0;
}

s32 func_8007DABC() {
    return 0;
}

s32 func_8007DB28() {
    return 0;
}

s32 func_8007DED8() {
    return 0;
}

s32 func_8007DEF0() {
    return 0;
}

s32 func_8007E008() {
    return 0;
}

s32 func_8007E038() {
    return 0;
}

s32 osAfterPreNMI() {
    return 0;
}

/**
 * @brief Sets a sound/audio ID (likely).
 *
 * Masks the argument to 8 bits and calls the audio library function func_800BAF30.
 */
void func_8007E0AC(s32 arg0) {
    func_800BAF30(arg0 & 0xFF);
}

s32 n_alSeqpDelete() {
    return 0;
}

void func_8007E0EC(void) {
    func_800BB078();
}

/**
 * @brief Gets a byte from a global array.
 *
 * @param arg0 Index.
 * @return s8 The value at D_800D4690[arg0].
 */
s8 func_8007E10C(s32 arg0) {
    return *(&D_800D4690 + arg0);
}

s32 func_8007E11C() {
    return 0;
}

s32 func_8007E1C0() {
    return 0;
}

s32 func_8007E2B4() {
    return 0;
}

s32 func_8007E398() {
    return 0;
}
