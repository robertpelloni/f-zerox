# F-Zero X PC Port - Project Vision

## The Ultimate F-Zero Experience
This project is not just a port; it is a **Reimagining**. We are taking the decompiled N64 codebase and wrapping it in a modern, flexible PC engine that preserves the original's perfect controls while adding the content and features of its successors.

## Core Pillars

### 1. Accuracy First (The "X" Core)
The physics and handling must be 100% identical to the N64 original when "Classic Mode" is selected. We achieve this by compiling the original C code (`math_utils.c`, `game_physics.c`) directly into the PC executable, stubbing only the hardware calls.

### 2. Feature Parity (The "GX" Expansion)
F-Zero GX set the standard for content. We aim to implement:
*   **Story Mode:** Hardcoded missions with scripted events.
*   **Garage:** Deep customization of machine colors, emblems, and parts.
*   **Visuals:** Porting the "Cyberpunk/Plasma" aesthetic of GX to the X engine via shaders (Bloom, Distortion).

### 3. Modernization (The "Redout" Flair)
We learn from modern racers:
*   **VR Support:** Cockpit view with head tracking.
*   **Motion Blur:** Sense of speed enhancements.
*   **Online Play:** Rollback netcode for 30-player chaos.

## Architecture
*   **Language:** C99 (for compatibility with N64 SDK headers).
*   **Backend:** SDL2 (Window/Input/Audio) + OpenGL 3.3 (Rendering).
*   **UI:** Nuklear (Immediate Mode GUI) for seamless in-game configuration.
*   **Build System:** Make (native PC target).

## User Experience
The user launches `fzerox_pc`. They are greeted by a sleek, modern dashboard overlaying the classic title screen. They can tweak resolution, bind controls, and select "Netplay" to join a server immediately. The friction of emulators (plugins, rom hacks) is gone.
