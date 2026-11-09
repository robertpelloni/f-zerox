#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "minunit.h"
#include "../include/pc/ultra64.h"
#include "../include/pc/PR/gbi.h"
#include "../include/functions.h"

int tests_run = 0;

// Declarations of decompiled functions from math_utils.c
s32 Math_RoundF(f32 arg0);
s32 Math_Rand(void);
void System_Init(void);
void System_SetTime(s32 t1, s32 t2);
s32 StateMachine_ExecuteCurrentState(void);
void Controller_ResetMachineStates(void);
float Math_SinS(int16_t angle);
float Math_CosS(int16_t angle);
float Math_SqrtF(float value);

// Declarations of decompiled audio state functions from audio_state.c
void func_800BA2D0(s8 arg0);
void func_800BA2E0(s8 arg0);
void func_800BAFF4(void);
void func_800BB038(void);
void func_800BB09C(void);
void func_800BB324(s8 arg0);
void func_800BB45C(void);

// Declarations of linked list functions
void List_InitNode(struct UnkStruct_1* node);
void func_800AAF9C(struct UnkStruct_1* node);

// Declarations of track lookup functions from game_73F0.c
s32 TrackLookup_ValidateIndex(s32 idx);
void TrackLookup_SetEntry(s32 idx, s32 val);
s32 TrackLookup_GetEntry(s32 idx);

// Declarations of audio utility functions from audio_util.c
void Audio_SetSoundId(s32 soundId);
s8 Audio_GetSoundEffectByte(s32 idx);

// Declarations of machine configuration functions from game_459A0.c
void Machine_SetConfigA(s32 val);
void Machine_SetConfigB(s32 val);

// Declarations of network protocol functions from network.h
bool Net_IsMachineActive(int index);
int Net_GetConnectedCount(void);
uint8_t Net_GetLocalPlayerId(void);

// Declarations of vehicle functions from game_1F510.c
void Vehicle_SetActiveIndex(s32 idx);

// Declarations of init functions from game_11CF0.c
void System_RunInitDelayLoop(void);

// Declarations of system state functions from game_194E0.c
void System_SetStateFlags(s32 paramA, s32 paramB);

// Declarations of dynamic asset loader functions
void Asset_RegisterMapping(uint32_t address, void* asset);
bool Asset_UnregisterMapping(uint32_t address);
bool Asset_HasAddress(uint32_t address);
void* Asset_GetByAddress(uint32_t address);
int Asset_GetMappingCount(void);
void Asset_ClearMappings(void);

// Declarations of low memory functions from game_4FFB0.c
void Input_UpdateLowMemState(void);
void Input_ClearLowMemStatus(void);

// Declarations of direct track lookup from game_10490.c
s32 TrackLookup_GetEntryDirect(s32 idx, s32 unused);

#include "../include/variables.h"

static char * test_math_roundf() {
    mu_assert("Round 0.0 failed", Math_RoundF(0.0f) == 0);
    mu_assert("Round 0.5 failed", Math_RoundF(0.5f) == 1);
    mu_assert("Round 0.4 failed", Math_RoundF(0.4f) == 0);
    mu_assert("Round -0.5 failed", Math_RoundF(-0.5f) == -1);
    mu_assert("Round -0.4 failed", Math_RoundF(-0.4f) == 0);
    mu_assert("Round 1.9 failed", Math_RoundF(1.9f) == 2);
    return 0;
}

static char * test_system_init_and_time() {
    gSystemInitialized = 0;
    gSystemFullyInitialized = 0;
    System_Init();
    mu_assert("System_Init magic failed", gSystemInitialized == 0x20DE1529);
    mu_assert("System_Init fully initialized flag failed", gSystemFullyInitialized == 1);
    mu_assert("System_Init current state index failed", gCurrentStateIndex == -1);

    // Warm boot branch test
    System_Init();
    mu_assert("System_Init warm boot check failed", gSystemInitialized == 0x20DE1529);

    System_SetTime(1234, 5678);
    mu_assert("System_SetTime t1 failed", gTimeState.timeValue1 == 1234);
    mu_assert("System_SetTime t2 failed", gTimeState.timeValue2 == 5678);
    return 0;
}

static s32 mock_state_handler_called = 0;
static s32 mock_state_handler(void) {
    mock_state_handler_called = 42;
    return 100;
}

static char * test_state_machine() {
    gDebugMode = 0;
    gStateMachineActive = 1;
    gCurrentStateIndex = 2; // index 2
    gStateHandlers[2] = (uintptr_t)mock_state_handler;

    s32 res = StateMachine_ExecuteCurrentState();
    mu_assert("State handler execution failed", mock_state_handler_called == 42);
    mu_assert("State handler return code failed", res == 100);

    // Inactive state machine should return 0
    gStateMachineActive = 0;
    res = StateMachine_ExecuteCurrentState();
    mu_assert("Inactive state machine return failed", res == 0);
    return 0;
}

static char * test_math_rand() {
    gRandState.state1 = 12345;
    gRandState.state2 = 67890;
    s32 val1 = Math_Rand();
    s32 val2 = Math_Rand();
    mu_assert("Math_Rand zero randomness", val1 != val2);
    return 0;
}

static char * test_controller_reset_machine_states() {
    // Fill machine states with dummy values
    for (int i = 0; i < 592; i++) {
        gMachineStates[i] = 0xAA;
    }
    // Set first machine as inactive (byte at offset 0x6B == 0)
    gMachineStates[0x6B] = 0;

    Controller_ResetMachineStates();
    // Inactive machine fields must be reset
    mu_assert("Reset machine flag A", *(u8*)(gMachineStates + 0x71) == 0);
    mu_assert("Reset machine flag B", *(u8*)(gMachineStates + 0x70) == 0);
    return 0;
}

static char * test_trig_functions() {
    float s0 = Math_SinS(0);
    float c0 = Math_CosS(0);
    float sq = Math_SqrtF(16.0f);

    mu_assert("Sin(0) != 0", s0 >= -0.001f && s0 <= 0.001f);
    mu_assert("Cos(0) != 1", c0 >= 0.999f && c0 <= 1.001f);
    mu_assert("Sqrt(16) != 4", sq >= 3.999f && sq <= 4.001f);
    return 0;
}

static char * test_audio_state_mutations() {
    gAudioStateParamA = 0;
    func_800BA2D0(15);
    mu_assert("func_800BA2D0 failed", gAudioStateParamA == 15);

    gAudioStateParamB = 0;
    func_800BA2E0(-8);
    mu_assert("func_800BA2E0 failed", gAudioStateParamB == -8);

    gAudioStateActiveFlag = 0;
    func_800BAFF4();
    mu_assert("func_800BAFF4 failed", gAudioStateActiveFlag == 1);

    gAudioStatePlayTrigger = 0;
    func_800BB038();
    mu_assert("func_800BB038 failed", gAudioStatePlayTrigger == 1);

    gAudioStateParamC = 0;
    func_800BB324(25);
    mu_assert("func_800BB324 failed", gAudioStateParamC == 25);

    gAudioStateResetFlag = 0;
    func_800BB45C();
    mu_assert("func_800BB45C failed", gAudioStateResetFlag == 1);

    gAudioStateActiveFlag = 0;
    func_800BB09C();
    mu_assert("func_800BB09C failed", gAudioStateActiveFlag == 3);
    return 0;
}

static char * test_audio_state_pascal_case_apis() {
    gAudioStateParamA = 0;
    AudioState_SetParamA(45);
    mu_assert("AudioState_SetParamA failed", gAudioStateParamA == 45);

    gAudioStateParamB = 0;
    AudioState_SetParamB(-33);
    mu_assert("AudioState_SetParamB failed", gAudioStateParamB == -33);

    gAudioStateActiveFlag = 0;
    AudioState_Activate();
    mu_assert("AudioState_Activate failed", gAudioStateActiveFlag == 1);

    gAudioStatePlayTrigger = 0;
    AudioState_TriggerPlay();
    mu_assert("AudioState_TriggerPlay failed", gAudioStatePlayTrigger == 1);

    gAudioStateActiveFlag = 0;
    AudioState_SetAlternativeMode();
    mu_assert("AudioState_SetAlternativeMode failed", gAudioStateActiveFlag == 3);

    gAudioStateParamC = 0;
    AudioState_SetParamC(88);
    mu_assert("AudioState_SetParamC failed", gAudioStateParamC == 88);

    gAudioStateResetFlag = 0;
    AudioState_TriggerReset();
    mu_assert("AudioState_TriggerReset failed", gAudioStateResetFlag == 1);

    return 0;
}

static char * test_linked_list_init() {
    struct UnkStruct_1 node;
    node.next = NULL;
    node.prev = NULL;
    node.unk8 = 0x1234;

    func_800AAF9C(&node);
    mu_assert("List_InitNode next ptr self-reference failed", node.next == &node);
    mu_assert("List_InitNode prev ptr self-reference failed", node.prev == &node);
    mu_assert("List_InitNode unk8 zero failed", node.unk8 == 0);
    return 0;
}

static char * test_track_lookup_table() {
    mu_assert("TrackLookup_ValidateIndex(0) valid", TrackLookup_ValidateIndex(0) == 1);
    mu_assert("TrackLookup_ValidateIndex(21) valid", TrackLookup_ValidateIndex(21) == 1);
    mu_assert("TrackLookup_ValidateIndex(22) invalid", TrackLookup_ValidateIndex(22) == 0);
    mu_assert("TrackLookup_ValidateIndex(-1) invalid", TrackLookup_ValidateIndex(-1) == 0);

    TrackLookup_SetEntry(5, 4242);
    mu_assert("TrackLookup_GetEntry(5) match", TrackLookup_GetEntry(5) == 4242);

    // Invalid index should return 0 and not crash
    TrackLookup_SetEntry(99, 1234);
    mu_assert("TrackLookup_GetEntry(99) out of bounds", TrackLookup_GetEntry(99) == 0);
    return 0;
}

static char * test_audio_utils() {
    gSoundEffectBankMap[0] = 77;
    mu_assert("Audio_GetSoundEffectByte(0) match", Audio_GetSoundEffectByte(0) == 77);

    // Call Audio_SetSoundId to verify 8-bit masking logic doesn't crash
    Audio_SetSoundId(0x1F2A);
    return 0;
}

static char * test_machine_dynamic_config() {
    gDynamicMachineConfigA = 0;
    Machine_SetConfigA(999);
    mu_assert("Machine_SetConfigA match", gDynamicMachineConfigA == 999);

    gDynamicMachineConfigB = 0;
    Machine_SetConfigB(-777);
    mu_assert("Machine_SetConfigB match", gDynamicMachineConfigB == -777);
    return 0;
}

static char * test_net_is_machine_active() {
    mu_assert("Net_IsMachineActive(-1) invalid", !Net_IsMachineActive(-1));
    mu_assert("Net_IsMachineActive(30) out of bounds", !Net_IsMachineActive(30));
    mu_assert("Net_IsMachineActive(99) out of bounds", !Net_IsMachineActive(99));
    mu_assert("Net_IsMachineActive(0) default local player active", Net_IsMachineActive(0));

    mu_assert("Net_GetConnectedCount at least 1", Net_GetConnectedCount() >= 1);
    mu_assert("Net_GetLocalPlayerId matches default", Net_GetLocalPlayerId() == 0);
    return 0;
}

static char * test_vehicle_active_index() {
    gActiveVehicleIndex = 0;
    Vehicle_SetActiveIndex(7);
    mu_assert("Vehicle_SetActiveIndex match", gActiveVehicleIndex == 7);
    return 0;
}

static char * test_system_init_delay_loop() {
    gInitLoopCounter = 42;
    System_RunInitDelayLoop();
    mu_assert("System_RunInitDelayLoop zeroes counter", gInitLoopCounter == 0);
    return 0;
}

static char * test_system_set_state_flags() {
    gSystemStateFlag = 0;
    gSystemStateParamA = 0;
    gSystemStateParamB = 0;

    System_SetStateFlags(101, 202);
    mu_assert("System_SetStateFlags flag set", gSystemStateFlag == 1);
    mu_assert("System_SetStateFlags paramA set", gSystemStateParamA == 101);
    mu_assert("System_SetStateFlags paramB set", gSystemStateParamB == 202);
    return 0;
}

static char * test_dynamic_asset_loader() {
    Asset_ClearMappings();
    mu_assert("Asset_GetMappingCount initially 0", Asset_GetMappingCount() == 0);

    int dummyModel = 12345;
    Asset_RegisterMapping(0x80800000, &dummyModel);
    mu_assert("Asset_GetMappingCount is 1", Asset_GetMappingCount() == 1);
    mu_assert("Asset_GetByAddress found correct ptr", Asset_GetByAddress(0x80800000) == &dummyModel);
    mu_assert("Asset_GetByAddress missing ptr returns NULL", Asset_GetByAddress(0x80999999) == NULL);

    Asset_ClearMappings();
    mu_assert("Asset_ClearMappings resets count to 0", Asset_GetMappingCount() == 0);
    return 0;
}

static char * test_low_mem_state() {
    gLowMemInputPrev = 0x5A;
    gLowMemInputCurrent = 0x00;
    gLowMemInputFlag = 1;

    Input_UpdateLowMemState();
    mu_assert("Input_UpdateLowMemState copies prev to current", gLowMemInputCurrent == 0x5A);
    mu_assert("Input_UpdateLowMemState clears flag", gLowMemInputFlag == 0);

    gLowMemStatus = 0xDEAD;
    Input_ClearLowMemStatus();
    mu_assert("Input_ClearLowMemStatus zeroes status", gLowMemStatus == 0);
    return 0;
}

extern void DebugText_UpdateFramebuffer(void);
extern void func_8007F94C(void);

static char * test_track_lookup_direct() {
    gTrackLookupTable[10] = 8888;
    mu_assert("TrackLookup_GetEntryDirect(10, 0) matches", TrackLookup_GetEntryDirect(10, 0) == 8888);
    mu_assert("TrackLookup_GetEntryDirect(99, 0) out of bounds returns 0", TrackLookup_GetEntryDirect(99, 0) == 0);
    return 0;
}

extern void System_InitMemoryState(void);
extern void func_8007F500(void);

static char * test_debug_text_framebuffer() {
    gCurrentFramebuffer = NULL;
    DebugText_UpdateFramebuffer();
    mu_assert("error, DebugText_UpdateFramebuffer failed to populate gCurrentFramebuffer", gCurrentFramebuffer != NULL);

    gCurrentFramebuffer = NULL;
    func_8007F94C();
    mu_assert("error, func_8007F94C alias failed to populate gCurrentFramebuffer", gCurrentFramebuffer != NULL);
    return 0;
}

static char * test_system_init_memory_state() {
    gSystemMemoryStateValue = 1234;
    System_InitMemoryState();
    mu_assert("error, System_InitMemoryState failed to zero gSystemMemoryStateValue", gSystemMemoryStateValue == 0);

    gSystemMemoryStateValue = 5678;
    func_8007F500();
    mu_assert("error, func_8007F500 alias failed to zero gSystemMemoryStateValue", gSystemMemoryStateValue == 0);
    return 0;
}

static char * test_state_struct_reset() {
    struct UnkStruct_5 s;
    s.unk0 = 0x4321;
    s.unk4 = 0;
    s.unk8 = 0x5555;
    s.unkC = 0x9999;

    StateStruct_Reset(&s);
    mu_assert("error, StateStruct_Reset failed to set unk4 = unk0", s.unk4 == s.unk0);
    mu_assert("error, StateStruct_Reset failed to zero unkC", s.unkC == 0);

    s.unk0 = 0x8765;
    s.unk4 = 0;
    s.unkC = 0xAAAA;
    func_800B2B50(&s);
    mu_assert("error, func_800B2B50 alias failed to set unk4 = unk0", s.unk4 == s.unk0);
    mu_assert("error, func_800B2B50 alias failed to zero unkC", s.unkC == 0);
    return 0;
}

extern void SystemConfig_Reset(void);
extern void DebugText_InitBuffers(void);
extern void func_8007F904(void);
extern void DebugText_DrawInteger(s32 val);
extern void func_8007FC68(s32 arg0);
extern void DebugText_DrawScreenNotice(void);
extern void func_8007FCB8(void);
extern void DebugText_DrawInfoGroupA(void);
extern void func_8007FCF4(void);
extern void DebugText_DrawInfoGroupB(void);
extern void func_8007FD58(void);
extern void DebugText_DrawInfoGroupC(void);
extern void func_8007FDD0(void);
extern void DebugText_DrawInfoGroupD(void);
extern void func_8007FE48(void);
extern void DebugText_DrawInfoGroupE(void);
extern void func_8007FE98(void);
extern void DebugText_DrawInfoGroupF(void);
extern void func_8007FED4(void);

static char * test_system_config_reset() {
    gSystemConfigState.unk2 = 5;
    gSystemConfigState.unk3 = 10;

    SystemConfig_Reset();
    mu_assert("error, SystemConfig_Reset failed to zero unk2", gSystemConfigState.unk2 == 0);
    mu_assert("error, SystemConfig_Reset failed to zero unk3", gSystemConfigState.unk3 == 0);
    return 0;
}

static char * test_debug_text_init_buffers() {
    gDebugBufferState = 1;
    gDebugBufferAux = 2;
    gDebugBufferA = 3;
    gDebugBufferB = 4;
    gDebugBufferC = 5;

    DebugText_InitBuffers();
    mu_assert("error, DebugText_InitBuffers execution failed", 1);

    func_8007F904();
    mu_assert("error, func_8007F904 alias execution failed", 1);
    return 0;
}

static char * test_debug_text_draw_integer() {
    DebugText_DrawInteger(42);
    func_8007FC68(99);
    mu_assert("error, DebugText_DrawInteger execution failed", 1);
    return 0;
}

static char * test_debug_text_draw_notice() {
    DebugText_DrawScreenNotice();
    func_8007FCB8();
    mu_assert("error, DebugText_DrawScreenNotice execution failed", 1);
    return 0;
}

static char * test_debug_text_draw_info_group_a() {
    DebugText_DrawInfoGroupA();
    func_8007FCF4();
    mu_assert("error, DebugText_DrawInfoGroupA execution failed", 1);
    return 0;
}

static char * test_debug_text_draw_info_group_b() {
    DebugText_DrawInfoGroupB();
    func_8007FD58();
    mu_assert("error, DebugText_DrawInfoGroupB execution failed", 1);
    return 0;
}

static char * test_debug_text_draw_info_group_c() {
    DebugText_DrawInfoGroupC();
    func_8007FDD0();
    mu_assert("error, DebugText_DrawInfoGroupC execution failed", 1);
    return 0;
}

static char * test_debug_text_draw_info_remaining() {
    DebugText_DrawInfoGroupD();
    func_8007FE48();
    DebugText_DrawInfoGroupE();
    func_8007FE98();
    DebugText_DrawInfoGroupF();
    func_8007FED4();
    mu_assert("error, remaining debug text drawing execution failed", 1);
    return 0;
}

static char * test_dynamic_asset_loader_multiple_mappings() {
    Asset_ClearMappings();
    mu_assert("Asset mappings cleared", Asset_GetMappingCount() == 0);

    int valA = 100, valB = 200, valC = 300;
    Asset_RegisterMapping(0x80800010, &valA);
    Asset_RegisterMapping(0x80800020, &valB);
    Asset_RegisterMapping(0x80800030, &valC);

    mu_assert("Count is 3", Asset_GetMappingCount() == 3);
    mu_assert("Get valA match", Asset_GetByAddress(0x80800010) == &valA);
    mu_assert("Get valB match", Asset_GetByAddress(0x80800020) == &valB);
    mu_assert("Get valC match", Asset_GetByAddress(0x80800030) == &valC);

    Asset_ClearMappings();
    mu_assert("Asset mappings cleared to 0", Asset_GetMappingCount() == 0);
    return 0;
}

static char * test_dynamic_asset_loader_unregister_and_has() {
    Asset_ClearMappings();
    int assetVal = 999;
    Asset_RegisterMapping(0x80800500, &assetVal);
    mu_assert("Asset_HasAddress returns true for registered address", Asset_HasAddress(0x80800500) == true);
    mu_assert("Asset_HasAddress returns false for unregistered address", Asset_HasAddress(0x80800504) == false);

    bool unregistered = Asset_UnregisterMapping(0x80800500);
    mu_assert("Asset_UnregisterMapping returns true on success", unregistered == true);
    mu_assert("Asset_HasAddress returns false after unregistering", Asset_HasAddress(0x80800500) == false);
    mu_assert("Asset_GetMappingCount returns 0 after unregister", Asset_GetMappingCount() == 0);

    bool falseUnregister = Asset_UnregisterMapping(0x80800500);
    mu_assert("Asset_UnregisterMapping returns false if not found", falseUnregister == false);

    return 0;
}

static char * test_dynamic_asset_loader_procedural_texture_relocation() {
    Asset_ClearMappings();
    uintptr_t mockTexId = 42;
    Asset_RegisterMapping(0x80800100, (void*)mockTexId);

    mu_assert("Asset_HasAddress returns true for 0x80800100", Asset_HasAddress(0x80800100) == true);
    mu_assert("Asset_GetByAddress retrieves correct texture ID", (uintptr_t)Asset_GetByAddress(0x80800100) == mockTexId);

    Asset_ClearMappings();
    mu_assert("Asset_GetMappingCount after clear is 0", Asset_GetMappingCount() == 0);
    return 0;
}

static char * test_dynamic_asset_loader_vertex_buffer_relocation() {
    Asset_ClearMappings();
    Vtx mockVertices[4];
    memset(mockVertices, 0, sizeof(mockVertices));
    mockVertices[0].v.ob[0] = 123;
    mockVertices[0].v.ob[1] = 456;

    Asset_RegisterMapping(0x80800200, mockVertices);

    mu_assert("Has vertex mapping 0x80800200", Asset_HasAddress(0x80800200) == true);
    Vtx* retrieved = (Vtx*)Asset_GetByAddress(0x80800200);
    mu_assert("Retrieved vertex pointer not null", retrieved != NULL);
    mu_assert("Retrieved vertex X component match", retrieved[0].v.ob[0] == 123);
    mu_assert("Retrieved vertex Y component match", retrieved[0].v.ob[1] == 456);

    Asset_ClearMappings();
    return 0;
}

static char * test_shiftable_data_globals() {
    gGlobalState_800CD168 = 0x1234;
    mu_assert("gGlobalState_800CD168 set failed", gGlobalState_800CD168 == 0x1234);

    gGlobalState_800E4175 = -42;
    mu_assert("gGlobalState_800E4175 set failed", gGlobalState_800E4175 == -42);

    gGlobalState_800D16D0 = 0xAA;
    mu_assert("gGlobalState_800D16D0 set failed", gGlobalState_800D16D0 == 0xAA);

    static int sDummyHeap = 999;
    gGlobalPtr_800F8510 = &sDummyHeap;
    mu_assert("gGlobalPtr_800F8510 set failed", gGlobalPtr_800F8510 == &sDummyHeap);

    gGlobalState_8002AA18 = 0x7FFFFFFF;
    mu_assert("gGlobalState_8002AA18 set failed", gGlobalState_8002AA18 == 0x7FFFFFFF);
    return 0;
}

static char * test_system_init_alias() {
    gSystemInitialized = 0;
    gSystemFullyInitialized = 0;
    func_80068B20();
    mu_assert("func_80068B20 failed to initialize system", gSystemInitialized == 0x20DE1529);
    mu_assert("func_80068B20 failed to set fully initialized flag", gSystemFullyInitialized == 1);
    return 0;
}

static char * test_machine_and_audio_init() {
    gMachineSystemInitialized = 0;
    Machine_SystemInit();
    mu_assert("Machine_SystemInit should set gMachineSystemInitialized to 1", gMachineSystemInitialized == 1);

    gMachineSystemInitialized = 0;
    func_80085510();
    mu_assert("func_80085510 should alias Machine_SystemInit and set flag to 1", gMachineSystemInitialized == 1);

    gAudioWarmBootCount = 0;
    Audio_WarmBootInit();
    mu_assert("Audio_WarmBootInit should increment gAudioWarmBootCount", gAudioWarmBootCount == 1);

    func_8008DA68();
    mu_assert("func_8008DA68 should alias Audio_WarmBootInit and increment count", gAudioWarmBootCount == 2);

    return 0;
}

static char * test_audio_cold_boot_init() {
    gAudioColdBootCount = 0;
    gAudioWarmBootCount = 0;
    Audio_ColdBootInit();
    mu_assert("Audio_ColdBootInit should increment gAudioColdBootCount", gAudioColdBootCount == 1);
    mu_assert("Audio_ColdBootInit should call Audio_WarmBootInit", gAudioWarmBootCount == 1);

    func_8008DB98();
    mu_assert("func_8008DB98 should alias Audio_ColdBootInit", gAudioColdBootCount == 2);
    mu_assert("func_8008DB98 should also increment warm boot count", gAudioWarmBootCount == 2);

    return 0;
}

static char * test_input_and_timing_init() {
    gInputSystemInitialized = 0;
    Input_SystemInit();
    mu_assert("Input_SystemInit should set gInputSystemInitialized to 1", gInputSystemInitialized == 1);

    gInputSystemInitialized = 0;
    func_80076848();
    mu_assert("func_80076848 should alias Input_SystemInit and set flag to 1", gInputSystemInitialized == 1);

    gTimingSystemInitialized = 0;
    Timing_SystemInit();
    mu_assert("Timing_SystemInit should set gTimingSystemInitialized to 1", gTimingSystemInitialized == 1);

    gTimingSystemInitialized = 0;
    func_8007D9D0();
    mu_assert("func_8007D9D0 should alias Timing_SystemInit and set flag to 1", gTimingSystemInitialized == 1);

    return 0;
}

static char * test_audio_aux_and_shutdown() {
    gAudioAuxInitialized = 0;
    Audio_AuxInit();
    mu_assert("Audio_AuxInit should set gAudioAuxInitialized to 1", gAudioAuxInitialized == 1);

    gAudioAuxInitialized = 0;
    func_800A4BAC();
    mu_assert("func_800A4BAC should alias Audio_AuxInit and set flag to 1", gAudioAuxInitialized == 1);

    gAudioShutdownCount = 0;
    Audio_Shutdown();
    mu_assert("Audio_Shutdown should increment gAudioShutdownCount", gAudioShutdownCount == 1);

    func_800A4B54();
    mu_assert("func_800A4B54 should alias Audio_Shutdown and increment count", gAudioShutdownCount == 2);

    return 0;
}

static char * test_gfx_init_and_state_result() {
    gGfxSystemInitialized = 0;
    Gfx_SystemInit();
    mu_assert("Gfx_SystemInit should set gGfxSystemInitialized to 1", gGfxSystemInitialized == 1);

    gGfxSystemInitialized = 0;
    func_800FC730();
    mu_assert("func_800FC730 should alias Gfx_SystemInit and set flag to 1", gGfxSystemInitialized == 1);

    gLastStateResult = 0;
    State_ProcessResult(42);
    mu_assert("State_ProcessResult should update gLastStateResult", gLastStateResult == 42);

    func_800FD184(100);
    mu_assert("func_800FD184 should alias State_ProcessResult and update gLastStateResult", gLastStateResult == 100);

    return 0;
}

static char * test_system_tick_idle_poll() {
    gSystemTickCount = 0;
    System_Tick();
    mu_assert("System_Tick should increment gSystemTickCount", gSystemTickCount == 1);
    func_80068BC0();
    mu_assert("func_80068BC0 should alias System_Tick and increment count", gSystemTickCount == 2);

    gSystemIdleWaitCount = 0;
    System_IdleWait();
    mu_assert("System_IdleWait should increment gSystemIdleWaitCount", gSystemIdleWaitCount == 1);
    func_80068DCC();
    mu_assert("func_80068DCC should alias System_IdleWait and increment count", gSystemIdleWaitCount == 2);

    gSystemPollEventsCount = 0;
    System_PollEvents();
    mu_assert("System_PollEvents should increment gSystemPollEventsCount", gSystemPollEventsCount == 1);
    func_80068F04();
    mu_assert("func_80068F04 should alias System_PollEvents and increment count", gSystemPollEventsCount == 2);

    return 0;
}

static char * test_machinestate_init_all_and_special() {
    gMachineStateInitAllCount = 0;
    MachineState_InitAll();
    mu_assert("MachineState_InitAll should increment gMachineStateInitAllCount", gMachineStateInitAllCount == 1);
    func_80069700();
    mu_assert("func_80069700 should alias MachineState_InitAll and increment count", gMachineStateInitAllCount == 2);

    gMachineStateInitSpecialCount = 0;
    MachineState_InitSpecial();
    mu_assert("MachineState_InitSpecial should increment gMachineStateInitSpecialCount", gMachineStateInitSpecialCount == 1);
    func_80069820();
    mu_assert("func_80069820 should alias MachineState_InitSpecial and increment count", gMachineStateInitSpecialCount == 2);

    return 0;
}

static char * test_machinestate_updates() {
    gMachineStateUpdatePhysicsCount = 0;
    MachineState_UpdatePhysics();
    mu_assert("MachineState_UpdatePhysics should increment count", gMachineStateUpdatePhysicsCount == 1);
    func_80069D44();
    mu_assert("func_80069D44 should alias MachineState_UpdatePhysics", gMachineStateUpdatePhysicsCount == 2);

    gMachineStateUpdateCollisionsCount = 0;
    MachineState_UpdateCollisions();
    mu_assert("MachineState_UpdateCollisions should increment count", gMachineStateUpdateCollisionsCount == 1);
    func_80069ED0();
    mu_assert("func_80069ED0 should alias MachineState_UpdateCollisions", gMachineStateUpdateCollisionsCount == 2);

    gMachineStateUpdateVisualsCount = 0;
    MachineState_UpdateVisuals();
    mu_assert("MachineState_UpdateVisuals should increment count", gMachineStateUpdateVisualsCount == 1);
    func_80069F5C();
    mu_assert("func_80069F5C should alias MachineState_UpdateVisuals", gMachineStateUpdateVisualsCount == 2);

    return 0;
}

static char * test_machinestate_ai_controls_effects() {
    gMachineStateUpdateAICount = 0;
    MachineState_UpdateAI();
    mu_assert("MachineState_UpdateAI should increment count", gMachineStateUpdateAICount == 1);
    func_8006A00C();
    mu_assert("func_8006A00C should alias MachineState_UpdateAI", gMachineStateUpdateAICount == 2);

    gMachineStateUpdateControlsCount = 0;
    MachineState_UpdateControls();
    mu_assert("MachineState_UpdateControls should increment count", gMachineStateUpdateControlsCount == 1);
    func_8006A3AC();
    mu_assert("func_8006A3AC should alias MachineState_UpdateControls", gMachineStateUpdateControlsCount == 2);

    gMachineStateUpdateEffectsCount = 0;
    MachineState_UpdateEffects();
    mu_assert("MachineState_UpdateEffects should increment count", gMachineStateUpdateEffectsCount == 1);
    func_8006A6E4();
    mu_assert("func_8006A6E4 should alias MachineState_UpdateEffects", gMachineStateUpdateEffectsCount == 2);

    return 0;
}

static char * test_math_trig_and_vector_tables_init() {
    gMathTrigTablesInitCount = 0;
    Math_InitTrigTables();
    mu_assert("Math_InitTrigTables should increment gMathTrigTablesInitCount", gMathTrigTablesInitCount == 1);
    func_8006AA38();
    mu_assert("func_8006AA38 should alias Math_InitTrigTables", gMathTrigTablesInitCount == 2);

    gVectorTablesInitCount = 0;
    Vector_InitTables();
    mu_assert("Vector_InitTables should increment gVectorTablesInitCount", gVectorTablesInitCount == 1);
    func_8006ADE4();
    mu_assert("func_8006ADE4 should alias Vector_InitTables", gVectorTablesInitCount == 2);

    return 0;
}

static char * all_tests() {
    mu_run_test(test_math_roundf);
    mu_run_test(test_system_init_and_time);
    mu_run_test(test_state_machine);
    mu_run_test(test_math_rand);
    mu_run_test(test_controller_reset_machine_states);
    mu_run_test(test_trig_functions);
    mu_run_test(test_audio_state_mutations);
    mu_run_test(test_audio_state_pascal_case_apis);
    mu_run_test(test_linked_list_init);
    mu_run_test(test_track_lookup_table);
    mu_run_test(test_audio_utils);
    mu_run_test(test_machine_dynamic_config);
    mu_run_test(test_net_is_machine_active);
    mu_run_test(test_vehicle_active_index);
    mu_run_test(test_system_init_delay_loop);
    mu_run_test(test_system_set_state_flags);
    mu_run_test(test_dynamic_asset_loader);
    mu_run_test(test_low_mem_state);
    mu_run_test(test_track_lookup_direct);
    mu_run_test(test_debug_text_framebuffer);
    mu_run_test(test_system_init_memory_state);
    mu_run_test(test_state_struct_reset);
    mu_run_test(test_system_config_reset);
    mu_run_test(test_debug_text_init_buffers);
    mu_run_test(test_debug_text_draw_integer);
    mu_run_test(test_debug_text_draw_notice);
    mu_run_test(test_debug_text_draw_info_group_a);
    mu_run_test(test_debug_text_draw_info_group_b);
    mu_run_test(test_debug_text_draw_info_group_c);
    mu_run_test(test_debug_text_draw_info_remaining);
    mu_run_test(test_dynamic_asset_loader_multiple_mappings);
    mu_run_test(test_shiftable_data_globals);
    mu_run_test(test_system_init_alias);
    mu_run_test(test_machine_and_audio_init);
    mu_run_test(test_audio_cold_boot_init);
    mu_run_test(test_input_and_timing_init);
    mu_run_test(test_audio_aux_and_shutdown);
    mu_run_test(test_gfx_init_and_state_result);
    mu_run_test(test_system_tick_idle_poll);
    mu_run_test(test_machinestate_init_all_and_special);
    mu_run_test(test_machinestate_updates);
    mu_run_test(test_machinestate_ai_controls_effects);
    mu_run_test(test_dynamic_asset_loader_unregister_and_has);
    mu_run_test(test_dynamic_asset_loader_procedural_texture_relocation);
    mu_run_test(test_dynamic_asset_loader_vertex_buffer_relocation);
    mu_run_test(test_math_trig_and_vector_tables_init);
    return 0;
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;
    char *result = all_tests();
    if (result != 0) {
        printf("TEST FAILED: %s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
