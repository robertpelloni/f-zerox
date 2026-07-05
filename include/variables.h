#ifndef VARIABLES_H
#define VARIABLES_H

extern s16 D_800E42CC;
extern s32 D_80025D78;
extern s32 D_8002AA18;
extern s32 D_800CD510;
extern s16 gDebugMode;
extern s32 gStateHandlers[];
extern s32 D_800CD520;
extern s32 D_800D0AC0;
extern s32 D_800D0AC4;
extern s32 D_800D1C18;
extern s32 D_800D46C0; // gDebugPrintFormat (Used in game_197D0.c)
extern s32 D_800D46C8;
extern s32 D_800D46D8;
extern s32 D_800D46FC;
extern s32 D_800D471C;
extern s32 D_800D473C;
extern s32 D_800D474C;
extern s32 D_800D476C;
extern s32 D_800D478C;
extern s32 D_800D479C;
extern s32 D_800D47B4;
extern s32 D_800D47D0;
extern s32 D_800D47EC;
extern s32 D_800D480C;
extern s32 D_800D481C;
extern s32 D_800D4834;
extern s32 D_800D4854;
extern s32 D_800D4870;
extern s32 D_800D488C;
extern s16 gSystemFullyInitialized;
extern s32 D_800DCCF8;
extern s32 D_800DCD04;
extern s32 D_800DCD08;
extern s32 D_800DCD0C;
extern s32 gCurrentStateIndex;
extern s32 gUnknownStateVar;
extern s32 gSystemInitialized;
extern u8 gMachineStates[592];
extern u8 gSpecialMachineState[256];
extern s32 D_800E4260;
extern s32 D_800E4264;
extern s32 D_800E4348; // gCurrentFramebuffer (Updated in game_197D0.c)
extern s32 D_800E4350;
extern s32 D_800E44D0;
extern s32 D_800E4650;
extern s32 D_800E5E84;
extern s32 D_80400008;
extern s32 D_A4500000;
extern s32 D_A4500004;
extern s32 D_A450000C;
extern s8 D_8002B348;
extern s8 D_800D19E0;
extern s8 D_800D1A0C;
extern s8 D_800D1A18;
extern s8 D_800D1A20;
extern s8 D_800D1A3C;
extern s16 D_800CD168;
extern s8 D_800D4690;
extern s8 D_800E4175;
extern struct UnkStruct_4 D_8010B7B0;
extern s16 gStateMachineActive;
extern u8 D_8002B346;
extern u8 D_8002B347;
extern u8 D_800D16D0;
extern u8 D_800D1A48;
extern void* D_800F8510;
extern s32 D_800E33C0[];
extern s32 D_800E4068;


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

#endif
