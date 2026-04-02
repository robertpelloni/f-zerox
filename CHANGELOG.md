# Changelog

## [0.1.28] - 2024-05-24
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
