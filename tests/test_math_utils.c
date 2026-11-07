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
