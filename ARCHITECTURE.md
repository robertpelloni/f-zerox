# F-Zero X PC Port - Architecture Deep Dive

## 1. High-Level Overview
The F-Zero X PC Port is a **Native High-Level Emulation (HLE)** wrapper around the original game's decompiled logic. Unlike a traditional emulator (which emulates the CPU/RCP), this project compiles the original C code (`src/math_utils.c`) directly for the host architecture (x86_64) and replaces the N64 hardware calls with a custom **Hardware Abstraction Layer (HAL)**.

## 2. Core Subsystems

### 2.1. The HAL (Hardware Abstraction Layer)
Located in `src/pc/sdl2/`, the HAL bridges the gap between N64 SDK calls and modern PC APIs.
*   **Video (`hal_video.c`):** Initializes an SDL2 Window and OpenGL context. Handles resolution switching and fullscreen toggling via `gConfig`.
*   **Audio (`hal_audio.c`):** Uses `SDL_QueueAudio` to buffer raw PCM samples. It acts as the final mixer for the `Music_Player` and `Audio_Engine`.
*   **Input (`hal_input.c`):** Maps SDL Events (Keyboard) and `SDL_GameController` (Gamepad/Wheel) to the N64 `OSContPad` structure. It supports analog steering logic and deadzones.
*   **Haptics (`hal_haptic.c`):** Interfaces with `SDL_Haptic` to provide Force Feedback for Arcade Cabinet integration.

### 2.2. The Game Loop (`src/pc/game_loop.c`)
This is the heart of the PC port. It replaces the N64's scheduler.
1.  **Input Poll:** Calls `HAL_Input_Poll`.
2.  **Physics Update:** Iterates over 30 machines (`gMachines`), updating their position/velocity/rotation using the Vector Physics engine (`src/pc/physics.c`).
3.  **AI Update:** Simple steering logic for opponents (`src/pc/ai_system.c`).
4.  **Networking:** Broadcasts local state via UDP (`src/pc/network/`).
5.  **Arcade I/O:** Sends telemetry to motion seats (`src/pc/arcade/`).
6.  **Rendering:**
    *   Applies Camera transform (`src/pc/camera.c`).
    *   Renders Track (`src/pc/track_system.c`).
    *   Renders Machines via Fast3D.
    *   Renders HUD and UI Overlay.

### 2.3. Fast3D Renderer (`src/pc/gfx/fast3d.c`)
This is a custom GBI (Graphics Binary Interface) parser.
*   **Purpose:** The original game logic outputs Display Lists (`Gfx*`). The PC port must interpret these commands to draw 3D geometry.
*   **Architecture:** It mimics the N64 RCP state machine. It maintains a vertex buffer (`sVertexBuffer`) and processes commands like `G_VTX` (load verts) and `G_TRI1` (draw triangle).
*   **Translation:** N64 vertices are translated to `glVertex3s` calls. Textures are bound via `G_SETTIMG` which triggers the texture loader lookup.

### 2.4. Vector Physics Engine (`src/pc/physics.c`)
To support "Extreme-G" style 360-degree gameplay (loops, tubes), we moved away from Euler angles (Yaw/Pitch/Roll) for physics calculation.
*   **State:** Each vehicle tracks `Up`, `Forward`, and `Right` vectors.
*   **Gravity:** The `Up` vector is interpolated towards the track surface normal (retrieved via `Track_GetInfoAt`). This allows the car to drive upside down.
*   **Mechanics:** Includes "Momentum Turbo Slide" (MTS) and "Side Attack" logic from F-Zero GX.

### 2.5. Procedural Track Editor (`src/pc/track_editor.c`)
A runtime mesh generator.
*   **Data Model:** A linked list of `TrackSegment` (Straight, Curve, Loop, Pipe).
*   **Generation:** It walks the list, calculating the "Turtle" position/orientation, and extrudes vertices to form a ribbon or tube mesh.
*   **UVs:** Automatically generates texture coordinates for the "Asphalt" texture.

### 2.6. UI System (`src/pc/ui/`)
Built on the **Nuklear** immediate-mode GUI library.
*   **Integration:** Hooks into the main loop via `UI_Render`. It captures input via `UI_HandleEvent` to prevent game control while in menus.
*   **Configuration:** All widgets are bound directly to the global `gConfig` struct, ensuring WYSIWYG editing.

## 3. Data Flow
`Input (SDL)` -> `OSContPad` -> `Physics Engine` -> `Vehicle State` -> `Camera System` -> `Fast3D` -> `OpenGL`
                                      ^
                                      |
                                 `Arcade IO` (Output)
                                 `Network` (Sync)
