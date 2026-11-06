#include "common.h"

/**
 * @brief Rotates three global variables in a cycle.
 *
 * Cycle: gGameStateCycleA -> gGameStateCycleB -> gGameStateCycleC -> gGameStateCycleA
 * Formerly: D_800DCD04 -> D_800DCD08 -> D_800DCD0C -> D_800DCD04
 */
void GameState_RotateCycle(void) {
    s32 temp_t7;

    temp_t7 = gGameStateCycleB;
    gGameStateCycleB = gGameStateCycleA;
    gGameStateCycleA = gGameStateCycleC;
    gGameStateCycleC = temp_t7;
}

void func_80067AE0(void) {
    GameState_RotateCycle();
}

void func_80067B14(void) { /* TODO: SHIFTABLE - Implement */ }

void GameState_UpdateControlMask(void) {
    extern s32 func_80076C08(s32);
    gGameStateControlMask = func_80076C08(gGameStateControlMask);
}

void func_80067BA8(void) {
    GameState_UpdateControlMask();
}


void func_80067BD0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80067C0C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80067D64(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80067E98(void) { /* TODO: SHIFTABLE - Implement */ }

void func_80068008(void) { /* TODO: SHIFTABLE - Implement */ }
