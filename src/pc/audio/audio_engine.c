#include "pc/audio/audio_engine.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_CHANNELS 8
#define SAMPLE_RATE 44100
#define SPEED_OF_SOUND 343.0f // m/s

typedef struct {
    bool active;
    float volume; // 0.0 to 1.0
    float pitch;  // 1.0 is normal
    float pan;    // -1.0 left to 1.0 right

    // Procedural synthesis state
    float phase;
    float base_freq;

    // ADSR Envelope
    float attack_time;
    float decay_time;
    float sustain_level;
    float release_time;
    float env_time;
    bool releasing;

} AudioChannel;

static AudioChannel sChannels[MAX_CHANNELS];

void AudioEngine_Init(void) {
    memset(sChannels, 0, sizeof(sChannels));
}

// Generate procedural engine noise based on speed
void AudioEngine_Generate(int16_t* buffer, int samples, float speed_ratio) {
    // Basic engine sound generation (sawtooth wave) for channel 0
    AudioChannel* ch = &sChannels[0];
    ch->active = true;
    ch->base_freq = 100.0f + (speed_ratio * 200.0f); // Engine pitch goes up with speed
    ch->volume = 0.5f;

    for (int i = 0; i < samples; i++) {
        float sample = 0.0f;
        for (int c = 0; c < MAX_CHANNELS; c++) {
            if (sChannels[c].active) {
                // Generate a sawtooth wave
                float phase_inc = (sChannels[c].base_freq * sChannels[c].pitch) / SAMPLE_RATE;
                sChannels[c].phase += phase_inc;
                if (sChannels[c].phase >= 1.0f) sChannels[c].phase -= 1.0f;

                float wave = (sChannels[c].phase * 2.0f) - 1.0f;

                // Mix in channel
                sample += wave * sChannels[c].volume;
            }
        }

        // Clip
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;

        // Convert to 16-bit
        // Mix into existing buffer safely to avoid overwriting music
        int32_t mixed = buffer[i] + (int32_t)(sample * 32767.0f);
        if (mixed > 32767) mixed = 32767;
        if (mixed < -32768) mixed = -32768;
        buffer[i] = (int16_t)mixed;
    }
}

// Play a simple procedural SFX on a free channel
void AudioEngine_PlaySFX(float freq, float vol) {
    for (int i = 1; i < MAX_CHANNELS; i++) {
        if (!sChannels[i].active) {
            sChannels[i].active = true;
            sChannels[i].base_freq = freq;
            sChannels[i].pitch = 1.0f;
            sChannels[i].volume = vol;
            sChannels[i].phase = 0.0f;
            break;
        }
    }
}

void AudioEngine_Update3D(float listener_x, float listener_y, float listener_z,
                          float listener_vx, float listener_vy, float listener_vz,
                          float* emitter_x, float* emitter_y, float* emitter_z,
                          float* emitter_vx, float* emitter_vy, float* emitter_vz,
                          int num_emitters) {

    // Assign an emitter to a channel (simplified: 1 to 1 mapping starting at channel 1)
    for (int i = 0; i < num_emitters && (i + 1) < MAX_CHANNELS; i++) {
        AudioChannel* ch = &sChannels[i + 1];
        ch->active = true;
        ch->base_freq = 150.0f; // Default remote engine base frequency

        // Distance Attenuation
        float dx = emitter_x[i] - listener_x;
        float dy = emitter_y[i] - listener_y;
        float dz = emitter_z[i] - listener_z;
        float distSq = dx*dx + dy*dy + dz*dz;
        float dist = sqrtf(distSq);

        // Inverse distance clamping
        if (dist < 10.0f) dist = 10.0f;
        ch->volume = 100.0f / (dist * dist);
        if (ch->volume > 1.0f) ch->volume = 1.0f;

        // Doppler Effect
        // Relative velocity along the line of sight
        float nx = dx / dist;
        float ny = dy / dist;
        float nz = dz / dist;

        float v_listener_proj = listener_vx * nx + listener_vy * ny + listener_vz * nz;
        float v_emitter_proj = emitter_vx[i] * nx + emitter_vy[i] * ny + emitter_vz[i] * nz;

        // Doppler factor: f' = f * (c + v_r) / (c + v_s)
        // Where v_r is velocity of receiver relative to medium (positive if moving towards source)
        // v_s is velocity of source relative to medium (positive if moving away from receiver)
        // Note: Our projections are positive if moving towards each other based on vector math?
        // Let's standardise: vector from listener to emitter.
        // v_listener_proj: positive means listener moving towards emitter.
        // v_emitter_proj: positive means emitter moving AWAY from listener.

        float doppler_factor = (SPEED_OF_SOUND + v_listener_proj) / (SPEED_OF_SOUND + v_emitter_proj);

        // Clamp to prevent crazy pitches if speeds exceed speed of sound
        if (doppler_factor < 0.1f) doppler_factor = 0.1f;
        if (doppler_factor > 3.0f) doppler_factor = 3.0f;

        ch->pitch = doppler_factor;
    }
}
