#include "common.h"

/**
 * @brief Retrieves the audio subsystem status.
 *
 * @return s32 The current status of gAudioStateStatus. Formerly func_800B5FB0.
 */
s32 AudioState_GetStatus(void) {
    return gAudioStateStatus;
}

s32 func_800B5FB0(void) {
    return AudioState_GetStatus();
}

void func_800B641C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6744(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B679C(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B67EC(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B6894(void) { /* TODO: SHIFTABLE - Implement */ }

void func_800B68EC(void) { /* TODO: SHIFTABLE - Implement */ }

/**
 * @brief Dispatches audio state command and parameter.
 *
 * @param cmd The command code to send.
 * @param param Associated 8-bit parameter.
 */
void AudioState_DispatchCommand(s32 cmd, u8 param) {
    gAudioLastCommand = cmd;
    gAudioLastParam = param;
}

void func_800B6910(s32 a, u8 b) {
    AudioState_DispatchCommand(a, b);
}

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
