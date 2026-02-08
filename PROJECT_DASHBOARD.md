# Project Dashboard & Structure

## Directory Structure
*   **root/**: Build scripts and documentation.
*   **src/**: Source code.
    *   **game_*.c**: Decompiled N64 logic.
    *   **assets/**: Game assets (meshes, textures).
    *   **pc/**: PC Port Engine.
        *   **gfx/**: Graphics backend (Fast3D, Post-Processing).
        *   **sdl2/**: Hardware Abstraction Layer (HAL).
        *   **ui/**: Nuklear UI implementation.
*   **include/**: Headers.
*   **tools/**: Submodules and build utilities.

## Submodules
| Name | Path | Version | Purpose |
|------|------|---------|---------|
| **asm-differ** | `tools/asm-differ` | `1e81f18` | Diffing tool for matching assembly. |
| **asm-processor** | `tools/asm-processor` | `742406e` | Pre-processor for GLOBAL_ASM. |
| **ido5.3_cc** | `tools/ido5.3_cc` | `faa773c` | N64 Compiler. |
| **splat** | `tools/splat` | `f44113b` | ROM Splitter. |

## Build Status
*   **Version:** 0.1.16 (See VERSION.md)
*   **Target:** `fzerox_pc` (Linux/Windows via MinGW)
*   **Dependencies:** SDL2, OpenGL, Python 3.10+
