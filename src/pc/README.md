# F-Zero X PC Port Layer

This directory contains the code specific to the native PC port of F-Zero X.

## Architecture
The PC port uses a Hardware Abstraction Layer (HAL) to separate game logic from platform-specific APIs.

- **Game Logic**: Uses the original N64 engine code (mostly preserved).
- **HAL**: `include/pc/hal.h` defines the interface.
- **Backends**: Implementations of the HAL (e.g., SDL2, OpenGL).

## Planned Backends
- **Video**: Fast3D (N64 GBI emulation) on top of OpenGL/Vulkan.
- **Audio**: N64 ABI emulation on top of SDL2 Audio/OpenAL.
- **Input**: SDL2 Controller/Keyboard mappings to `OSContPad`.

## Build Instructions
(TODO: Add CMake or Make targets for PC build)
