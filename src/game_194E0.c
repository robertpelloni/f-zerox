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

void func_8007F500(void) { /* TODO: SHIFTABLE - Implement */ }
// void func_8007F500(void) {
//     static struct UnkStruct_0 D_800E42C9;
//     D_800E42C9.unk0 = 0;
//     D_800E42C9.unk1 = 0;
//     D_800E42C9.unk2 = 0;
//     D_800E42C9.unk3 = 0;
//     D_800E42CC = 0;
// }

void func_8007F520(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007F5EC(void) { /* TODO: SHIFTABLE - Implement */ }
