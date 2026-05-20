# F-Zero X PC Port - Short Term Goals

## Netplay
- [x] **Interpolation:** Implement Lerp/Slerp for remote vehicles to smooth out UDP packet jitter.
- [x] **Prediction:** Implement Dead Reckoning (extrapolate position based on velocity).
- [ ] **Lobby:** Implement a handshake protocol to assign Player IDs (currently hardcoded/random).

## Visuals
- [ ] **Particles:** Add smoke trails for damaged machines.
- [x] **Lighting:** Implement basic directional lighting in `Fast3D` (currently flat/vertex colored).
- [ ] **Shadows:** Render a simple blob shadow under vehicles.

## Audio
- [ ] **Mixer:** Implement a software mixer to handle volume envelopes and multiple SFX channels properly.
- [ ] **Doppler:** Pitch shift engine sounds based on relative velocity to camera.

## Content
- [ ] **Cup Logic:** Implement the "Grand Prix" state machine (5 tracks, point tracking).
- [ ] **AI:** Improve collision avoidance (boids algorithm).
