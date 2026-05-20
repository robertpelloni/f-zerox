# F-Zero X PC Port - Architectural Brainstorming

## 1. Decoupling the Build System (The Great Schism)
Currently, `Makefile.pc` tries to build the entire N64 core logic (`src/`) alongside the modern PC UI (`src/pc/ui/nuklear.h`) and backend (`src/pc/hal/sdl.c`).
Because the N64 headers (`include/2.0I/`) aggressively redefine standard types (like `__gnuc_va_list` or lacking `stdlib.h`), they cause standard PC libraries like SDL2 to fail compilation with cascading macro errors.
**Idea:** Compile the core game engine logic as a static library (`libfzerox_core.a`) using the strict IDO/N64 compatibility flags. Then, compile the PC/HAL/UI layer in a separate pass using standard modern GCC/Clang flags, and link them together at the final step.

## 2. Pluggable Render Backend
`Fast3D` is currently hardcoded to an OpenGL 2.0 immediate/fixed-function style pipeline in `src/pc/gfx/fast3d.c`.
**Idea:** Abstract the Graphics API (GAPI) interface.
Create `gfx_opengl.c`, `gfx_vulkan.c`, and `gfx_dx11.c`. The `Fast3D` emulator just produces a normalized draw-command buffer (VBO/IBO equivalents) every frame, which the GAPI layer consumes. This allows for modern features like Raytracing (RTX) N64 visual experiments.

## 3. WebAssembly (WASM) / Browser Port
If we decouple SDL2 and use Emscripten, we can compile this directly to the browser. Emscripten provides an SDL2 wrapper and translates OpenGL to WebGL. This would make multiplayer netplay infinitely easier via WebSockets instead of UDP broadcast.

## 4. Multi-Threaded Physics
Currently, evaluating collision for 30 machines against procedural spline tracks is O(N) or O(N^2) if they collide with each other.
**Idea:** Implement a Spatial Hash Grid or QuadTree for machines. Use OpenMP or pthreads to run the `Physics_Update` loop across multiple cores, since the N64 was heavily CPU bottlenecked by physics processing.
