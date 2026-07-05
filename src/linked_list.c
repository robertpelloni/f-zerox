#include "common.h"

void func_800AA6D0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AA7D4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AA84C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AA8E4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AA940(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AACF0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AAD2C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AAD58(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AAD84(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AAF5C(s32 arg0) {
    func_800AAD84(arg0, 6);
}

void func_800AAF7C(s32 arg0) {
    func_800AAD84(arg0, 7);
}

/**
 * @brief Initializes a circular linked list node.
 *
 * Sets the next and prev pointers (unk0, unk4) to point to itself,
 * effectively creating an empty list or isolated node.
 *
 * @param arg0 Pointer to the list node.
 */
void func_800AAF9C(struct UnkStruct_1* arg0) {
    arg0->unk0 = arg0;
    arg0->unk4 = arg0;
    arg0->unk8 = 0;
}

void func_800AAFAC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AAFFC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB0AC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB204(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB340(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB380(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB3AC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB41C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB51C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB550(void) { /* TODO: SHIFTABLE - Implement */ }

s32 func_800AB57C(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 sp1C;

    temp_v0 = func_800AC830();
    if (temp_v0 != 0) {
        func_800AB41C(temp_v0, arg1);
        func_800AB340(arg0 + 0x30, temp_v0);
    }
    sp1C = temp_v0;
    return sp1C;
}

s32 func_800AB5C8(s32 arg0, s32 arg1) {
    s32 sp1C;

    sp1C = func_800AC830(arg0 + 0x10);
    if (sp1C != 0) {
        func_800AB550(sp1C, arg1);
        func_800AC7F0(arg0 + 0x20, sp1C);
    }
    return sp1C;
}


void func_800AB618(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB710(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800AB888(void) { /* TODO: SHIFTABLE - Implement */ }
