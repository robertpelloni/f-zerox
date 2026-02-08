#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <stdint.h>

// Generate procedural engine noise based on speed
void AudioEngine_Generate(int16_t* buffer, int samples, float speed_ratio);

#endif
