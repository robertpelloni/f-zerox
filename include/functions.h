#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structs.h"

void AudioState_DispatchCommand(s32 cmd, u8 param);
void func_800B6910(s32 cmd, u8 param);

/* src/math_utils.c (game_2B20.c) - Math & Vectors */
s32 func_8006A918(void); // Math_Rand
s32 func_8006A9E0(f32 arg0); // Math_RoundF
void Vector_SetDual(struct UnkStruct_10* vecStruct, s32 compX, s32 compY, s32 compZ);
void func_8006AFC8(struct UnkStruct_10* arg0, s32 arg1, s32 arg2, s32 arg3); // SetVectors
void Vector_SetDual_Alias(struct UnkStruct_10* vecStruct, s32 compX, s32 compY, s32 compZ);
void func_8006AFE4(struct UnkStruct_10* arg0, s32 arg1, s32 arg2, s32 arg3); // SetVectors (Alias)
void Vector_SetTriple(struct UnkStruct_8* vecStruct, s32 compX, s32 compY, s32 compZ);
void func_8006B000(struct UnkStruct_8* arg0, s32 arg1, s32 arg2, s32 arg3); // SetVector8
void Vector_UpdateCacheA(void);
void func_8006B010(void);
void Vector_UpdateCacheB(void);
void func_8006B07C(void);
void Vector_ProcessStateA(void);
void func_8006B18C(void);
void Vector_ProcessStateB(void);
void func_8006B33C(void);

/* src/linked_list.c (game_446D0.c) - Linked List */
void func_800AAF9C(struct UnkStruct_1* arg0); // List_InitNode

/* src/game_73F0.c - Utils */
s32 func_8006D3F0(s32 arg0); // BoundsCheck_0_22

/* src/debug_text.c (game_197D0.c) - Debug & Framebuffer */
void DebugText_InitBuffers(void);
void func_8007F904(void);
void DebugText_DrawInteger(s32 val);
void DebugText_DrawScreenNotice(void);
void DebugText_DrawInfoGroupA(void);
void DebugText_DrawInfoGroupB(void);
void DebugText_DrawInfoGroupC(void);
void DebugText_DrawInfoGroupD(void);
void DebugText_DrawInfoGroupE(void);
void DebugText_DrawInfoGroupF(void);
void func_8007F94C(void); // UpdateFramebufferPointer
void func_8007FC68(s32 arg0); // DrawDebugInt
void func_8007FCB8(void);
void func_8007FCF4(void);
void func_8007FD58(void);
void func_8007FDD0(void);
void func_8007FE48(void);
void func_8007FE98(void);
void func_8007FED4(void);
void func_8007F86C(void* a, void* b, void* c, void* d, void* e);
void func_8007FB80(s32 x, s32 y, void* str);

/* src/audio_state.c (game_511D0.c) - Audio State */
void AudioState_SetParamA(s8 arg0);
void func_800BA2D0(s8 arg0);
void AudioState_SetParamB(s8 arg0);
void func_800BA2E0(s8 arg0);
void AudioState_Activate(void);
void func_800BAFF4(void);
void AudioState_TriggerPlay(void);
void func_800BB038(void);
void AudioState_SetAlternativeMode(void);
void func_800BB09C(void);
void AudioState_SetParamC(s8 arg0);
void func_800BB324(s8 arg0);
void AudioState_TriggerReset(void);
void func_800BB45C(void);
s32 AudioState_GetStatus(void);
s32 func_800B5FB0(void);

/* src/game_11CF0.c - Init & Memory */
void Memory_InitHeap(void);
void func_80077CF0(void);
void Memory_Free(void* ptr);
void myfree(void);
void Memory_AllocateBlockA(void);
void func_80077D50(void);
void Memory_AllocateBlockB(void);
void func_80078104(void);
void func_80079080(void); // InitLoop

/* src/game_1AE0.c - State */
void func_80067AE0(void); // RotateThreeGlobals

/* src/game_1F510.c - Machines */
void func_80088CAC(s32 arg0);
void func_8008D7E8(void); // InitAllMachines

/* src/game_194E0.c - State Setter */
void func_8007F4E0(s32 arg0, s32 arg1);

/* src/game_4FFB0.c - Low Mem */
void func_800B6A18(void);

/* src/audio_util.c (game_14440.c) - Audio Util */
void func_8007E0AC(s32 arg0);
s8 func_8007E10C(s32 arg0);

/* src/math_utils.c - Decompiled Logic */
void MachineState_InitAll(void);
void func_80069700(void);
void MachineState_InitSpecial(void);
void func_80069820(void);
void MachineState_UpdatePhysics(void);
void func_80069D44(void);
void MachineState_UpdateCollisions(void);
void func_80069ED0(void);
void MachineState_UpdateVisuals(void);
void func_80069F5C(void);
void MachineState_UpdateAI(void);
void func_8006A00C(void);
void MachineState_UpdateControls(void);
void func_8006A3AC(void);
void MachineState_UpdateEffects(void);
void func_8006A6E4(void);
void Math_InitTrigTables(void);
void func_8006AA38(void);
void Vector_InitTables(void);
void func_8006ADE4(void);
void System_Tick(void);
void func_80068BC0(void);
void System_IdleWait(void);
void func_80068DCC(void);
void System_PollEvents(void);
void func_80068F04(void);
void func_80068B20(void);
// Dependencies of func_80068B20
void Audio_ColdBootInit(void);
void func_8008DB98(void);
void Audio_AuxInit(void);
void func_800A4BAC(void);
void Audio_WarmBootInit(void);
void func_8008DA68(void);
void Audio_Shutdown(void);
void func_800A4B54(void);
void Machine_SystemInit(void);
void func_80085510(void);
void Gfx_SystemInit(void);
void func_800FC730(void);
void System_InitMemoryState(void);
void func_8007F500(void);
void Input_SystemInit(void);
void func_80076848(void);
void Timing_SystemInit(void);
void func_8007D9D0(void);
void State_ProcessResult(s32 result);
void func_800FD184(s32 arg0);

/* src/game_73F0.c - System Config & Track State */
void SystemConfig_Reset(void);
void func_800742D0(void);
void Track_ResetState(void);
void func_80072D00(void);
void Track_InitContext(s32 contextId);
void func_80072ECC(s32 arg0);

/* src/game_459A0.c - Variable Setters */
void func_800AF820(void);
void Machine_SetConfigA(s32 val);
void func_800B0784(s32 arg0);
void Machine_SetConfigB(s32 val);
void func_800B0790(s32 arg0);
void StateStruct_Reset(struct UnkStruct_5* state);
void func_800B2B50(struct UnkStruct_5* arg0);

#endif
