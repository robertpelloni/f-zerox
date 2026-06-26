# F-Zero X PC Port - Short Term Goals

## Netplay
- [x] **Interpolation:** Implement Lerp/Slerp for remote vehicles to smooth out UDP packet jitter.
- [x] **Prediction:** Implement Dead Reckoning (extrapolate position based on velocity).
- [x] **Lobby:** Implement a handshake protocol to assign Player IDs (currently hardcoded/random).
- [x] **Lobby UI:** Add lobby UI to show connected players before starting a race.

## Visuals
- [x] **Particles:** Add smoke trails for damaged machines.
- [x] **Lighting:** Implement basic directional lighting in `Fast3D` (currently flat/vertex colored).
- [x] **Shadows:** Render a simple blob shadow under vehicles.
- [x] **Bloom:** Implement a bloom shader for glowing track elements.

## Audio
- [x] **Mixer:** Implement a software mixer to handle volume envelopes and multiple SFX channels properly.
- [x] **Doppler:** Pitch shift engine sounds based on relative velocity to camera.
- [ ] **Dynamic Music:** Transition music intensity based on lap number.

## Content
- [x] **Cup Logic:** Implement the "Grand Prix" state machine (5 tracks, point tracking).
- [x] **AI:** Improve collision avoidance (boids algorithm).
- [ ] **Track Importer:** Create an importer script/logic for GX `.gma`/`.tpl` track formats.

## Decompilation
- [x] **Math Utils:** Decompile all 100% logic in `src/math_utils.c`.

## Engine / Shiftability
- [x] **Data:** Decouple hardcoded IDO pointers into linkable symbols.
