# F-Zero X to GX/AX: Decompilation & Porting Roadmap

## Feature Status Matrix

| Feature | UI Status | Backend Status | Notes |
| :--- | :--- | :--- | :--- |
| **Render Pipeline** | Configurable | **Fast3D Implemented** | OpenGL 2.0 backend. Texture replacement logic active. |
| **Physics** | Configurable (Gravity, Grip) | **Advanced** | 360 vectors implemented. Loops and Tubes supported. |
| **Track Editor** | Full UI | **Procedural Mesh** | Straights, Curves, Loops, Pipes implemented. |
| **Audio** | Volume/Music Toggles | **Implemented** | Wav streaming + Procedural Engine + FFB. |
| **Netplay** | Lobby/Settings UI | **Implemented** | UDP Broadcast logic active with Prediction and Lerping. |
| **Arcade I/O** | Calibration UI | **Implemented** | Serial output for motion seats. |
| **Save/Load** | Editor UI | **Implemented** | Binary format for tracks. |
| **Modding** | N/A | **Implemented** | OBJ Loader active for Blue Falcon replacement. |

1.  **Complete Decompilation**
    -   **Current Status:** ~10% decompiled. **Documentation Phase Active** (due to missing ROM).
    -   **Action:** Systematically replace `GLOBAL_ASM` blocks with matching C code.
    -   **Priority:** High. This is the prerequisite for any meaningful porting work.
    -   **Milestone:** 100% C codebase matching the original US ROM.

2.  **Achieve Shiftability**
    -   **Current Status:** Shiftability unblocked. Asset loader (`Dynamic_Asset_Loader`) active. C wrappers implemented. Data section pointers still need migration.
    -   **Action:**
        -   Refactor identified `D_80` globals (see `MEMORY.md`) into properly typed C variables.
        -   Ensure the build system (`splat`, `make`) supports position-independent code or automatic relocation.
    -   **Milestone:** Ability to compile without any absolute pointer references in logic code.


3.  **Documentation & Naming**
    -   **Current Status:** **Partial**. Identified linked list nodes, debug functions, and math utilities.
    -   **Action:** rename variables (e.g., `func_80076498` -> `Physics_UpdateMomentum`) and document structs (e.g., `PlayerState`, `VehiclePhysics`).
    -   **Milestone:** A codebase readable by human contributors.

## Phase 2: PC Porting (Native Execution)
**Goal:** Break free from N64 hardware limitations by creating a platform abstraction layer (HAL) that runs natively on Windows/Linux/macOS.

1.  **Platform Abstraction Layer (PAL)**
    -   **Action:** Replace N64 hardware calls with generic interfaces.
        -   **Video:** Map N64 GBI (Graphics Binary Interface) commands to a modern backend (OpenGL/Vulkan/DirectX).
            -   *Status:* **Implemented**. `src/pc/gfx/fast3d.c` parses GBI and renders via OpenGL 2.0.
        -   **Audio:** Map N64 ABI (Audio Binary Interface) to SDL2/OpenAL.
            -   *Status:* **Implemented**. `src/pc/libaudio_impl.c` and `src/pc/sdl2/hal_audio.c` handle audio.
        -   **Input:** Map N64 controller inputs to SDL2 input (supporting Keyboard, Gamepad, Wheels).
            -   *Status:* **Implemented**. `src/pc/controller_api.c` handles remapping.
    -   **Milestone:** "F-Zero X PC" executable that boots and plays original content. (Achieved: Engine Shell runs with Blue Falcon asset).

2.  **Game Loop & Asset Integration**
    -   **Action:**
        -   Import assets (meshes, textures) into the PC build.
        -   Connect game logic to the render loop.
    -   *Status:* **Partially Achieved**. `Game_RunFrame` renders a Blue Falcon test asset.

3.  **UI & UX**
    -   **Action:** Create a modern, mouse-driven UI for settings and customization.
    -   *Status:* **Implemented**. Nuklear UI with Gameplay, Visuals, Garage, Netplay, Editor tabs.

## Phase 3: Modernization (Visuals & Audio)
**Goal:** "GX Quality" presentation.

1.  **Render Pipeline Enhancements**
    -   **Action:**
        -   Implement programmable shaders to replace fixed-function N64 combiner logic.
        -   Add support for dynamic lighting (basic directional lighting implemented), shadows, and bloom (mimicking GX's visual style).
        -   Implement Model Replacement: Allow loading .obj/.fbx/.gltf models to replace low-poly N64 assets.
    -   **Milestone:** Ability to load a "High-Res Texture/Model Pack".

2.  **Audio Engine Overhaul**
    -   **Action:**
        -   Support high-quality streaming audio (WAV/FLAC/OGG) to replace compressed MIDI sequences.
        -   Implement 3D spatial audio.

## Phase 4: Feature Parity (GX/AX Mechanics & Content)
**Goal:** Implement specific gameplay features and content from F-Zero GX and AX.

1.  **Physics Engine Variations**
    -   **Action:**
        -   Reverse engineer GX physics (Momentum Turbo Slide, different gravity/grip values).
        -   Implement a "Physics Toggle" in options: `Classic (X)`, `Modern (GX)`, `Arcade (AX)`.
    -   **Milestone:** "Snaking" works exactly as it does in GX when the option is enabled.

2.  **Content Import (Tracks & Machines)**
    -   **Action:**
        -   Create importers for GX/AX track data (.gma, .tpl).
        -   Implement GX-specific track elements: Cylinders, Half-pipes (if not fully present in X), and complex moving geometry.
        -   Port the 30+ GX machines and AX arcade exclusives.
    -   **Milestone:** Play "Mute City: Twist Road" (GX) in the F-Zero X engine.

3.  **Story Mode & Customization**
    -   **Action:**
        -   Implement a mission scripting engine (Lua or Python bindings) to recreate GX Story Mode chapters.
        -   Build a "Garage" UI for machine customization (color edit, decal editor).

## Phase 5: Expansion ("And Beyond")
**Goal:** Features never seen in the original games.

1.  **Online Multiplayer**
    -   **Action:**
        -   Implement rollback netcode (GGPO-style) or deterministic lockstep for 30-player races.
        -   Server browser and matchmaking.
    -   **Milestone:** 30-player "Death Race" online.

2.  **Modding API**
    -   **Action:** Expose game logic to scripts (Lua).
    -   **Milestone:** Community-created tracks and game modes (e.g., "Battle Royale").

3.  **VR Support**
    -   **Action:** Stereoscopic rendering and head tracking support (OpenXR).
    -   **Milestone:** F-Zero in First-Person VR.
