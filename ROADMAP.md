# F-Zero X to GX/AX: Decompilation & Porting Roadmap

## Feature Status Matrix

| Feature | UI Status | Backend Status | Notes |
| :--- | :--- | :--- | :--- |
| **Render Pipeline** | Configurable | **Fast3D Implemented** | OpenGL 2.0 backend. Texture replacement logic active. |
| **Physics** | Configurable (Gravity, Grip) | **Advanced** | 360 vectors implemented. Loops and Tubes supported. |
| **Track Editor** | Full UI | **Procedural Mesh** | Straights, Curves, Loops, Pipes implemented. |
| **Audio** | Volume/Music Toggles | **Implemented** | Wav streaming + Procedural Engine + FFB. |
| **Netplay** | Lobby/Settings UI | **Implemented** | UDP Broadcast logic active (No interpolation yet). |
| **Arcade I/O** | Calibration UI | **Implemented** | Serial output for motion seats. |
| **Save/Load** | Editor UI | **Implemented** | Binary format for tracks. |
| **Modding** | N/A | **Implemented** | OBJ Loader active for Blue Falcon replacement. |

## Immediate Goals (v0.1.19+)
1.  **Netplay:** Implement State Interpolation (Lerp) to smooth remote movement.
2.  **Visuals:** Add Lighting/Shading to Fast3D (currently unlit/vertex color).
3.  **Content:** Implement "Cup" logic (5 track sequence).
