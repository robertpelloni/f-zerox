#include <stdio.h>
#include <stdbool.h>
#include "minunit.h"
#include "../include/pc/ultra64.h"
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
    gSoundEffectBankMap = 77;
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

static char * all_tests() {
    mu_run_test(test_math_roundf);
    mu_run_test(test_system_init_and_time);
    mu_run_test(test_state_machine);
    mu_run_test(test_math_rand);
    mu_run_test(test_controller_reset_machine_states);
    mu_run_test(test_trig_functions);
    mu_run_test(test_audio_state_mutations);
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
