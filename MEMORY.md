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

- `D_800E4348`: Likely `gCurrentFramebuffer` or a rendering target. Used in `ultra_impl.c` and debug output.
- `D_800E33C0`: A lookup array or table. Populated via `func_8006D414` and accessed via `func_8006D448` in `game_73F0.c`.
- `D_800D1A3C` / `D_800D1A20` / `D_800D1A48` / `D_800D19E0` / `D_800D1C18`: Audio state machine flags used in `game_511D0.c` (libaudio backend interactions).
- `D_800D0AC0` / `D_800D0AC4`: Unknown subsystem globals set in `game_459A0.c`.
- `D_800DCD04` through `D_800DCD0C`: Cycle registers managed in `game_1AE0.c`.
- `D_800D46C0` through `D_800D488C`: A block of variables exclusively used as format or parameter strings for `func_8007FB80` (debug text rendering) in `debug_text.c`.
- `D_800CD170` / `D_800CD174`: Seed states for `Math_Rand` PRNG in `math_utils.c` (successfully mapped in the PC port).
- `D_8010B7B0`: Struct containing unknowns, initialized in `game_73F0.c`.
