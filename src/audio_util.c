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

/**
 * @brief Initializes the timing subsystem.
 *
 * Marks timing system state as initialized. Formerly func_8007D9D0.
 */
void Timing_SystemInit(void) {
    gTimingSystemInitialized = 1;
}

void func_8007D9D0(void) {
    Timing_SystemInit();
}

void func_8007DABC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DB28(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DED8(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007DEF0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E008(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E038(void) { /* TODO: SHIFTABLE - Implement */ }

void osAfterPreNMI(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Sets a sound/audio ID.
 *
 * Masks the argument to 8 bits and forwards to the audio library function func_800BAF30.
 *
 * @param soundId The sound effect ID.
 */
void Audio_SetSoundId(s32 soundId) {
    extern void func_800BAF30(s32);
    func_800BAF30(soundId & 0xFF);
}

void func_8007E0AC(s32 arg0) {
    Audio_SetSoundId(arg0);
}

void n_alSeqpDelete(ALSeqPlayer *seqp) { (void)seqp; /* TODO: SHIFTABLE - Implement */ }

void func_8007E0EC(void) {
    extern void func_800BB078(void);
    func_800BB078();
}

/**
 * @brief Gets a byte from the sound effect bank map.
 *
 * @param idx Byte index.
 * @return s8 The byte value at gSoundEffectBankMap + idx.
 */
s8 Audio_GetSoundEffectByte(s32 idx) {
    if (idx < 0 || idx >= 256) return 0;
    return gSoundEffectBankMap[idx];
}

s8 func_8007E10C(s32 arg0) {
    return Audio_GetSoundEffectByte(arg0);
}

void func_8007E11C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E1C0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E2B4(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007E398(void) { /* TODO: SHIFTABLE - Implement */ }
