#include <stdio.h>
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

static char * all_tests() {
    mu_run_test(test_math_roundf);
    mu_run_test(test_system_init_and_time);
    mu_run_test(test_state_machine);
    mu_run_test(test_math_rand);
    mu_run_test(test_controller_reset_machine_states);
    mu_run_test(test_trig_functions);
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
