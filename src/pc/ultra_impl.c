#include "pc/ultra64.h"
#include "pc/hal.h"
#include "pc/debug_text_hook.h"
#include "structs.h"
#include <stdio.h>

// Mock Global Variables (usually defined in ld script or asm)
void* gCurrentFramebuffer = NULL;
s32 D_800D46C0 = 0; // gDebugPrintFormat?
s32 D_800D46C8 = 0;
s32 D_800D46D8 = 0;
s32 D_800D46FC = 0;
s32 D_800D471C = 0;
s32 D_800D473C = 0;
s32 D_800D474C = 0;
s32 D_800D476C = 0;
s32 D_800D478C = 0;
s32 D_800D479C = 0;
s32 D_800D47B4 = 0;
s32 D_800D47D0 = 0;
s32 D_800D47EC = 0;
s32 D_800D480C = 0;
s32 D_800D481C = 0;
s32 D_800D4834 = 0;
s32 D_800D4854 = 0;
s32 D_800D4870 = 0;
s32 D_800D488C = 0;
s32 D_800CD520 = 0;
s32 D_80400008 = 0;
s32 D_800E4350 = 0;
s32 D_800E44D0 = 0;
s32 D_800E4650 = 0;











s16 D_800CD168 = 0;

// Shiftable Machine Dynamic Configuration variables (formerly D_800D0AC0, D_800D0AC4)
s32 gDynamicMachineConfigA = 0;
s32 gDynamicMachineConfigB = 0;

// Shiftable Game State Cycle variables (formerly D_800DCD04, D_800DCD08, D_800DCD0C, D_800DCCF8)
s32 gGameStateCycleA = 0;
s32 gGameStateCycleB = 0;
s32 gGameStateCycleC = 0;
s32 gGameStateControlMask = 0;

// Shiftable Track / Subsystem Lookup Table
s32 gTrackLookupTable[22] = {0};
struct UnkStruct_4 D_8010B7B0;
s32 gActiveVehicleIndex = 0;
s32 gInitLoopCounter = 0;
s32 gSystemStateFlag = 0;
s32 gSystemStateParamA = 0;
s32 gSystemStateParamB = 0;
s32 gLowMemStatus = 0;
u8 gLowMemInputPrev = 0;
u8 gLowMemInputCurrent = 0;
s8 gLowMemInputFlag = 0;



// Stubs for functions called by func_80068B20
void func_800A4BAC(void) {}
void func_800A4B54(void) {}
void func_800FC730(void) {}
void func_800FD184(s32 arg0) { (void)arg0; }
s32 __osMotorAccess(void* pfs, s32 flag) { (void)pfs; (void)flag; return 0; }

OSTime osGetTime(void) {
    return (OSTime)HAL_GetTimeMillis();
}

static u32 sMockFramebuffer[320 * 240];

void* osViGetCurrentFramebuffer(void) {
    return (void*)sMockFramebuffer;
}

// Stubs for functions called by linked_list.c are implemented in game_459A0.c

// Stubs for functions called by game_197D0.c
void func_8007F86C(void* a, void* b, void* c, void* d, void* e) {
    (void)a; (void)b; (void)c; (void)d; (void)e;
}

// Hook N64 debug text into PC UI overlay
void func_8007FB80(s32 x, s32 y, void* str) {
    DebugText_AddString(x, y, (const char*)str);
}
