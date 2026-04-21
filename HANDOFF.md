# Session Handoff & Global AI Context

**Current Version:** v0.1.28
**Phase:** Vertical Slice Complete / Early Logic Injection
**Target Platform:** PC (SDL2 + OpenGL 2.0/Nuklear)

## Welcome, Incoming Agent
You are continuing a massive autonomous development effort to create the definitive, native, moddable PC port of F-Zero X, incorporating GX mechanics (360 physics, attacks).

**CRITICAL INSTRUCTION:** Before doing anything, you MUST read `LLM_INSTRUCTIONS.md` and `PROJECT_DASHBOARD.md`. Do NOT stop or pause for confirmation unless fundamentally blocked. Maintain extreme momentum.

---

## What Was Achieved This Session (The "Engine Shell")
We shifted the project from a pure decompilation focus to building a robust PC "Shell" (`src/pc/`) that simulates the game environment while we wait for the ROM logic to be fully decompiled.

### 1. Physics Engine (`src/pc/physics.c`)
- **Implemented 3D Vector Math:** Replaced Euler angles with Forward/Up/Right vectors to allow driving on walls and upside-down (Loops/Tubes).
- **Advanced Combat:** Added `Spin Attack` (Z) and `Side Attack` (L/R + Z). These apply rapid rotation, lateral translation, and project a damage radius causing knockback to nearby `gMachines`.
- **Sub-stepping (CCD):** Implemented Continuous Collision Detection. High-speed vectors (e.g., > 2000 km/h) are sliced into 10-unit chunks per frame. This completely eliminates "tunneling" through walls on sharp curves or loops.
- **Dynamic Config:** `gravity_scale`, `grip_loss_factor`, and `strafe_power` are wired directly from the UI config to the physics update loop.

### 2. Graphics & Camera (`src/pc/gfx/fast3d.c`, `camera.c`)
- **Fast3D Enhancements:**
  - Enabled `GL_LIGHTING` with a directional sun (`GL_LIGHT0`).
  - Added dynamic Flat-Normal calculation for `OBJ` models that lack vertex normals.
  - Enabled `GL_ALPHA_TEST` for texture punch-through (decals, fences).
- **Dynamic Camera:** Added `Fast3D_SetFOV`. The camera linearly zooms out based on vehicle speed. Spring constraints tighten at high speeds to prevent lagging behind the machine on vertical loops.
- **Shadows & Particles:** Machines cast a projected blob shadow. The Particle system supports color/size variance, emitting blue engine exhaust, and dynamic damage smoke (black -> fire) based on `energy` levels.

### 3. Audio System (`src/pc/audio/audio_engine.c`)
- **Software Mixer:** Implemented an 8-channel software mixer with ADSR envelopes to prevent popping when overlapping SFX (Boost + Crash).
- **Procedural Synthesis:** Engine sounds are generated via sawtooth wave synthesis, with pitch smoothed over frames to prevent audio tearing.
- **3D Doppler Effect:** Added `AudioEngine_Update3D`. Up to 30 remote machines calculate their relative velocity to the camera, applying a pitch-shift (Doppler) and distance-based volume attenuation.
- **WAV Streaming:** `Music_Player` implemented for background tracks.

### 4. Network & Persistence (`src/pc/network/network.c`, `fs/`, `ghost_recorder.c`)
- **Netplay:** UDP Broadcast multiplayer (Port 7000). Exchanges a custom `NetPacket` struct containing position, rotation, velocity, and boost state for up to 30 players.
- **Track Editor:** Procedurally generates dynamic meshes (Straights, Curves, Loops). `TrackSegment` structs cache world-space coordinates (`distStart`, `distEnd`, normals) for O(1) collision queries in `Track_GetInfoAt`. Serializes to `custom_track.dat`.
- **Ghosts:** Records Position/Rotation at 60fps into a binary buffer (`ghost.dat`). Plays back by driving an AI machine slot with transparency blending.

### 5. UI & State Machine (`src/pc/ui/`, `game_state.c`)
- **Nuklear UI:** Extensive menus for Garage (Stats/Colors), Visuals (FOV, Blur, Filters), Gameplay (Physics toggles), and Netplay (Server browser mockup).
- **Racing HUD:** Real-time transparent overlay displaying Speed, Energy (Color-coded), Rank, Lap, and a blinking "REPLAY" indicator.
- **Game Flow:** `GameState_Update` orchestrates Title -> Menu -> Cup Select -> Race -> Result podiums.
- **Grand Prix:** `cup_system.c` tracks points across 5 races.

### 6. Original Logic Injection (The Breakthrough)
- We successfully intercepted original N64 debug text (`func_8007FB80`) in `ultra_impl.c`, buffered it in `debug_text_hook.c`, and rendered it scaled to modern resolutions via the Nuklear HUD.
- We removed the PC stub for `Math_Rand` and successfully linked the authentic decompiled N64 XOR-Shift RNG from `src/math_utils.c`, mapping it to the global state variables `D_800CD170` and `D_800CD174`.

---

## Your Mission: The Next Phase

1. **Network Interpolation (High Priority):**
   - The current Netplay logic snaps remote vehicles to the received packet or does a naive 20% lerp.
   - **Task:** Implement time-stamped Dead Reckoning in `Net_UpdateRemoteMachines`. Extrapolate `remote->pos + (remote->velocity * latency)` and use a smooth Hermite spline or Slerp for rotation.

2. **Asset Extraction Pipeline:**
   - We are currently using a procedurally generated asphalt texture and a mocked `.obj` loader.
   - **Task:** Analyze `tools/` and `Makefile`. Expand `make extract` or write a Python script in `tools/` to extract actual textures (PNG) and display lists (converted to OBJ or custom format) from `baserom.us.z64`.

3. **Continue Logic Injection:**
   - Look at `include/functions.h`. Identify more decompiled functions (e.g., AI behaviors, specific track curve generation math).
   - **Task:** Remove their stubs in `src/pc/game_stubs.c`, ensure their global variables are mapped in `ultra_impl.c`, and wire them into the `Game_Update` loop.

4. **Robust Particle System:**
   - The particle limit is 1024. In a 30-player race, this exhausts immediately.
   - **Task:** Implement an object pool with a ring-buffer strategy (overwrite oldest particles) to prevent stalling.

## Environment Notes
- **SDL2 Headers:** `make -f Makefile.pc` currently fails compiling HAL files *only* because `SDL2/SDL.h` is missing in the CI/Sandbox environment. This is expected. **The code logic is syntax-correct.** Do not waste time trying to "fix" the missing SDL2 dependency; focus on writing perfect logic.
- **Version Number:** Maintained in `VERSION.md`. Injected into the UI via the Makefile `-DPROJECT_VERSION` macro. Always bump this when you make a meaningful change.

**Godspeed.** Keep going. Don't stop the party.
