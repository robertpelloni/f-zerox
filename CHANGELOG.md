# Changelog

All notable changes to this project will be documented in this file.

## [0.1.38] - 2026-11-06
### Added
- **Low Memory State Shiftability**: Decoupled `D_8002B346`, `D_8002B347`, `D_8002B348`, and `D_80025D78` into typed `gLowMemInputPrev`, `gLowMemInputCurrent`, `gLowMemInputFlag`, and `gLowMemStatus` in `include/variables.h` and `src/pc/ultra_impl.c`.
- **Low Memory Input Logic**: Implemented `Input_UpdateLowMemState` and `Input_ClearLowMemStatus` in `src/game_4FFB0.c`.
- **Testing**: Added `test_low_mem_state` in `tests/test_math_utils.c` and integrated `src/game_4FFB0.o` into `Makefile.pc` (17 tests passing).

## [0.1.37] - 2026-11-06
### Added
- **Dynamic Asset Loader**: Implemented `Asset_GetMappingCount` and `Asset_ClearMappings` in `src/pc/assets/dynamic_asset_loader.c` and `include/pc/assets/dynamic_asset_loader.h`.
- **Testing**: Added `test_dynamic_asset_loader` in `tests/test_math_utils.c` and integrated `src/pc/assets/dynamic_asset_loader.o` into `Makefile.pc` (16 tests passing).

## [0.1.36] - 2026-11-06
### Added
- **System State Flags Shiftability**: Decoupled `D_800CD510`, `D_800E4264`, and `D_800E4260` into typed `gSystemStateFlag`, `gSystemStateParamA`, and `gSystemStateParamB` in `include/variables.h` and `src/pc/ultra_impl.c`.
- **System Logic**: Implemented `System_SetStateFlags` in `src/game_194E0.c`.
- **Testing**: Added `test_system_set_state_flags` in `tests/test_math_utils.c` and integrated `src/game_194E0.o` into the `Makefile.pc` test runner (15 tests passing).

## [0.1.35] - 2026-11-06
### Added
- **Network Telemetry**: Implemented `Net_GetConnectedCount` and `Net_GetLocalPlayerId` in `src/pc/network/network.c` and `include/pc/network/network.h`.
- **UI Integration**: Added live participant counter and local player ID indicator in `src/pc/ui/tabs/ui_tab_netplay.c`.
- **Testing**: Added telemetry assertions in `tests/test_math_utils.c`.

## [0.1.34] - 2026-11-06
### Added
- **Init Loop Shiftability**: Decoupled `D_800E4068` into typed `gInitLoopCounter` in `include/variables.h` and `src/pc/ultra_impl.c`.
- **System Logic**: Implemented `System_RunInitDelayLoop` in `src/game_11CF0.c`.
- **Testing**: Added `test_system_init_delay_loop` in `tests/test_math_utils.c` and integrated `src/game_11CF0.o` into the `Makefile.pc` test runner (14 tests passing).

## [0.1.33] - 2026-11-06
### Added
- **Vehicle State Shiftability**: Decoupled `D_800E5E84` into typed `gActiveVehicleIndex` in `include/variables.h` and `src/pc/ultra_impl.c`.
- **Vehicle State Logic**: Implemented `Vehicle_SetActiveIndex` in `src/game_1F510.c`.
- **Testing**: Added `test_vehicle_active_index` in `tests/test_math_utils.c` and integrated `src/game_1F510.o` into the `Makefile.pc` test runner (13 tests passing).

## [0.1.32] - 2026-11-06
### Fixed
- **Network**: Removed redundant redefinition of `Net_IsMachineActive` in `src/pc/network/network.c`.
### Added
- **Testing**: Added `test_net_is_machine_active` verifying ID validation, range checks, and local player identification in `tests/test_math_utils.c`.
- **Infrastructure**: Integrated `src/pc/network/network.o` and `src/pc/configfile.o` into the `Makefile.pc` test runner (12 tests passing).

## [0.1.31] - 2026-11-06
### Added
- **Machine Config Shiftability**: Decoupled `D_800D0AC0` and `D_800D0AC4` into typed `gDynamicMachineConfigA` and `gDynamicMachineConfigB` in `include/variables.h` and `src/pc/ultra_impl.c`.
- **Machine Logic**: Implemented `Machine_SetConfigA` and `Machine_SetConfigB` in `src/game_459A0.c`.
- **Testing**: Added `test_machine_dynamic_config` to `tests/test_math_utils.c` and integrated `src/game_459A0.o` into the `Makefile.pc` test runner (11 tests passing).

## [0.1.30] - 2026-11-06
### Added
- **Audio Utility Shiftability**: Decoupled `D_800D4690` into typed `gSoundEffectBankMap` in `include/variables.h` and `src/pc/libaudio_impl.c`.
- **Audio Utilities**: Implemented `Audio_SetSoundId` (masking sound ID to 8-bit) and `Audio_GetSoundEffectByte` in `src/audio_util.c` and `src/game_14440.c`.
- **Testing**: Added `test_audio_utils` to `tests/test_math_utils.c` and integrated `src/audio_util.o` into the `Makefile.pc` test runner (10 tests passing).

## [0.1.29] - 2026-11-06
### Added
- **Track Subsystem Shiftability**: Decompiled and shifted `D_800E33C0` into typed `gTrackLookupTable[22]` with helper functions `TrackLookup_ValidateIndex`, `TrackLookup_SetEntry`, and `TrackLookup_GetEntry` in `src/game_73F0.c` and `include/variables.h`.
- **Testing**: Added `test_track_lookup_table` in `tests/test_math_utils.c` and integrated `src/game_73F0.o` into the `Makefile.pc` test runner (9 tests passing).

## [0.1.28] - 2026-11-06
### Added
- **Shiftability & Data Structures**: Refactored `struct UnkStruct_1` into 64-bit safe self-referencing `ListNode` with typed `next`/`prev` pointers in `include/structs.h`.
- **Linked List Decomp**: Implemented `List_InitNode` in `src/linked_list.c` for circular node self-initialization.
- **Testing**: Added `test_linked_list_init` to `tests/test_math_utils.c` and integrated `src/linked_list.o` into `Makefile.pc` test suite (8 tests passing).

## [0.1.27] - 2026-11-06
### Added
- **Audio Shiftability**: Refactored audio state globals (`D_800D1A3C`, `D_800D1A20`, `D_800D1A48`, `D_800D19E0`, `D_800D1A18`, `D_800D1A0C`, `D_800D1C18`) into typed C symbols (`gAudioStateParamA/B/C`, `gAudioStateActiveFlag`, `gAudioStatePlayTrigger`, `gAudioStateResetFlag`, `gAudioStateStatus`) with PascalCase function aliases in `src/audio_state.c` and `src/game_511D0.c`.
- **UI**: Added Audio State monitoring and trigger controls to the Decompilation & Shiftability monitor in `src/pc/ui/tabs/ui_tab_debug.c`.
- **Testing**: Added `test_audio_state_mutations` in `tests/test_math_utils.c` covering all audio state mutations.

## [0.1.26] - 2026-11-06
### Added
- **Decompilation**: Expanded comprehensive unit tests in `tests/test_math_utils.c` covering `System_Init`, `System_SetTime`, `StateMachine_ExecuteCurrentState`, `Controller_ResetMachineStates`, `Math_Rand`, and trigonometry functions (`Math_SinS`, `Math_CosS`, `Math_SqrtF`).
- **UI**: Added a "Decompilation & Shiftability Monitor" to `src/pc/ui/tabs/ui_tab_debug.c` with live indicators for `gSystemInitialized`, `gCurrentStateIndex`, and `gGameStateCycleA/B/C`, accompanied by tooltips and interactive debug triggers.
- **Engine**: Integrated `System_Init()` execution directly into `Game_Init()` in `src/pc/game_loop.c`.

### Changed
- **Shiftability**: Refactored `D_800DCD04`, `D_800DCD08`, `D_800DCD0C`, and `D_800DCCF8` in `src/game_1AE0.c` and `include/variables.h` into named shiftable symbols (`gGameStateCycleA`, `gGameStateCycleB`, `gGameStateCycleC`, `gGameStateControlMask`) with legacy macro aliases.
- **Architecture**: Refactored `gStateHandlers` jump table in `src/math_utils.c` to `uintptr_t` for 64-bit safe pointer handling without integer-to-pointer truncation.

### Fixed
- **Testing**: Resolved duplicate `DebugText_AddString` linker error in `tests/stubs.c`.
- **Logic**: Corrected reverse iteration boundary logic in `Controller_ResetMachineStates()` in `src/math_utils.c`.

## [0.1.22] - 2026-05-21
### Added
- **Netplay**: Implemented `Net_ConnectLobby` handshake protocol to negotiate and assign Player IDs dynamically, resolving collisions instead of relying on immediate randomized broadcast.

## [0.1.21] - 2026-05-20
### Added
- **Visuals**: Implemented blob shadow rendering under the player vehicle.

## [0.1.20] - 2026-05-20
### Added
- **Visuals**: Implemented Damage Smoke Trails. Machines with low energy spawn black smoke and fire particles.

## [0.1.19] - 2026-05-20
### Added
- **Audio**: Implemented `AudioEngine_Update3D` for 3D Doppler pitch shifting and distance attenuation of remote vehicles.
- **Audio**: Implemented an 8-channel software mixer with procedural synthesis for engine noises.

## [0.1.18] - 2026-05-20
### Added
- **Netplay**: Implemented capped Dead Reckoning prediction for remote machines.
- **Netplay**: Implemented exponential decay Lerp for smoother visual correction of network jitter.

### Added
- **Graphics**: Implemented basic directional lighting in Fast3D.
### Fixed

### Added
- **PC Port Engine**: Implemented `src/pc/game_loop.c` and `src/pc/gfx/fast3d.c`, allowing the game to render N64 display lists via OpenGL.
- **Assets**: Added `src/assets/machines/blue_falcon.c` as a test asset.
- **UI Expansion**: Implemented tabs for Gameplay, Visuals, Garage, Netplay, and Editor using Nuklear.
- **Configuration**: Added `src/pc/configfile.c` to persist all settings.
- **Input Mapping**: Added `src/pc/controller_api.c` for dynamic key rebinding.
- **Documentation**: Overhauled documentation system with `LLM_INSTRUCTIONS.md`, `VISION.md`, and `PROJECT_DASHBOARD.md`.

## [0.1.15] - 2024-12-27
### Added
- Initial UI Implementation (Nuklear).

## [0.1.10] - 2024-12-27
### Fixed
- **PC Port**: Fixed linker errors by stubbing missing global variables and functions in `src/pc/ultra_impl.c`.
- **Infrastructure**: Reordered changelog entries to be chronological.

## [0.1.11] - 2024-12-27
### Changed
- Updated `HANDOFF.md` to reflect that the ROM blocker is resolved but a Toolchain blocker remains.

## [0.1.9] - 2024-12-27
### Changed
- **Decompilation**: Obtained base ROM and initialized decompilation environment (`make extract`).
- **Decompilation**: Decompiled `func_80068B20` (System Init) in `src/math_utils.c`.
- **Decompilation**: Decompiled `func_80069698` (State Machine) in `src/math_utils.c`.
- **Decompilation**: Decompiled `func_80069790` (Controller Init) in `src/math_utils.c`.
- **Decompilation**: Decompiled `func_8006A918` (Math_Rand) in `src/math_utils.c`.
- **Infrastructure**: Updated `variables.h` with new globals.

## [0.1.13] - 2024-12-27
### Fixed
- **Decompilation**: Updated `GLOBAL_ASM` paths in renamed source files to match the new directory structure generated by `splat` (e.g., `asm/nonmatchings/math_utils/`). This fixes the N64 build regression.
- **Cleanup**: Restored decompiled C code in renamed files and deleted orphaned source files.

## [0.1.8] - 2024-12-27
### Changed
- **Refactor**: Renamed key source files to meaningful names (`math_utils.c`, `debug_text.c`, etc.).
- Updated `f-zerox.us.yaml` to reflect file renames.
- Updated `Makefile.pc` and documentation to track file renames.

## [0.1.7] - 2024-12-27
### Changed
- Updated `HANDOFF.md` to explicitly state the "Missing ROM" blocker and provide resolution options.

## [0.1.6] - 2024-12-27
### Added
- **Documentation**: Overhauled global AI documentation rules (`LLM_INSTRUCTIONS.md`, `CLAUDE.md`, `GPT.md`, `GEMINI.md`, `copilot-instructions.md`) to establish a universal source of truth.
- **UI**: Added configuration options for Track segment width and banking directly in `ui_tab_editor.c`.
- **UI**: Integrated the global `PROJECT_VERSION` macro into the build system so the UI automatically reads `VERSION.md`.

## [0.1.27] - 2024-05-24
### Added
- **Audio**: Implemented 3D Doppler pitch shifting and distance attenuation for remote AI engines in the software mixer.
- **Visuals**: Implemented Damage Smoke Trails. Machines with low energy spawn black smoke and fire particles.

## [0.1.26] - 2024-05-24
### Added
- **Logic Injection**: Intercepted N64 `func_8007FB80` (DrawText) to buffer debug strings.
- **UI**: Rendered N64 debug text over the PC GUI using Nuklear layout spaces.

## [0.1.25] - 2026-06-20
### Added
- **Visuals**: Implemented a multi-pass pseudo-bloom shader in `src/pc/gfx/post_processing.c` using accumulation and additive blending, compatible with legacy OpenGL pipelines.
- **UI**: Added a "Enable Bloom Shader" toggle in `UI_Tab_Visuals`.

## [0.1.24] - 2024-05-24
### Added
- **Logic Injection**: Wired `Math_Rand` to use the authentic decompiled N64 XOR Shift RNG sequence.

## [0.1.23] - 2024-05-24
### Added
- **Graphics**: Fast3D Lighting (`GL_LIGHT0`) and blob shadows.
- **Audio**: 8-channel software mixer with ADSR envelopes.
- **Physics**: Wired UI config (Gravity, Grip) dynamically into update loop.

## [0.1.22] - 2024-05-24
### Added
- **Game Flow**: Grand Prix mode, tracking points across races.

## [0.1.21] - 2024-05-24
### Added
- **Combat**: Spin Attack and Side Attack implemented with UI toggles.
- **Docs**: Created `VISION.md` and `IDEAS.md`.

## [0.1.20] - 2024-05-24
### Added
- **Game Flow**: Scene graph management (Title -> Menu -> Select -> Race).
- **Persistence**: Ghost Recorder and Track Save/Load.
- **Audio**: WAV streaming music player.

## [0.1.18] - 2024-05-23
### Added
- **Physics**: 360-degree loop support, gravity on slopes, dynamic wall collision.
- **AI**: Spline-following AI with look-ahead steering.

## [0.1.15] - 2024-05-22
### Added
- **Core**: Initial PC Port Shell (SDL2 + OpenGL).
