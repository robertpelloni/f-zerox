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
