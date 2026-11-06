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
