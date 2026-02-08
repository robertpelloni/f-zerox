# F-Zero X PC Port (Decompilation) - LLM Instructions

## Universal Directives for AI Agents
You are an expert software engineer working on the F-Zero X PC Port. This file is the **Source of Truth** for your behavior, instructions, and project goals.

### 1. Core Principles
*   **Safety First:** Do not break the build. Do not delete files without verification. Do not overwrite logic without understanding it.
*   **Verification:** Always verify your changes. If you write code, try to compile it. If you cannot compile, verify syntax and structure rigorously.
*   **Documentation:** Every new feature must be documented in `MANUAL.md`. Every major architectural change must be updated in `PROJECT_STRUCTURE.md`.
*   **Versioning:** Check `VERSION.md`. If you merge a significant feature, bump the patch version (e.g., 0.1.15 -> 0.1.16) and update `CHANGELOG.md`.

### 2. Project Goals (The Vision)
*   **Goal 1: PC Port Shell:** Create a native Windows/Linux executable that runs the game logic without an emulator. (Status: Achieved via `src/pc/`).
*   **Goal 2: Feature Parity:** Implement features from F-Zero GX (Story Mode, Customization) and AX (Arcade Physics). (Status: UI Implemented, Logic In-Progress).
*   **Goal 3: Modernization:** Add High-FPS support, Widescreen, and Post-Processing (Bloom, Motion Blur).

### 3. Workflow Protocol
1.  **Analyze:** Read `ROADMAP.md` and `HANDOFF.md` before starting.
2.  **Plan:** Use `set_plan` to outline your steps.
3.  **Implement:** Write code in small, testable chunks.
4.  **Verify:** Check for linker errors, undefined symbols, and logical consistency.
5.  **Document:** Update `CHANGELOG.md` and `VERSION.md` before submitting.

### 4. Technical Guidelines
*   **Style:** Use C99 standard. Indent with 4 spaces.
*   **Naming:** Use `PascalCase` for functions (`Game_RunFrame`) and `gCamelCase` for globals (`gConfig`).
*   **Structure:**
    *   `src/pc/`: Platform-specific code (SDL2, OpenGL, UI).
    *   `src/game_*.c`: Original decompiled game logic.
    *   `include/pc/`: PC-specific headers.
*   **UI:** Use the **Nuklear** library (`src/pc/ui/`). All settings must be persisted via `gConfig`.

### 5. Specific Instructions for Models
*   **Claude:** Focus on architectural correctness and deep reasoning.
*   **GPT-4:** Focus on code generation and syntax verification.
*   **Gemini:** Focus on rapid implementation and searching for solutions.

### 6. Submodule Management
*   Always ensure submodules (`tools/asm-differ`, etc.) are documented in `PROJECT_DASHBOARD.md`.
*   If a submodule is updated, document the version and date.

### 7. Versioning Protocol
*   **File:** `VERSION.md` contains the single version string (e.g., `0.1.16`).
*   **Changelog:** `CHANGELOG.md` must have a new entry for the current version.
*   **Commit:** The commit message should reference the version bump.
