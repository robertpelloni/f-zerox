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
 * @brief Sets global audio state variable gAudioStateParamA (formerly D_800D1A3C).
 */
void AudioState_SetParamA(s8 arg0) {
    gAudioStateParamA = arg0;
}

void func_800BA2D0(s8 arg0) {
    AudioState_SetParamA(arg0);
}

/**
 * @brief Sets global audio state variable gAudioStateParamB (formerly D_800D1A20).
 */
void AudioState_SetParamB(s8 arg0) {
    gAudioStateParamB = arg0;
}

void func_800BA2E0(s8 arg0) {
    AudioState_SetParamB(arg0);
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

void n_alSynFreeFX(ALSynth *s, void *fx) { (void)s; (void)fx; /* TODO: SHIFTABLE - Implement */ }

void func_800BAE98(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAEDC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BAF18(s32 arg0) {
    (void)arg0;
}

void func_800BAF20(s32 arg0) {
    (void)arg0;
}

void func_800BAF28(void) {
}

void func_800BAF30(void) { /* TODO: SHIFTABLE - Implement */ }

void n_alSynRemovePlayer(ALSynth *s, ALPlayer *p) { (void)s; (void)p; /* TODO: SHIFTABLE - Implement */ }

void func_800BAFA4(s32 arg0) { (void)arg0; /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Sets audio state gAudioStateActiveFlag (formerly D_800D1A48) to 1 if currently 0.
 */
void AudioState_Activate(void) {
    if (gAudioStateActiveFlag == 0) {
        gAudioStateActiveFlag = 1;
    }
}

void func_800BAFF4(void) {
    AudioState_Activate();
}

void func_800BB018(void) {
    func_800BAFA4(0x17);
}

/**
 * @brief Sets audio state gAudioStatePlayTrigger (formerly D_800D19E0) to 1.
 */
void AudioState_TriggerPlay(void) {
    gAudioStatePlayTrigger = 1;
}

void func_800BB038(void) {
    AudioState_TriggerPlay();
}

void func_800BB048(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB078(void) { /* TODO: SHIFTABLE - Implement */ }

void AudioState_SetAlternativeMode(void) {
    if (gAudioStateActiveFlag == 0) {
        gAudioStateActiveFlag = 3;
    }
}

void func_800BB09C(void) {
    AudioState_SetAlternativeMode();
}

void func_800BB0C0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB2E0(void) { /* TODO: SHIFTABLE - Implement */ }

void AudioState_SetParamC(s8 arg0) {
    gAudioStateParamC = arg0;
}

void func_800BB324(s8 arg0) {
    AudioState_SetParamC(arg0);
}

void func_800BB334(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB370(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB39C(s32 arg0) {
    func_800B9ED4();
    func_800B6994();
}

void func_800BB3C4(void) { /* TODO: SHIFTABLE - Implement */ }

void AudioState_TriggerReset(void) {
    gAudioStateResetFlag = 1;
}

void func_800BB45C(void) {
    AudioState_TriggerReset();
}

void func_800BB46C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB49C(void) {
    func_800B82C8();
    func_800B7CA4();
    func_800B8598();
    func_800B6994();
    gAudioStateStatus = AudioState_GetStatus();
}

void func_800BB4E0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800BB61C(void) { /* TODO: SHIFTABLE - Implement */ }
