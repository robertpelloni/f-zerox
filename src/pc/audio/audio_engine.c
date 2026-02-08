#include "pc/audio/audio_engine.h"
#include <math.h>
#include <stdlib.h>

static float sPhase = 0.0f;

void AudioEngine_Generate(int16_t* buffer, int samples, float speed_ratio) {
    // Base frequency 100Hz, scales up to 400Hz with speed
    float freq = 100.0f + (speed_ratio * 300.0f);
    float sampleRate = 44100.0f;
    float step = freq / sampleRate;

    for (int i = 0; i < samples; i++) {
        // Sawtooth wave for rough engine sound
        float sample = (sPhase * 2.0f) - 1.0f;

        // Add some noise/jitter for texture
        float noise = ((float)rand() / RAND_MAX) * 0.1f;

        buffer[i] += (int16_t)((sample + noise) * 2000.0f); // Mix into existing buffer

        sPhase += step;
        if (sPhase > 1.0f) sPhase -= 1.0f;
    }
}
