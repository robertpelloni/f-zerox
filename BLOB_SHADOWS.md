# Blob Shadow Rendering Documentation

## Overview
This document covers the implementation details for the newly integrated **blob-shadow rendering** system within the `Fast3D` PC Port engine. This was added to enhance visual fidelity ("GX Quality") during early engine bootstrap before programmable shaders are fully active.

## Pipeline Integration
Currently, the blob shadow is rendered inside `src/pc/game_loop.c` (`Game_RunFrame()`) as a direct immediate-mode OpenGL `GL_TRIANGLE_FAN` instruction.
It uses legacy blending to overlay a dark, semi-transparent shape beneath the vehicle.

### GBI Mapping
In the authentic SGI IDO toolchain, this would typically involve parsing specific Display List (`Gfx`) commands via the `G_TRI1` macro or a custom microcode intercept. Because we bypass the RDP for PC-native UI, we intercept the rendering at the top of the C-level Game Loop rather than sending `Gfx` commands to the `Fast3D_ProcessDisplayList` parser.

## Shader Tweaks & Future Migration
Currently, the shadow relies on:
```c
glEnable(GL_BLEND);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glDepthMask(GL_FALSE); // Don't write to depth buffer
```
**Future Migration:**
For the next iteration, when the renderer fully pivots to a shader-based (VBO/VAO) system instead of `glBegin`/`glEnd`, this blob shadow logic should be moved out of `Game_RunFrame` and converted into a textured quad (or a 2D distance field shader) bound to a dedicated render pass before the `Fast3D` track compilation.

## Math Helpers
The shadow orientates itself dynamically using the newly exposed `TrackSurfaceInfo` struct from the track collision mesh (`src/pc/track_system.c`). We extract the cross product of the upward normal `(0, 1, 0)` and the returned `surf.nx`, `surf.ny`, `surf.nz` to simulate correct pitching when vehicles drive on walls/loops.
