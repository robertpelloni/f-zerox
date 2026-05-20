#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

void AudioEngine_Init(void);

// Generate procedural engine noise based on speed
void AudioEngine_Generate(int16_t* buffer, int samples, float speed_ratio);

// Play a simple procedural SFX
void AudioEngine_PlaySFX(float freq, float vol);

// Update 3D audio for remote emitters (Doppler and attenuation)
void AudioEngine_Update3D(float listener_x, float listener_y, float listener_z,
                          float listener_vx, float listener_vy, float listener_vz,
                          float* emitter_x, float* emitter_y, float* emitter_z,
                          float* emitter_vx, float* emitter_vy, float* emitter_vz,
                          int num_emitters);

#endif
