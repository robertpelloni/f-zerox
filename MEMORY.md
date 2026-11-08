# F-Zero X PC Port - Memory & Design Log

## Architectural Decisions

### 1. The "Shell" Approach
Instead of modifying the decompiled N64 code directly, we wrap it in a `src/pc/` layer.
*   **Reason:** Preserves the accuracy of the original logic (`src/game_*.c`) while allowing modern features.
*   **Result:** A clean separation between "Emulation" (math/logic) and "Engine" (rendering/input).

### 2. Nuklear for UI
We chose **Nuklear** (single-header C library) over ImGui.
*   **Reason:** Zero C++ dependency. Fits perfectly with the C99 codebase of the N64 port.
*   **Implementation:** `src/pc/ui/` handles all menus via immediate mode calls. State is persisted in `gConfig`.

### 3. Fast3D via OpenGL 2.0 (Immediate Mode)
We implemented a GBI parser that maps N64 commands to `glBegin/glEnd`.
*   **Reason:** Simplicity and compatibility. N64 Display Lists are procedural/stateful, matching the legacy OpenGL pipeline better than modern VBOs for this stage of reverse engineering.
*   **Future:** Should migrate to VBOs/Shaders for performance, but Immediate Mode is "Good Enough" for N64 poly counts (3k-5k tris).

### 4. Vector Physics (360 Degree)
Standard Euler angles (Yaw/Pitch/Roll) failed for loop-de-loops (Gimbal lock).
*   **Solution:** We implemented a custom physics engine (`src/pc/physics.c`) using `Up`, `Forward`, and `Right` vectors.
*   **Gravity:** The `Up` vector aligns to the track surface normal, allowing driving on walls/ceilings (Tube tracks).

### 5. Procedural Assets
To avoid copyright issues and build friction (requiring a ROM), we implemented:
*   `assets.c`: Generates textures (Asphalt) at runtime.
*   `track_editor.c`: Generates meshes (Ribbons, Tubes) at runtime.
*   `obj_loader.c`: Loads external models to override placeholders.

## Design Preferences
*   **Code Style:** PascalCase for functions, gCamelCase for globals.
*   **Config:** Binary struct dumping (`fzerox_pc.bin`) for simplicity, but JSON is planned.
*   **Networking:** UDP Broadcast for zero-config LAN play.

### 6. Netplay Handshake & Dead Reckoning
*   **Protocol:** Uses a handshake protocol via `Net_ConnectLobby` (`PACKET_HANDSHAKE`) to securely assign client IDs by resolving timestamp collisions.
*   **Extrapolation:** Remote clients implement time-stamped Dead Reckoning by extrapolating positions from velocity and smoothing out network jitter via exponential decay lerp.

### 7. Blob Shadows & Particles
*   **Shadows:** Semi-transparent, fixed-function `GL_TRIANGLE_FAN` blob shadows render directly beneath vehicles, adding visual grounding.
*   **Particles:** Uses a fast ring-buffer object pool that avoids stuttering even under heavy combat effects.

## Known Hardcoded Globals (D_80...)

- `D_800E4348`: Current Framebuffer pointer in `debug_text.c`, `game_197D0.c`, and `ultra_impl.c` (Refactored to `gCurrentFramebuffer` with `DebugText_UpdateFramebuffer` and `func_8007F94C`).
- `D_800E42CC`: Memory state parameter in `game_194E0.c` (Refactored to `gSystemMemoryStateValue` with `System_InitMemoryState` and `func_8007F500`).
- `struct UnkStruct_5`: Machine/subsystem state structure in `src/game_459A0.c` reset logic (`StateStruct_Reset` / `func_800B2B50`).
- `D_800E33C0`: A lookup array or table. Populated via `func_8006D414` and accessed via `func_8006D448` in `game_73F0.c`, and directly accessed via `TrackLookup_GetEntryDirect` in `game_10490.c` (Refactored to `gTrackLookupTable[22]`).
- `D_800D1A3C` / `D_800D1A20` / `D_800D1A48` / `D_800D19E0` / `D_800D1C18`: Audio state machine flags used in `game_511D0.c` (libaudio backend interactions).
- `D_800D0AC0` / `D_800D0AC4`: Machine dynamic config globals set in `game_459A0.c` (Refactored to `gDynamicMachineConfigA` / `gDynamicMachineConfigB` with `Machine_SetConfigA` / `Machine_SetConfigB`).
- `D_800CD510` / `D_800E4264` / `D_800E4260`: System state flags and parameters in `game_194E0.c` (Refactored to `gSystemStateFlag`, `gSystemStateParamA`, `gSystemStateParamB` with `System_SetStateFlags`).
- `D_8002B346` / `D_8002B347` / `D_8002B348` / `D_80025D78`: Low memory input and status variables in `game_4FFB0.c` (Refactored to `gLowMemInputPrev`, `gLowMemInputCurrent`, `gLowMemInputFlag`, and `gLowMemStatus` with `Input_UpdateLowMemState` and `Input_ClearLowMemStatus`).
- `Dynamic Asset Loader`: `src/pc/assets/dynamic_asset_loader.c` maps hardcoded ROM addresses (such as `0x80800000`, `0x80800100`) to dynamically loaded PC assets; supports `Asset_GetMappingCount()` and `Asset_ClearMappings()`.
- `D_800E5E84`: Active vehicle index in `game_1F510.c` (Refactored to `gActiveVehicleIndex` with `Vehicle_SetActiveIndex`).
- `D_800E4068`: System initialization delay loop counter in `game_11CF0.c` (Refactored to `gInitLoopCounter` with `System_RunInitDelayLoop`).
- `D_800DCD04` through `D_800DCD0C`: Cycle registers managed in `game_1AE0.c` (Refactored to `gGameStateCycleA/B/C`, `gGameStateControlMask`).
- `D_800D1A3C` through `D_800D1C18`: Audio state machine variables in `src/audio_state.c` (Refactored to `gAudioStateParamA/B/C`, `gAudioStateActiveFlag`, `gAudioStatePlayTrigger`, `gAudioStateResetFlag`, `gAudioStateStatus`).
- `struct UnkStruct_1` / `ListNode`: In `include/structs.h` and `src/linked_list.c`, circular linked list nodes refactored into union-aliased 64-bit safe self-referencing pointers.
- `gStateHandlers`: State machine function pointer table in `src/math_utils.c` converted to `uintptr_t` for 64-bit portability.
- `D_800D4690`: Sound effect bank map byte table in `src/audio_util.c` and `src/game_14440.c` (Refactored to `gSoundEffectBankMap` with `Audio_SetSoundId` and `Audio_GetSoundEffectByte`).
- `D_800D46C0` through `D_800D488C`: A block of variables exclusively used as format or parameter strings for `func_8007FB80` (debug text rendering) in `debug_text.c`.
- `D_800CD170` / `D_800CD174`: Seed states for `Math_Rand` PRNG in `math_utils.c` (successfully mapped in the PC port).
- `D_8010B7B0`: System config state struct initialized/reset in `game_73F0.c` (Refactored to `gSystemConfigState` with `SystemConfig_Reset` and `func_800742D0`).
- `D_800CD520`, `D_80400008`, `D_800E4350`, `D_800E44D0`, `D_800E4650`: Debug text buffers (Refactored to `gDebugBufferState`, `gDebugBufferAux`, `gDebugBufferA/B/C` with `DebugText_InitBuffers` and `func_8007F904`).
