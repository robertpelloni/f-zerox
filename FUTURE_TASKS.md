# Future Tasks & Roadmap

This document outlines the granular steps required to move from the current "Feature Complete Shell" to a shipping product.

## 1. Asset Pipeline (Priority: High)
*   **OBJ Loader:** Implement `src/pc/assets/obj_loader.c` to load `.obj` files for custom machines. Replace `blue_falcon_vtx` with a loaded model.
*   **ROM Extraction:** Integrate `splat` output directly. Write a script to convert extracted binary DisplayLists into C-compatible structs that `fast3d.c` can digest natively.

## 2. Network Polish (Priority: Medium)
*   **Interpolation:** Currently `Net_Receive` snaps position instantly. Implement linear interpolation (Lerp) for smooth movement.
*   **Lobby System:** The UI has a placeholder lobby. Implement a master server handshake or simple peer-discovery broadcast.

## 3. Physics & Gameplay (Priority: High)
*   **Collision Masks:** Current collision is point-based. Implement Mesh-vs-Mesh collision for more accurate wall riding.
*   **Loop Geometry:** The `Track_Editor` generates loops, but the camera sometimes clips. Refine the 360-degree camera math (`gluLookAt` equivalent).

## 4. Audio (Priority: Low)
*   **Mixing:** Implement a proper mixer with volume envelopes.
*   **Formats:** Add OGG support (via `stb_vorbis`) for smaller music files.

## 5. Arcade Integration (Priority: Specific)
*   **Protocol:** Reverse engineer the exact byte protocol for the specific hydraulic cylinders used in the AX DX cabinet. Update `cabinet_io.c`.
