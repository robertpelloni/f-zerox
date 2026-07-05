#include "common.h"

void func_800B71D0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B7244(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B759C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B7600(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B7C40(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B7CA4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B82C8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B84B8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B8598(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B8640(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B899C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B8DAC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9240(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9C54(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9D3C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9E28(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9E50(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9E94(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B9EBC(s32 arg0, s32 arg1, s32 arg2) {
}

void func_800B9ECC(void) {
}

void func_800B9ED4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA248(void) {
    func_800B6F58();
}

void func_800BA268(void) {
    func_800B079C(0, 0);
}

void func_800BA28C(u8 arg0) {
    func_800B6910(0xF0000000, arg0);
}

void func_800BA2B4(void) { /* TODO: SHIFTABLE - Implement */ }

// Audio/Sound State Management

/**
 * @brief Sets global audio state variable D_800D1A3C.
 */
void func_800BA2D0(s8 arg0) {
    D_800D1A3C = arg0;
}

/**
 * @brief Sets global audio state variable D_800D1A20.
 */
void func_800BA2E0(s8 arg0) {
    D_800D1A20 = arg0;
}

void func_800BA2F0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA3E4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA67C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA710(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA7C0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA7F8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BA8D8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAA88(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAB34(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAB50(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAB68(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BABA0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAE5C(void) { /* TODO: SHIFTABLE - Implement */ }

void n_alSynFreeFX(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAE98(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAEDC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAF18(s32 arg0) {
}

void func_800BAF20(s32 arg0) {
}

void func_800BAF28(void) {
}

void func_800BAF30(void) { /* TODO: SHIFTABLE - Implement */ }

void n_alSynRemovePlayer(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAFA4(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Sets audio state D_800D1A48 to 1 if currently 0.
 */
void func_800BAFF4(void) {
    if (D_800D1A48 == 0) {
        D_800D1A48 = 1;
    }
}
void func_800BB018(void) {
    func_800BAFA4(0x17);
}

/**
 * @brief Sets audio state D_800D19E0 to 1.
 */
void func_800BB038(void) {
    D_800D19E0 = 1;
}

void func_800BB048(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB078(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB09C(void) {
    if (D_800D1A48 == 0) {
        D_800D1A48 = 3;
    }
}

void func_800BB0C0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB2E0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB324(s8 arg0) {
    D_800D1A18 = arg0;
}

void func_800BB334(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB370(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB39C(s32 arg0) {
    func_800B9ED4();
    func_800B6994();
}

void func_800BB3C4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB45C(void) {
    D_800D1A0C = 1;
}

void func_800BB46C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB49C(void) {
    func_800B82C8();
    func_800B7CA4();
    func_800B8598();
    func_800B6994();
    D_800D1C18 = func_800B5FB0();
}

void func_800BB4E0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB61C(void) { /* TODO: SHIFTABLE - Implement */ }
