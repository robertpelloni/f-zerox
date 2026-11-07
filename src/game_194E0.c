#include "common.h"

/**
 * @brief Sets system state flag and saves arguments.
 *
 * @param paramA First state parameter (formerly D_800E4264).
 * @param paramB Second state parameter (formerly D_800E4260).
 */
void System_SetStateFlags(s32 paramA, s32 paramB) {
    gSystemStateFlag = 1;
    gSystemStateParamA = paramA;
    gSystemStateParamB = paramB;
}

void func_8007F4E0(s32 arg0, s32 arg1) {
    System_SetStateFlags(arg0, arg1);
}

/**
 * @brief Initializes memory and system state parameters.
 */
void System_InitMemoryState(void) {
    static struct UnkStruct_0 sMemoryStateControl;
    sMemoryStateControl.unk0 = 0;
    sMemoryStateControl.unk1 = 0;
    sMemoryStateControl.unk2 = 0;
    sMemoryStateControl.unk3 = 0;
    (void)sMemoryStateControl;
    gSystemMemoryStateValue = 0;
}

void func_8007F500(void) {
    System_InitMemoryState();
}

void func_8007F520(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007F5EC(void) { /* TODO: SHIFTABLE - Implement */ }
