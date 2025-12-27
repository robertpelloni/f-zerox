# Changelog

All notable changes to this project will be documented in this file.

## [0.0.1] - 2024-12-27
### Added
- Created `ROADMAP.md` outlining the path to F-Zero GX/AX parity.
- Added `VERSION.md` for version tracking.
- Added `PROJECT_STRUCTURE.md` detailing the directory layout and submodules.
- Added `HANDOFF.md` for session continuity.
- Added `LLM_INSTRUCTIONS.md` and updated agent configuration files.
- Added Doxygen documentation for:
    - `src/game_197D0.c` (Debug/Framebuffer)
    - `src/game_19F40.c` (Struct Setters)
    - `src/game_1AE0.c` (Global Rotation)
    - `src/game_1F510.c` (Machine Loop)
    - `src/game_2B20.c` (Math/Vectors)
    - `src/game_44620.c` (libmus)
    - `src/game_446D0.c` (Linked List)
    - `src/game_459A0.c` (Variable Setters)
    - `src/game_511D0.c` (Audio State)
    - `src/game_73F0.c` (Bounds Check)
    - `src/game_11CF0.c` (Init Loop)
- Updated `include/structs.h` and `include/variables.h` with findings.

## [0.0.2] - 2024-12-27
### Added
- Documented `src/game_14440.c` (Audio utility).
- Documented `src/game_194E0.c` (State setter).
- Documented `src/game_4FFB0.c` (Low memory state update).

## [0.0.3] - 2024-12-27
### Added
- Populated `include/functions.h` with prototypes for all analyzed functions.

## [0.0.4] - 2024-12-27
### Changed
- Updated `include/structs.h` with detailed analysis of `UnkStruct_10` (Vectors), `UnkStruct_8`, and others.
- Updated `HANDOFF.md` to reflect the completion of the static analysis phase.

## [0.0.5] - 2024-12-27
### Changed
- Updated `HANDOFF.md` with build system origins (forked from `mkst/sssv`).
