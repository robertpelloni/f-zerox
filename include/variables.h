#ifndef VARIABLES_H
#define VARIABLES_H

// Shiftable System Memory State Value (formerly D_800E42CC)
extern s16 gSystemMemoryStateValue;
#define D_800E42CC gSystemMemoryStateValue
// Shiftable Low Memory Globals (formerly D_80025D78, D_8002B346, D_8002B347, D_8002B348)
extern s32 gLowMemStatus;
extern u8 gLowMemInputPrev;
extern u8 gLowMemInputCurrent;
extern s8 gLowMemInputFlag;
#define D_80025D78 gLowMemStatus
#define D_8002B346 gLowMemInputPrev
#define D_8002B347 gLowMemInputCurrent
#define D_8002B348 gLowMemInputFlag

// Shiftable Global State 8002AA18 (formerly D_8002AA18)
extern s32 gGlobalState_8002AA18;
#define D_8002AA18 gGlobalState_8002AA18
// Shiftable System State Flag & Params (formerly D_800CD510, D_800E4264, D_800E4260)
extern s32 gSystemStateFlag;
extern s32 gSystemStateParamA;
extern s32 gSystemStateParamB;
#define D_800CD510 gSystemStateFlag
#define D_800E4264 gSystemStateParamA
#define D_800E4260 gSystemStateParamB

extern s16 gDebugMode;
#include <stdint.h>
extern uintptr_t gStateHandlers[];
// Shiftable Debug Text Buffers (formerly D_800CD520, D_80400008, D_800E4350, D_800E44D0, D_800E4650)
extern s32 gDebugBufferState;
extern s32 gDebugBufferAux;
extern s32 gDebugBufferA;
extern s32 gDebugBufferB;
extern s32 gDebugBufferC;
#define D_800CD520 gDebugBufferState
#define D_80400008 gDebugBufferAux
#define D_800E4350 gDebugBufferA
#define D_800E44D0 gDebugBufferB
#define D_800E4650 gDebugBufferC
// Shiftable Machine Dynamic Configuration variables (formerly D_800D0AC0, D_800D0AC4)
extern s32 gDynamicMachineConfigA;
extern s32 gDynamicMachineConfigB;
#define D_800D0AC0 gDynamicMachineConfigA
#define D_800D0AC4 gDynamicMachineConfigB
extern s32 D_800D1C18;
// Shiftable Debug Format and Label Strings (formerly D_800D46C0, D_800D46C8, D_800D46D8)
extern const char gDebugFormatInteger[];
extern const char gDebugLabelInteger[];
extern const char gDebugScreenNotice[];
#define D_800D46C0 gDebugFormatInteger
#define D_800D46C8 gDebugLabelInteger
#define D_800D46D8 gDebugScreenNotice
extern const char gDebugInfoGroupA1[];
extern const char gDebugInfoGroupA2[];
extern const char gDebugInfoGroupA3[];
#define D_800D46FC gDebugInfoGroupA1
#define D_800D471C gDebugInfoGroupA2
#define D_800D473C gDebugInfoGroupA3
extern const char gDebugInfoGroupB1[];
extern const char gDebugInfoGroupB2[];
extern const char gDebugInfoGroupB3[];
extern const char gDebugInfoGroupB4[];
#define D_800D474C gDebugInfoGroupB1
#define D_800D476C gDebugInfoGroupB2
#define D_800D478C gDebugInfoGroupB3
#define D_800D479C gDebugInfoGroupB4
extern const char gDebugInfoGroupC1[];
extern const char gDebugInfoGroupC2[];
extern const char gDebugInfoGroupC3[];
extern const char gDebugInfoGroupC4[];
#define D_800D47B4 gDebugInfoGroupC1
#define D_800D47D0 gDebugInfoGroupC2
#define D_800D47EC gDebugInfoGroupC3
#define D_800D480C gDebugInfoGroupC4
extern const char gDebugInfoGroupD1[];
extern const char gDebugInfoGroupD2[];
extern const char gDebugInfoGroupE1[];
extern const char gDebugInfoGroupF1[];
extern const char gDebugInfoGroupF2[];
#define D_800D481C gDebugInfoGroupD1
#define D_800D4834 gDebugInfoGroupD2
#define D_800D4854 gDebugInfoGroupE1
#define D_800D4870 gDebugInfoGroupF1
#define D_800D488C gDebugInfoGroupF2
extern s16 gSystemFullyInitialized;
// Shiftable Game State Cycle variables (formerly D_800DCD04, D_800DCD08, D_800DCD0C, D_800DCCF8)
extern s32 gGameStateCycleA;
extern s32 gGameStateCycleB;
extern s32 gGameStateCycleC;
extern s32 gGameStateControlMask;

#define D_800DCD04 gGameStateCycleA
#define D_800DCD08 gGameStateCycleB
#define D_800DCD0C gGameStateCycleC
#define D_800DCCF8 gGameStateControlMask
extern s32 gCurrentStateIndex;
extern s32 gUnknownStateVar;
extern s32 gSystemInitialized;
extern u8 gMachineStates[592];
extern u8 gSpecialMachineState[256];
// Shiftable Current Framebuffer pointer (formerly D_800E4348)
extern void* gCurrentFramebuffer;
#define D_800E4348 gCurrentFramebuffer
// Shiftable Active Vehicle Index (formerly D_800E5E84)
extern s32 gActiveVehicleIndex;
#define D_800E5E84 gActiveVehicleIndex
// Shiftable N64 PI Cartridge Hardware Registers (formerly D_A4500000, D_A4500004, D_A450000C)
extern s32 gCartridgeRegister0;
extern s32 gCartridgeRegister4;
extern s32 gCartridgeRegisterC;
#define D_A4500000 gCartridgeRegister0
#define D_A4500004 gCartridgeRegister4
#define D_A450000C gCartridgeRegisterC
// Shiftable Track Context & State Tracking
extern s32 gTrackContextId;
extern s32 gTrackResetCount;

// Shiftable Dynamic Assets Mapped Flag
extern s32 gSystemAssetsMapped;

// Shiftable Vector Cache Update Counters
extern s32 gVectorCacheACount;
extern s32 gVectorCacheBCount;
extern s32 gVectorProcessStateACount;
extern s32 gVectorProcessStateBCount;

// Shiftable Memory Subsystem Telemetry Counters
extern s32 gMemoryHeapInitCount;
extern s32 gMemoryFreeCount;
// Shiftable Audio State Variables
extern s8 gAudioStateParamA;
extern s8 gAudioStateParamB;
extern u8 gAudioStateActiveFlag;
extern s8 gAudioStatePlayTrigger;
extern s8 gAudioStateParamC;
extern s8 gAudioStateResetFlag;
extern s32 gAudioStateStatus;
extern s32 gAudioLastCommand;
extern u8 gAudioLastParam;

#define D_800D1A3C gAudioStateParamA
#define D_800D1A20 gAudioStateParamB
#define D_800D1A48 gAudioStateActiveFlag
#define D_800D19E0 gAudioStatePlayTrigger
#define D_800D1A18 gAudioStateParamC
#define D_800D1A0C gAudioStateResetFlag
#define D_800D1C18 gAudioStateStatus

// Shiftable Global State 800CD168 (formerly D_800CD168)
extern s16 gGlobalState_800CD168;
#define D_800CD168 gGlobalState_800CD168

// Shiftable Sound Effect Bank Map (formerly D_800D4690, 256 bytes)
extern s8 gSoundEffectBankMap[256];
#define D_800D4690 gSoundEffectBankMap[0]

// Shiftable Global State 800E4175 (formerly D_800E4175)
extern s8 gGlobalState_800E4175;
#define D_800E4175 gGlobalState_800E4175

// Shiftable System Configuration / State Struct (formerly D_8010B7B0)
extern struct UnkStruct_4 gSystemConfigState;
#define D_8010B7B0 gSystemConfigState
extern s16 gStateMachineActive;

// Shiftable Global State 800D16D0 (formerly D_800D16D0)
extern u8 gGlobalState_800D16D0;
#define D_800D16D0 gGlobalState_800D16D0

// Shiftable Global Ptr 800F8510 (formerly D_800F8510)
extern void* gGlobalPtr_800F8510;
#define D_800F8510 gGlobalPtr_800F8510
// Shiftable Track / Subsystem Lookup Table (22 entries)
extern s32 gTrackLookupTable[22];
#define D_800E33C0 gTrackLookupTable
// Shiftable Init Loop Counter (formerly D_800E4068)
extern s32 gInitLoopCounter;
#define D_800E4068 gInitLoopCounter


typedef struct {
    s32 state1;
    s32 state2;
} RandState;
extern RandState gRandState;

typedef struct {
    s32 timeValue1;
    s32 timeValue2;
} TimeState;
extern TimeState gTimeState;

// Subsystem Initialization Tracking Flags (Shiftable & Introspectable)
extern s32 gMachineSystemInitialized;
extern s32 gAudioColdBootCount;
extern s32 gAudioWarmBootCount;
extern s32 gInputSystemInitialized;
extern s32 gTimingSystemInitialized;
extern s32 gAudioAuxInitialized;
extern s32 gAudioShutdownCount;
extern s32 gGfxSystemInitialized;
extern s32 gLastStateResult;
extern s32 gSystemTickCount;
extern s32 gSystemIdleWaitCount;
extern s32 gSystemPollEventsCount;
extern s32 gMachineStateInitAllCount;
extern s32 gMachineStateInitSpecialCount;
extern s32 gMachineStateUpdatePhysicsCount;
extern s32 gMachineStateUpdateCollisionsCount;
extern s32 gMachineStateUpdateVisualsCount;
extern s32 gMachineStateUpdateAICount;
extern s32 gMachineStateUpdateControlsCount;
extern s32 gMachineStateUpdateEffectsCount;
extern s32 gMathTrigTablesInitCount;
extern s32 gVectorTablesInitCount;

#endif
