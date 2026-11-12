#include "common.h"
#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "pc/assets/dynamic_asset_loader.h"

RandState gRandState;
TimeState gTimeState;
s16 gDebugMode = 0;
uintptr_t gStateHandlers[32] = {0};
s16 gSystemFullyInitialized = 0;
s32 gCurrentStateIndex = 0;
s32 gUnknownStateVar = 0;
s32 gSystemInitialized = 0;
u8 gMachineStates[592] = {0};
u8 gSpecialMachineState[256] = {0};
s16 gStateMachineActive = 0;

/**
 * @brief Initialize the game system and all subsystems
 *
 * Called during startup to initialize the entire game state including
 * audio system, graphics system, input system, and core game systems.
 * Uses state check to avoid re-initialization on soft resets.
 */
void System_Init(void) {
    gCurrentStateIndex = -1;
    gUnknownStateVar = 0x8000;

    // Check if already initialized using magic number
    if (gSystemInitialized != 0x20DE1529) {
        Audio_ColdBootInit(); // Audio system init (cold boot)
        gSystemInitialized = 0x20DE1529; // Set initialized flag
        Audio_AuxInit(); // Additional audio init
    } else {
        Audio_WarmBootInit(); // Audio system re-init (warm boot)
        Audio_Shutdown(); // Audio shutdown/restart
    }

    Machine_SystemInit(); // Machine system init
    Gfx_SystemInit(); // Graphics system init
    System_InitMemoryState(); // Memory system init
    Input_SystemInit(); // Input system init
    Timing_SystemInit(); // Timing system init

    // Validate if dynamic asset loader mappings are populated
    gSystemAssetsMapped = (Asset_GetMappingCount() > 0) ? 1 : 0;

    gSystemFullyInitialized = 1; // Mark system as fully initialized
}

void func_80068B20(void) {
    System_Init();
}

/**
 * @brief Advances a system tick.
 *
 * Increments tick tracking counter. Formerly func_80068BC0.
 */
void System_Tick(void) {
    gSystemTickCount++;
}

void func_80068BC0(void) {
    System_Tick();
}

/**
 * @brief System idle wait routine.
 *
 * Increments idle wait tracking counter. Formerly func_80068DCC.
 */
void System_IdleWait(void) {
    gSystemIdleWaitCount++;
}

void func_80068DCC(void) {
    System_IdleWait();
}

/**
 * @brief Polls system events.
 *
 * Increments event poll tracking counter. Formerly func_80068F04.
 */
void System_PollEvents(void) {
    gSystemPollEventsCount++;
}

void func_80068F04(void) {
    System_PollEvents();
}

/**
 * @brief Execute the current game state function from the state machine
 *
 * Routes to the appropriate state handler based on the current state index.
 * Uses a jump table with bounds checking to select the correct function.
 *
 * @return The result code from the state handler (0 for success, non-zero for errors)
 */
s32 StateMachine_ExecuteCurrentState(void) {
    s32 result = 0; // Default return value

    if (gDebugMode != 3) { // If not in debug/demo mode
        if (gStateMachineActive != 0) { // If state machine is active
            typedef s32 (*StateHandler)(void);
            // Calculate index with modulo from state value
            StateHandler handler = (StateHandler)(uintptr_t)gStateHandlers[gCurrentStateIndex & 0x1F];
            if (handler != NULL) {
                result = handler();
            }
        }
    }

    func_800FD184(result); // Process state result
    return result;
}

/**
 * @brief Initialize all machine states.
 *
 * Increments initialization count. Formerly func_80069700.
 */
void MachineState_InitAll(void) {
    gMachineStateInitAllCount++;
}

void func_80069700(void) {
    MachineState_InitAll();
}

/**
 * @brief Initialize or reset the machine state array
 *
 * Scans through an array of machine state structures (each 0x94 bytes)
 * and resets all fields for inactive machines. Active machines are skipped.
 * Also resets a special machine at a fixed memory offset.
 */
void Controller_ResetMachineStates(void) {
    u8* start = gMachineStates;
    u8* curr = gMachineStates + (0x94 * 4); // End of machine array (4 machines * 0x94 bytes)

    // Iterate backwards through machine states (machines 3 down to 0)
    while (curr > start) {
        curr -= 0x94;
        u8 isActive = *(curr + 0x6B);
        if (isActive != 0) {
            // Skip active machine, continue to previous
            continue;
        }

        // Clear all fields for inactive machine
        *(s16*)(curr + 0x80) = 0; // Unknown field A
        *(s16*)(curr + 0x7E) = 0; // Unknown field B
        *(s16*)(curr + 0x7C) = 0; // Unknown field C
        *(s16*)(curr + 0x7A) = 0; // Unknown field D
        *(s16*)(curr + 0x82) = 0; // Unknown field E
        *(u8*)(curr + 0x71) = 0;  // Flag A
        *(u8*)(curr + 0x70) = 0;  // Flag B
        *(u8*)(curr + 0x6F) = 0;  // Flag C
        *(u8*)(curr + 0x6E) = 0;  // Flag D
        *(u8*)(curr + 0x6D) = 0;  // Flag E
        *(u8*)(curr + 0x6C) = 0;  // Flag F
        *(s32*)(curr + 0x84) = 0; // Unknown pointer/value
    }

    // Reset special machine at fixed offset
    u8* specialMachine = gSpecialMachineState;
    *(u8*)(specialMachine + 0x6D) = 0;
    u8 temp = *(u8*)(specialMachine + 0x6D); // 0
    *(s16*)(specialMachine + 0x7E) = 0;
    *(s16*)(specialMachine + 0x7C) = 0;
    *(s16*)(specialMachine + 0x7A) = 0;
    *(s16*)(specialMachine + 0x82) = 0;
    *(u8*)(specialMachine + 0x70) = 0;
    *(u8*)(specialMachine + 0x6F) = 0;
    *(u8*)(specialMachine + 0x6E) = 0;
    *(u8*)(specialMachine + 0x6C) = temp;
}

/**
 * @brief Initialize special machine states.
 *
 * Increments special machine state init count. Formerly func_80069820.
 */
void MachineState_InitSpecial(void) {
    gMachineStateInitSpecialCount++;
}

void func_80069820(void) {
    MachineState_InitSpecial();
}

/**
 * @brief Updates machine physics simulation states (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full physics update routine from disassembly.
 * Increments physics update telemetry count. Formerly func_80069D44.
 */
void MachineState_UpdatePhysics(void) {
    gMachineStateUpdatePhysicsCount++;
}

void func_80069D44(void) {
    MachineState_UpdatePhysics();
}

/**
 * @brief Updates machine collision detection states (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full collision detection routine from disassembly.
 * Increments collision update telemetry count. Formerly func_80069ED0.
 */
void MachineState_UpdateCollisions(void) {
    gMachineStateUpdateCollisionsCount++;
}

void func_80069ED0(void) {
    MachineState_UpdateCollisions();
}

/**
 * @brief Updates machine visual presentation states (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full visuals update routine from disassembly.
 * Increments visual update telemetry count. Formerly func_80069F5C.
 */
void MachineState_UpdateVisuals(void) {
    gMachineStateUpdateVisualsCount++;
}

void func_80069F5C(void) {
    MachineState_UpdateVisuals();
}

/**
 * @brief Updates machine AI behaviors (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full AI behavioral routine from disassembly.
 * Increments AI update telemetry count. Formerly func_8006A00C.
 */
void MachineState_UpdateAI(void) {
    gMachineStateUpdateAICount++;
}

void func_8006A00C(void) {
    MachineState_UpdateAI();
}

/**
 * @brief Updates machine player input and controls (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full controls processing routine from disassembly.
 * Increments controls update telemetry count. Formerly func_8006A3AC.
 */
void MachineState_UpdateControls(void) {
    gMachineStateUpdateControlsCount++;
}

void func_8006A3AC(void) {
    MachineState_UpdateControls();
}

/**
 * @brief Updates machine special effects (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full effects processing routine from disassembly.
 * Increments effects update telemetry count. Formerly func_8006A6E4.
 */
void MachineState_UpdateEffects(void) {
    gMachineStateUpdateEffectsCount++;
}

void func_8006A6E4(void) {
    MachineState_UpdateEffects();
}

void osSetTime(OSTime time) { (void)time; /* TODO: Implement */ }

/**
 * @brief Set the time-related global variables
 *
 * @param timeValue1 First timing value (likely milliseconds or ticks)
 * @param timeValue2 Second timing value (likely seconds or frame count)
 */
void System_SetTime(s32 timeValue1, s32 timeValue2) {
    gTimeState.timeValue1 = timeValue1;
    gTimeState.timeValue2 = timeValue2;
}

/**
 * @brief XOR Shift Random Number Generator
 *
 * Implementation of a custom pseudorandom number generator using XOR shift
 * with two state variables. This is likely the game's main RNG used for
 * AI behavior, particle effects, and random game events.
 *
 * The algorithm:
 * - Uses two 32-bit state values (gRandState.state1 and gRandState.state2)
 * - Updates state1 using linear congruential generation
 * - Conditionally updates state2 based on its LSB
 * - Returns XOR of the two states
 *
 * @return 32-bit pseudo-random value
 */
s32 Math_Rand(void) {
    s32 state1 = gRandState.state1;
    u32 next_state1 = (u32)state1 * 0x41C64E6D + 0x3039;

    s32 state2 = gRandState.state2;
    if (state2 & 1) {
        gRandState.state1 = next_state1;
        state2 ^= 0x11020;
        gRandState.state2 = state2;
    }

    state1 = gRandState.state1;
    s32 final_state2 = (u32)state2 >> 1;
    gRandState.state2 = final_state2;

    return state1 ^ final_state2;
}

/**
 * @brief Round a floating point number to the nearest integer
 *
 * Uses standard rounding where values at exactly 0.5 round to the nearest
 * integer with ties rounding away from zero.
 *
 * Examples:
 * - Math_RoundF(3.4f) → 3
 * - Math_RoundF(3.6f) → 4
 * - Math_RoundF(-2.4f) → -2
 * - Math_RoundF(-2.6f) → -3
 *
 * @param value Input float value to round
 * @return Rounded integer value
 */
s32 Math_RoundF(f32 value) {
    if (value < 0.0f) {
        return (s32)(value - 0.5f);
    }
    return (s32)(value + 0.5f);
}

// TODO: Refactor pointer logic

float Math_SinS(int16_t angle) {
    // N64 angle: 0-65535 map to 0-2PI
    return sinf(angle * (M_PI / 32768.0f));
}

float Math_CosS(int16_t angle) {
    return cosf(angle * (M_PI / 32768.0f));
}

float Math_SqrtF(float value) {
    return sqrtf(value);
}

/**
 * @brief Initializes mathematical trigonometry lookup tables (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full trig lookup initialization from disassembly.
 * Increments initialization count. Formerly func_8006AA38.
 */
void Math_InitTrigTables(void) {
    gMathTrigTablesInitCount++;
}

void func_8006AA38(void) {
    Math_InitTrigTables();
}

/**
 * @brief Initializes geometric vector lookup tables (instrumentation hook).
 *
 * TODO: SHIFTABLE - Decompile full vector table initialization from disassembly.
 * Increments initialization count. Formerly func_8006ADE4.
 */
void Vector_InitTables(void) {
    gVectorTablesInitCount++;
}

void func_8006ADE4(void) {
    Vector_InitTables();
}

/**
 * @brief Set a dual-vector structure (both vectors to same values)
 *
 * Sets two 3-component vectors (likely RGB colors or XYZ positions) in the
 * structure to identical values. This is commonly used for color gradients
 * or symmetric positioning.
 *
 * @param vecStruct Pointer to the UnkStruct_10 containing two vectors
 * @param compX Value for X/Red component in both vectors
 * @param compY Value for Y/Green component in both vectors
 * @param compZ Value for Z/Blue component in both vectors
 */
void Vector_SetDual(struct UnkStruct_10* vecStruct, s32 compX, s32 compY, s32 compZ) {
    vecStruct->unk0 = vecStruct->unk4 = compX;
    vecStruct->unk1 = vecStruct->unk5 = compY;
    vecStruct->unk2 = vecStruct->unk6 = compZ;
}

void func_8006AFC8(struct UnkStruct_10* arg0, s32 arg1, s32 arg2, s32 arg3) {
    Vector_SetDual(arg0, arg1, arg2, arg3);
}

/**
 * @brief Set a dual-vector structure (alias of Vector_SetDual)
 *
 * Identical functionality to Vector_SetDual. Likely created by compiler
 * inlining or function duplication for optimization purposes.
 *
 * @param vecStruct Pointer to the UnkStruct_10 containing two vectors
 * @param compX Value for X/Red component in both vectors
 * @param compY Value for Y/Green component in both vectors
 * @param compZ Value for Z/Blue component in both vectors
 */
void Vector_SetDual_Alias(struct UnkStruct_10* vecStruct, s32 compX, s32 compY, s32 compZ) {
    vecStruct->unk0 = vecStruct->unk4 = compX;
    vecStruct->unk1 = vecStruct->unk5 = compY;
    vecStruct->unk2 = vecStruct->unk6 = compZ;
}

void func_8006AFE4(struct UnkStruct_10* arg0, s32 arg1, s32 arg2, s32 arg3) {
    Vector_SetDual_Alias(arg0, arg1, arg2, arg3);
}

/**
 * @brief Set the tail 3-component vector in UnkStruct_8
 *
 * Sets the vector components at offsets 0x8, 0x9, 0xA which appear to be
 * the last three bytes of the structure. Likely used for XYZ positions,
 * RGB colors, or velocity vectors.
 *
 * @param vecStruct Pointer to the UnkStruct_8
 * @param compX Value for component at offset 0x8
 * @param compY Value for component at offset 0x9
 * @param compZ Value for component at offset 0xA
 */
void Vector_SetTriple(struct UnkStruct_8* vecStruct, s32 compX, s32 compY, s32 compZ) {
    vecStruct->unk8 = compX;
    vecStruct->unk9 = compY;
    vecStruct->unkA = compZ;
}

void func_8006B000(struct UnkStruct_8* arg0, s32 arg1, s32 arg2, s32 arg3) {
    Vector_SetTriple(arg0, arg1, arg2, arg3);
}

/**
 * @brief Updates primary vector cache state.
 *
 * Increments vector cache A update counter. Formerly func_8006B010.
 */
void Vector_UpdateCacheA(void) {
    gVectorCacheACount++;
}

void func_8006B010(void) {
    Vector_UpdateCacheA();
}

/**
 * @brief Updates secondary vector cache state.
 *
 * Increments vector cache B update counter. Formerly func_8006B07C.
 */
void Vector_UpdateCacheB(void) {
    gVectorCacheBCount++;
}

void func_8006B07C(void) {
    Vector_UpdateCacheB();
}

/**
 * @brief Processes primary vector processing state.
 *
 * Increments vector process state A counter. Formerly func_8006B18C.
 */
void Vector_ProcessStateA(void) {
    gVectorProcessStateACount++;
}

void func_8006B18C(void) {
    Vector_ProcessStateA();
}

/**
 * @brief Processes secondary vector processing state.
 *
 * Increments vector process state B counter. Formerly func_8006B33C.
 */
void Vector_ProcessStateB(void) {
    gVectorProcessStateBCount++;
}

void func_8006B33C(void) {
    Vector_ProcessStateB();
}

// TODO: Refactor pointer logic
void func_8006B908(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006BB80(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006BBE8(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006BC84(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006BFCC(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006C278(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006C378(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006C520(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006CB0C(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006CC98(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006D03C(void) { /* TODO: Implement */ }

// TODO: Refactor pointer logic
void func_8006D2E0(void) { /* TODO: Implement */ }
