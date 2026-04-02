# F-Zero X PC Port - API Reference

## 1. Hardware Abstraction Layer (`include/pc/hal.h`)
*   `HAL_Video_Init(config)`: Setup SDL window and GL context.
*   `HAL_Audio_Init(config)`: Setup SDL audio device.
*   `HAL_Input_Poll()`: Pump SDL events and update controllers.
*   `HAL_Haptic_Rumble(strength, duration)`: Trigger force feedback.

## 2. Physics & Game Logic (`include/pc/physics.h`, `game_loop.h`)
*   `Physics_Init(Vehicle* v)`: Reset vehicle state.
*   `Physics_Update(Vehicle* v, OSContPad* pad)`: Run simulation step (Drag, Accel, Turn, Collision).
*   `Game_RunFrame()`: Execute one tick of the engine (Input -> Physics -> Render).

## 3. Track System (`include/pc/track_system.h`, `track_editor.h`)
*   `Track_Init()`: Load the default or custom track.
*   `Track_Render()`: Submit the track Display List to Fast3D.
*   `Track_GetInfoAt(x, y, z)`: Raycast against track geometry to get Height, Width, and Normal Vector.
*   `Track_Editor_AddSegment(type)`: Add a piece to the procedural track.

## 4. Graphics Backend (`include/pc/gfx/fast3d.h`)
*   `Fast3D_Init()`: Setup GL state (Depth, Cull).
*   `Fast3D_ProcessDisplayList(Gfx* dl)`: Parse N64 GBI commands and render immediately.
*   `Tex_Load(path)`: Load a PNG from disk into an OpenGL texture.

## 5. Network (`include/pc/network/network.h`)
*   `Net_Init(port)`: Bind UDP socket.
*   `Net_BroadcastPos(Vehicle* v)`: Send telemetry packet.
*   `Net_Receive()`: Process incoming packets and update `gMachines`.

## 6. Arcade I/O (`include/pc/arcade/arcade_io.h`)
*   `Arcade_Init(port)`: Open serial connection to motion board.
*   `Arcade_SendMotion(pitch, roll, heave)`: Send 3-DOF data packet.
