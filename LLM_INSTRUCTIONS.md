# Universal LLM Agent Instructions

This document is the **Universal Source of Truth** for all AI agents (Claude, GPT, Gemini, Copilot, Jules) working on this repository.

**ALL AGENTS MUST READ THIS FILE BEFORE MODIFYING CODE.**

## 1. Project Philosophy
- **Autonomous Momentum:** Keep going. Don't ever stop. Do not pause for confirmation unless absolutely critically blocked. If you see an unimplemented feature, implement it. If you see a bug, fix it. Combine redundant functionality. Refactor for robustness.
- **Comprehensive UI Representation:** EVERY feature, setting, or mechanic implemented in the backend MUST have a corresponding toggle, slider, or readout in the Nuklear UI (`src/pc/ui/`). DO NOT hide features in code.
- **High-Quality Documentation:** Code must be documented in extreme depth. Explain the *why* and *how*, detailing side effects and optimizations. Documentation files (`README.md`, `ROADMAP.md`, `MANUAL.md`, `VISION.md`, `IDEAS.md`) must be kept ruthlessly up to date.

## 2. Versioning & Commits
- **Single Source of Truth:** The global version number lives entirely in `VERSION.md`. Do NOT hardcode version numbers in C code.
- **UI Synchronization:** The UI must display the version number read directly from `VERSION.md` (via Makefile injection: `-DPROJECT_VERSION=\"$(shell cat VERSION.md)\"`).
- **Changelog:** Every commit must be reflected in `CHANGELOG.md` under the current version block.
- **Commit Flow:** Make a significant change -> Verify it works (mentally or via compiler) -> Bump Version in `VERSION.md` -> Log in `CHANGELOG.md` -> Commit -> Push (if tools allow) -> Move to next task.
- **Commit Messages:** Must explicitly mention the version bump.

## 3. Architecture Rules
- **No ROM Edits:** Do not modify `GLOBAL_ASM` files directly. We are building the PC "Shell" (`src/pc/`) around the decompilation. If injecting original logic, remove the stub from `src/pc/game_stubs.c` and map global state in `src/pc/ultra_impl.c`.
- **C99 Standard:** Use C99. The Fast3D parser and UI are C-based.
- **Modular Subsystems:** Keep Audio, Network, Physics, and Render separate. Use `game_state.c` and `game_loop.c` to orchestrate them.
- **Sub-stepping Physics:** High speeds require sub-stepping in `physics.c` to prevent tunneling. Do not use simple Euler integration for collisions at 2000 km/h.

## 4. Submodules & Dependencies
- All submodules used in this project must be documented in `PROJECT_DASHBOARD.md` with their purpose, version, and location in the directory structure.
- If updating submodules, merge upstream changes intelligently. Resolve conflicts carefully to avoid losing progress.

## 5. Session Handoff
- When completing a session, update `HANDOFF.md` with an extremely detailed summary of what was done, what was learned, and what the next agent should focus on based on the `ROADMAP.md`.
