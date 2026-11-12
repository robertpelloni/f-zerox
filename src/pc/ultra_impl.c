#include "pc/ultra64.h"
#include "pc/hal.h"
#include "pc/debug_text_hook.h"
#include "structs.h"
#include <stdio.h>

// Mock Global Variables (usually defined in ld script or asm)
void* gCurrentFramebuffer = NULL;
const char gDebugFormatInteger[] = "%d";
const char gDebugLabelInteger[] = "INT:";
const char gDebugScreenNotice[] = "NOTICE";
const char gDebugInfoGroupA1[] = "INFO_A1";
const char gDebugInfoGroupA2[] = "INFO_A2";
const char gDebugInfoGroupA3[] = "INFO_A3";
const char gDebugInfoGroupB1[] = "INFO_B1";
const char gDebugInfoGroupB2[] = "INFO_B2";
const char gDebugInfoGroupB3[] = "INFO_B3";
const char gDebugInfoGroupB4[] = "INFO_B4";
const char gDebugInfoGroupC1[] = "INFO_C1";
const char gDebugInfoGroupC2[] = "INFO_C2";
const char gDebugInfoGroupC3[] = "INFO_C3";
const char gDebugInfoGroupC4[] = "INFO_C4";
const char gDebugInfoGroupD1[] = "INFO_D1";
const char gDebugInfoGroupD2[] = "INFO_D2";
const char gDebugInfoGroupE1[] = "INFO_E1";
const char gDebugInfoGroupF1[] = "INFO_F1";
const char gDebugInfoGroupF2[] = "INFO_F2";
s32 gDebugBufferState = 0;
s32 gDebugBufferAux = 0;
s32 gDebugBufferA = 0;
s32 gDebugBufferB = 0;
s32 gDebugBufferC = 0;











s16 gGlobalState_800CD168 = 0;
s8 gGlobalState_800E4175 = 0;
u8 gGlobalState_800D16D0 = 0;
void* gGlobalPtr_800F8510 = NULL;
s32 gGlobalState_8002AA18 = 0;

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
struct UnkStruct_4 gSystemConfigState;
s32 gActiveVehicleIndex = 0;
s32 gInitLoopCounter = 0;
s32 gCartridgeRegister0 = 0;
s32 gCartridgeRegister4 = 0;
s32 gCartridgeRegisterC = 0;
s32 gTrackContextId = 0;
s32 gTrackResetCount = 0;
s32 gSystemAssetsMapped = 0;
s32 gVectorCacheACount = 0;
s32 gVectorCacheBCount = 0;
s32 gVectorProcessStateACount = 0;
s32 gVectorProcessStateBCount = 0;
s32 gMemoryHeapInitCount = 0;
s32 gMemoryFreeCount = 0;
s32 gMemoryAllocBlockACount = 0;
s32 gMemoryAllocBlockBCount = 0;
s32 gMemoryCompactHeapCount = 0;
s32 gMemoryVerifyIntegrityCount = 0;
s32 gMemoryPoolAInitCount = 0;
s32 gMemoryPoolBInitCount = 0;
s32 gSystemStateFlag = 0;
s32 gSystemStateParamA = 0;
s32 gSystemStateParamB = 0;
s32 gLowMemStatus = 0;
u8 gLowMemInputPrev = 0;
u8 gLowMemInputCurrent = 0;
s8 gLowMemInputFlag = 0;
s16 gSystemMemoryStateValue = 0;

// Subsystem Initialization Tracking Flags
s32 gMachineSystemInitialized = 0;
s32 gAudioColdBootCount = 0;
s32 gAudioWarmBootCount = 0;
s32 gInputSystemInitialized = 0;
s32 gTimingSystemInitialized = 0;
s32 gAudioAuxInitialized = 0;
s32 gAudioShutdownCount = 0;
s32 gGfxSystemInitialized = 0;
s32 gLastStateResult = 0;
s32 gSystemTickCount = 0;
s32 gSystemIdleWaitCount = 0;
s32 gSystemPollEventsCount = 0;
s32 gMachineStateInitAllCount = 0;
s32 gMachineStateInitSpecialCount = 0;
s32 gMachineStateUpdatePhysicsCount = 0;
s32 gMachineStateUpdateCollisionsCount = 0;
s32 gMachineStateUpdateVisualsCount = 0;
s32 gMachineStateUpdateAICount = 0;
s32 gMachineStateUpdateControlsCount = 0;
s32 gMachineStateUpdateEffectsCount = 0;
s32 gMathTrigTablesInitCount = 0;
s32 gVectorTablesInitCount = 0;

/**
 * @brief Initializes the graphics subsystem.
 *
 * Marks graphics system state as initialized. Formerly func_800FC730.
 */
void Gfx_SystemInit(void) {
    gGfxSystemInitialized = 1;
}

void func_800FC730(void) {
    Gfx_SystemInit();
}

/**
 * @brief Processes the result of a state handler execution.
 *
 * Stores the last state result. Formerly func_800FD184.
 */
void State_ProcessResult(s32 result) {
    gLastStateResult = result;
}

void func_800FD184(s32 arg0) {
    State_ProcessResult(arg0);
}

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
