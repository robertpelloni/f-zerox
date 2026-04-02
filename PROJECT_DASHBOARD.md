# Project Dashboard: F-Zero X PC Port

**Status:** Vertical Slice Complete (v0.1.20)
**Goal:** Create a native, moddable, networked PC port of F-Zero X with GX mechanics.

## Architecture Overview
The project follows a "Shell" architecture, wrapping the original N64 game logic (simulated or decompiled) with a modern PC HAL (Hardware Abstraction Layer).

### Directory Structure
*   `src/pc/` - **PC Port Core**: Contains all platform-specific code.
    *   `gfx/` - Fast3D OpenGL Renderer, Texture Loading, Particles.
    *   `audio/` - Software Audio Mixer, procedural engine synthesis.
    *   `network/` - UDP Network Stack (Broadcast/Peer-to-Peer).
    *   `ui/` - Nuklear-based GUI system (Tabs, HUD).
    *   `fs/` - File System abstraction (Save/Load).
    *   `sdl2/` - HAL Implementation using SDL2.
*   `src/game_*.c` - **Game Logic**: Original decompiled C code (or stubs for now).
*   `include/pc/` - **Headers**: Interfaces for the PC subsystems.

## Module Status

| Module | Status | Features | Notes |
| :--- | :--- | :--- | :--- |
| **Render** | 🟢 Stable | OBJ Loading, Texture Replacement, Particles | Uses Fast3D GBI parser. |
| **Audio** | 🟡 Beta | Procedural Engine, WAV Streaming | Needs spatial audio refinement. |
| **Network** | 🟡 Beta | UDP Broadcast, Position Sync (30p) | Needs delta compression/interpolation. |
| **Physics** | 🟢 Stable | 360 Loop Support, Wall Bounce, Gravity | Gravity on slopes works correctly. |
| **AI** | 🟢 Stable | Spline Following, Proportional Steering | AI can navigate loops. |
| **Track** | 🟢 Stable | Dynamic Mesh Gen, Serialization | Save/Load works. Collision is robust. |
| **UI** | 🟢 Polish | Garage, Netplay, Editor Tabs | Fully integrated with Nuklear. |

## Submodules & Dependencies
*   **SDL2**: Windowing, Input, Audio Context.
*   **Nuklear**: Immediate Mode GUI (Single Header).
*   **stb_image**: Texture Loading (Single Header).
*   **minunit**: Unit Testing.

## Key Files
*   `src/pc/game_loop.c`: Central orchestrator (`Game_Init`, `Game_Update`, `Game_Render`).
*   `src/pc/physics.c`: Vehicle physics engine.
*   `src/pc/track_editor.c`: Procedural track generation and collision caching.
*   `src/pc/network/network.c`: Network packet handling.

## Build Instructions
```bash
make -f Makefile.pc
```
*Requires `libsdl2-dev` and a C compiler.*
