#include "common.h"

void func_8007A440(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007A59C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007A828(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AB88(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007ABA4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AC48(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AD44(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AE70(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AE8C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AF40(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007AFF4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007B0A8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007B14C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007CDB0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007D9D0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DABC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DB28(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DED8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DEF0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E008(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E038(void) { /* TODO: SHIFTABLE - Implement */ }

void osAfterPreNMI(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Sets a sound/audio ID (likely).
 *
 * Masks the argument to 8 bits and calls the audio library function func_800BAF30.
 */
void func_8007E0AC(s32 arg0) {
    func_800BAF30(arg0 & 0xFF);
}

void n_alSeqpDelete(void) { /* TODO: SHIFTABLE - Implement */ }

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

void func_8007E11C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E1C0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E2B4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E398(void) { /* TODO: SHIFTABLE - Implement */ }
