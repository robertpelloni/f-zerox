#include "common.h"

s32 func_800B71D0() {
    return 0;
}

s32 func_800B7244() {
    return 0;
}

s32 func_800B759C() {
    return 0;
}

s32 func_800B7600() {
    return 0;
}

s32 func_800B7C40() {
    return 0;
}

s32 func_800B7CA4() {
    return 0;
}

s32 func_800B82C8() {
    return 0;
}

s32 func_800B84B8() {
    return 0;
}

s32 func_800B8598() {
    return 0;
}

s32 func_800B8640() {
    return 0;
}

s32 func_800B899C() {
    return 0;
}

s32 func_800B8DAC() {
    return 0;
}

s32 func_800B9240() {
    return 0;
}

s32 func_800B9C54() {
    return 0;
}

s32 func_800B9D3C() {
    return 0;
}

s32 func_800B9E28() {
    return 0;
}

s32 func_800B9E50() {
    return 0;
}

s32 func_800B9E94() {
    return 0;
}

void func_800B9EBC(s32 arg0, s32 arg1, s32 arg2) {
}

void func_800B9ECC(void) {
}

s32 func_800B9ED4() {
    return 0;
}

void func_800BA248(void) {
    func_800B6F58();
}

void func_800BA268(void) {
    func_800B079C(0, 0);
}

void func_800BA28C(u8 arg0) {
    func_800B6910(0xF0000000, arg0);
}

s32 func_800BA2B4() {
    return 0;
}

// Audio/Sound State Management

/**
 * @brief Sets global audio state variable gAudioStateA.
 */
void func_800BA2D0(s8 arg0) {
    gAudioStateA = arg0;
}

/**
 * @brief Sets global audio state variable gAudioStateB.
 */
void func_800BA2E0(s8 arg0) {
    gAudioStateB = arg0;
}

s32 func_800BA2F0() {
    return 0;
}

s32 func_800BA3E4() {
    return 0;
}

s32 func_800BA67C() {
    return 0;
}

s32 func_800BA710() {
    return 0;
}

s32 func_800BA7C0() {
    return 0;
}

s32 func_800BA7F8() {
    return 0;
}

s32 func_800BA8D8() {
    return 0;
}

s32 func_800BAA88() {
    return 0;
}

s32 func_800BAB34() {
    return 0;
}

s32 func_800BAB50() {
    return 0;
}

s32 func_800BAB68() {
    return 0;
}

s32 func_800BABA0() {
    return 0;
}

s32 func_800BAE5C() {
    return 0;
}

s32 n_alSynFreeFX() {
    return 0;
}

s32 func_800BAE98() {
    return 0;
}

s32 func_800BAEDC() {
    return 0;
}

void func_800BAF18(s32 arg0) {
}

void func_800BAF20(s32 arg0) {
}

void func_800BAF28(void) {
}

s32 func_800BAF30() {
    return 0;
}

s32 n_alSynRemovePlayer() {
    return 0;
}

s32 func_800BAFA4() {
    return 0;
}

/**
 * @brief Sets audio state gAudioStateC to 1 if currently 0.
 */
void func_800BAFF4(void) {
    if (gAudioStateC == 0) {
        gAudioStateC = 1;
    }
}
void func_800BB018(void) {
    func_800BAFA4(0x17);
}

/**
 * @brief Sets audio state gAudioStateD to 1.
 */
void func_800BB038(void) {
    gAudioStateD = 1;
}

s32 func_800BB048() {
    return 0;
}

s32 func_800BB078() {
    return 0;
}

void func_800BB09C(void) {
    if (gAudioStateC == 0) {
        gAudioStateC = 3;
    }
}

s32 func_800BB0C0() {
    return 0;
}

s32 func_800BB2E0() {
    return 0;
}

void func_800BB324(s8 arg0) {
    gAudioStateE = arg0;
}

s32 func_800BB334() {
    return 0;
}

s32 func_800BB370() {
    return 0;
}

void func_800BB39C(s32 arg0) {
    func_800B9ED4();
    func_800B6994();
}

s32 func_800BB3C4() {
    return 0;
}

void func_800BB45C(void) {
    gAudioStateF = 1;
}

s32 func_800BB46C() {
    return 0;
}

void func_800BB49C(void) {
    func_800B82C8();
    func_800B7CA4();
    func_800B8598();
    func_800B6994();
    gAudioStateG = func_800B5FB0();
}

s32 func_800BB4E0() {
    return 0;
}

s32 func_800BB61C() {
    return 0;
}
