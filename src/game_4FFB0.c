#include "common.h"

void func_800B5FB0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B641C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6744(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B679C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B67EC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6894(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B68EC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6910(s32 a, u8 b) { (void)a; (void)b; /* TODO: SHIFTABLE - Implement */ }

void func_800B6934(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6964(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6994(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Updates global state variables in low memory (input related).
 */
void Input_UpdateLowMemState(void) {
    gLowMemInputCurrent = gLowMemInputPrev;
    gLowMemInputFlag = 0;
}

void func_800B6A18(void) {
    Input_UpdateLowMemState();
}

void func_800B6A34(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6D90(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6DF4(void) {
    func_800AF820();
}

void func_800B6E14(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6E6C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6EC0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6F58(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6FA4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B7000(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B7030(void) { /* TODO: SHIFTABLE - Implement */ }

void Input_ClearLowMemStatus(void) {
    gLowMemStatus = 0;
}

void func_800B7060(void) {
    Input_ClearLowMemStatus();
}

void func_800B706C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B71A4(void) { /* TODO: SHIFTABLE - Implement */ }
