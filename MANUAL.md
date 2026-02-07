# F-Zero X PC Port Manual

## Introduction
Welcome to the F-Zero X PC Port (Decompilation). This project aims to bring the classic N64 racer to modern platforms with native performance, enhanced visuals, and new features from F-Zero GX/AX.

## Features Overview
This port includes features that are either implemented or planned for future updates. Please refer to the Roadmap for development status.

### Graphics
- **Resolution:** Supports arbitrary resolutions including 4K and Ultrawide (21:9).
- **Widescreen:** Native widescreen support without stretching HUD elements.
- **High FPS:** Unlocked framerate (up to 360Hz) with interpolation for smooth gameplay.
- **Textures:** Support for high-resolution texture packs and model replacements.
- **Lighting:** Enhanced lighting engine with dynamic shadows and bloom (Planned).

### Audio
- **High Quality:** Replaced MIDI sequences with high-quality streaming audio (WAV/FLAC).
- **3D Spatial Audio:** Implements HRTF for realistic sound positioning (Planned).
- **Volume Control:** Independent sliders for Music, SFX, and Voice.

### Input
- **Controller Support:** Native support for XInput, DirectInput, and SDL2-compatible controllers.
- **Mapping:** Fully remappable controls for all actions.
- **Deadzone:** Adjustable analog stick deadzone and sensitivity.
- **Rumble:** Force feedback support for compatible controllers.

### Gameplay
- **Physics Modes:**
  - *Classic (X):* Original N64 physics. 100% accurate.
  - *Modern (GX):* Physics from F-Zero GX including Momentum Turbo Slide (MTS).
  - *Arcade (AX):* Physics from F-Zero AX arcade cabinets.
- **Story Mode:** A new mission-based campaign inspired by GX's story mode (Planned).
- **Track Editor:** Built-in track editor to create and share custom courses (Planned).

### Cheats & Mods
- **God Mode:** Infinite energy and invincibility.
- **Unlock All:** Instantly unlocks all machines, cups, and difficulties.
- **Debug Menu:** Access to internal game variables and state machine visualization.

## Troubleshooting
- **Performance:** If experiencing stuttering, try lowering the resolution or disabling V-Sync.
- **Input:** If your controller is not recognized, check the SDL2 mapping database.
- **Crashes:** Please report crashes on the GitHub repository with a reproduction log.

## Credits
- **Decompilation Team:** The original authors of the N64 decompilation.
- **PC Port Team:** Contributors to the PC platform layer.
- **Community:** Special thanks to the F-Zero speedrunning community.
