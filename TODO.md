# F-Zero X PC Port - Short Term Goals

## Netplay
- [x] **Interpolation:** Implement Lerp/Slerp for remote vehicles to smooth out UDP packet jitter.
- [x] **Prediction:** Implement Dead Reckoning (extrapolate position based on velocity).
- [x] **Lobby:** Implement a handshake protocol to assign Player IDs (currently hardcoded/random).
- [x] **Lobby UI:** Add lobby UI to show connected players before starting a race.

## Visuals
- [x] **Particles:** Add smoke trails for damaged machines.
- [x] **Lighting:** Implement basic directional lighting in `Fast3D` (currently flat/vertex colored).
- [x] **Shadows:** Render a simple blob shadow under vehicles.
- [x] **Bloom:** Implement a bloom shader for glowing track elements.

## Audio
- [x] **Mixer:** Implement a software mixer to handle volume envelopes and multiple SFX channels properly.
- [x] **Doppler:** Pitch shift engine sounds based on relative velocity to camera.
- [x] **Dynamic Music:** Transition music intensity based on lap number.

## Content
- [x] **Cup Logic:** Implement the "Grand Prix" state machine (5 tracks, point tracking).
- [x] **AI:** Improve collision avoidance (boids algorithm).
- [x] **Track Importer:** Create an importer script/logic for GX `.gma`/`.tpl` track formats.

## Decompilation
- [x] **Math Utils:** Decompile all 100% logic in `src/math_utils.c`.
- [x] **Unit Testing:** Comprehensive test runner validation for `System_Init`, `StateMachine`, `Controller_ResetMachineStates`, `Math_Rand`, and trig.
- [ ] **State Logic:** Progressively decompile and match remaining stubs in `src/game_10490.c` and `src/game_73F0.c`.

## Engine / Shiftability
- [x] **Data:** Decouple hardcoded IDO pointers into linkable symbols.
- [x] **64-bit Portability:** Convert state handler function pointer tables (`gStateHandlers`) to `uintptr_t`.
- [x] **Cycle Registers:** Decouple and name cycle registers in `game_1AE0.c` (`gGameStateCycleA/B/C`, `gGameStateControlMask`).
- [x] **Audio State Globals:** Decouple and name audio state variables (`gAudioStateParamA/B/C`, `gAudioStateActiveFlag`, `gAudioStatePlayTrigger`, `gAudioStateResetFlag`, `gAudioStateStatus`).
- [x] **Linked List Structs:** Decouple `UnkStruct_1` into 64-bit safe `ListNode` with self-referencing pointers.
- [x] **Track Lookup Table:** Decouple `D_800E33C0` into `gTrackLookupTable[22]` with bounds-checked accessors.
- [x] **Audio Bank Map:** Decouple `D_800D4690` into `gSoundEffectBankMap` with `Audio_SetSoundId` and `Audio_GetSoundEffectByte`.
- [x] **Machine Config Globals:** Decouple `D_800D0AC0`/`D_800D0AC4` into `gDynamicMachineConfigA`/`B` with `Machine_SetConfigA`/`B`.
- [x] **Vehicle State Globals:** Decouple `D_800E5E84` into `gActiveVehicleIndex` with `Vehicle_SetActiveIndex`.
- [x] **Init Loop Globals:** Decouple `D_800E4068` into `gInitLoopCounter` with `System_RunInitDelayLoop`.
- [x] **Network Cleanup:** Fix duplicate function redefinitions in `network.c` and test active machine state verification.
- [x] **Network Telemetry:** Implement `Net_GetConnectedCount` and `Net_GetLocalPlayerId` with UI status telemetry.
- [x] **System State Globals:** Decouple `D_800CD510`, `D_800E4264`, `D_800E4260` into `gSystemStateFlag`/`A`/`B` with `System_SetStateFlags`.
- [x] **Dynamic Asset Loader API:** Implement `Asset_GetMappingCount` and `Asset_ClearMappings` with automated unit testing.
- [x] **Low Memory Globals:** Decouple `D_8002B346/7/8` and `D_80025D78` into `gLowMemInput*` and `gLowMemStatus` with `Input_UpdateLowMemState`.
- [x] **Direct Track Lookup:** Implement `TrackLookup_GetEntryDirect` in `game_10490.c` with bounds checks.
- [x] **Framebuffer Globals:** Decouple `D_800E4348` into `gCurrentFramebuffer` with `DebugText_UpdateFramebuffer`.
