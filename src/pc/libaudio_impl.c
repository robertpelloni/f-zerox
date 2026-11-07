#include "pc/ultra64.h"
#include <stdio.h>


// Functions called by game_511D0.c / audio_state.c
void func_800B6994(void) {}
s32 func_800B5FB0(void) { return 0; }
void func_800B6F58(void) {}
void func_800B6910(s32 a, u8 b) { (void)a; (void)b; }

// Variables used in game_511D0.c (externs)
s8 gAudioStateParamA = 0;
s8 gAudioStateParamB = 0;
u8 gAudioStateActiveFlag = 0;
s8 gAudioStatePlayTrigger = 0;
s8 gAudioStateParamC = 0;
s8 gAudioStateResetFlag = 0;
s32 gAudioStateStatus = 0;

// Variables used in game_14440.c / audio_util.c
s8 gSoundEffectBankMap = 0;
