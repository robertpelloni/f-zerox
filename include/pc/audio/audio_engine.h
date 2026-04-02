#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <stdint.h>

void Audio_PlaySound_Boost(void);
void Audio_PlaySound_Crash(void);

// Updates 3D audio listener (Camera) and multiple engine sources
// Called from Game_Update
void AudioEngine_Update3D(float camX, float camY, float camZ, float camVelX, float camVelY, float camVelZ);
void AudioEngine_AddRemoteEngine(float x, float y, float z, float velX, float velY, float velZ, float engineRPM);

// Generates procedural engine noise and mixes all active SFX channels
void AudioEngine_Generate(int16_t* buffer, int samples, float speed_ratio);

#endif
