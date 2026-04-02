#include "pc/audio/audio_engine.h"
#include <math.h>
#include <stdlib.h>

#define MAX_AUDIO_CHANNELS 8
#define MAX_REMOTE_ENGINES 30
#define SAMPLE_RATE 44100

typedef enum {
    SFX_NONE,
    SFX_BOOST,
    SFX_CRASH
} SfxType;

typedef struct {
    bool active;
    SfxType type;
    int samplesLeft;
    int totalSamples;
    float phase;
} AudioChannel;

typedef struct {
    float x, y, z;
    float vx, vy, vz;
    float rpm;
} RemoteEngine;

static AudioChannel sChannels[MAX_AUDIO_CHANNELS];
static float sEnginePhase = 0.0f;

// 3D Audio State
static float sCamX, sCamY, sCamZ;
static float sCamVx, sCamVy, sCamVz;

static RemoteEngine sRemoteEngines[MAX_REMOTE_ENGINES];
static int sNumRemoteEngines = 0;

void AudioEngine_Update3D(float camX, float camY, float camZ, float camVelX, float camVelY, float camVelZ) {
    sCamX = camX; sCamY = camY; sCamZ = camZ;
    sCamVx = camVelX; sCamVy = camVelY; sCamVz = camVelZ;
    sNumRemoteEngines = 0; // Reset list for next frame
}

void AudioEngine_AddRemoteEngine(float x, float y, float z, float velX, float velY, float velZ, float engineRPM) {
    if (sNumRemoteEngines < MAX_REMOTE_ENGINES) {
        sRemoteEngines[sNumRemoteEngines].x = x;
        sRemoteEngines[sNumRemoteEngines].y = y;
        sRemoteEngines[sNumRemoteEngines].z = z;
        sRemoteEngines[sNumRemoteEngines].vx = velX;
        sRemoteEngines[sNumRemoteEngines].vy = velY;
        sRemoteEngines[sNumRemoteEngines].vz = velZ;
        sRemoteEngines[sNumRemoteEngines].rpm = engineRPM;
        sNumRemoteEngines++;
    }
}

void Audio_PlaySound_Boost(void) {
    for (int i = 0; i < MAX_AUDIO_CHANNELS; i++) {
        if (!sChannels[i].active) {
            sChannels[i].active = true;
            sChannels[i].type = SFX_BOOST;
            sChannels[i].samplesLeft = 22050; // 0.5s
            sChannels[i].totalSamples = 22050;
            sChannels[i].phase = 0.0f;
            return;
        }
    }
}

void Audio_PlaySound_Crash(void) {
    for (int i = 0; i < MAX_AUDIO_CHANNELS; i++) {
        if (!sChannels[i].active) {
            sChannels[i].active = true;
            sChannels[i].type = SFX_CRASH;
            sChannels[i].samplesLeft = 11025; // 0.25s
            sChannels[i].totalSamples = 11025;
            return;
        }
    }
}

void AudioEngine_Generate(int16_t* buffer, int samples, float speed_ratio) {
    // Engine Pitch Smoothing (Player)
    static float smoothedRatio = 0.0f;
    smoothedRatio += (speed_ratio - smoothedRatio) * 0.1f;

    float freq = 100.0f + (smoothedRatio * 300.0f);
    float step = freq / SAMPLE_RATE;
    float engineVol = 2000.0f + (smoothedRatio * 2000.0f);

    // State for remote engines
    static float remotePhases[MAX_REMOTE_ENGINES] = {0};

    // Calculate 3D Audio per block (not per sample for performance)
    float remoteFreq[MAX_REMOTE_ENGINES] = {0};
    float remoteVol[MAX_REMOTE_ENGINES] = {0};

    for (int i = 0; i < sNumRemoteEngines; i++) {
        RemoteEngine* re = &sRemoteEngines[i];

        // 1. Distance Attenuation (Volume)
        float dx = re->x - sCamX;
        float dy = re->y - sCamY;
        float dz = re->z - sCamZ;
        float distSq = dx*dx + dy*dy + dz*dz;

        if (distSq > 250000.0f) { // 500 units max distance
            remoteVol[i] = 0.0f;
            continue;
        }

        float dist = sqrtf(distSq);
        float volScale = 1.0f - (dist / 500.0f);
        if (volScale < 0.0f) volScale = 0.0f;
        remoteVol[i] = volScale * 1500.0f; // Max volume for remote

        // 2. Doppler Effect (Pitch)
        // Relative velocity along line of sight
        // vr = dot(V_source - V_listener, normalize(P_source - P_listener))
        float invDist = 1.0f / (dist + 0.0001f);
        float nx = dx * invDist;
        float ny = dy * invDist;
        float nz = dz * invDist;

        float relVx = re->vx - sCamVx;
        float relVy = re->vy - sCamVy;
        float relVz = re->vz - sCamVz;

        float vRel = relVx*nx + relVy*ny + relVz*nz;

        // Doppler formula: f' = f * (c / (c - v_source_towards))
        // Simplified: just add relative velocity as a pitch modifier
        // If moving towards us (vRel < 0), pitch goes up.
        // If moving away (vRel > 0), pitch goes down.

        float baseFreq = 150.0f + (re->rpm * 150.0f);
        float dopplerShift = vRel * -0.5f; // Adjust scalar for "sound speed"

        float shiftedFreq = baseFreq + dopplerShift;
        if (shiftedFreq < 50.0f) shiftedFreq = 50.0f; // Clamp low
        if (shiftedFreq > 800.0f) shiftedFreq = 800.0f; // Clamp high

        remoteFreq[i] = shiftedFreq;
    }

    for (int i = 0; i < samples; i++) {
        int32_t mix = 0;

        // 1. Procedural Engine (Player)
        float engineSample = (sEnginePhase * 2.0f) - 1.0f;
        float noise = ((float)rand() / RAND_MAX) * 0.1f;
        mix += (int32_t)((engineSample + noise) * engineVol);

        sEnginePhase += step;
        if (sEnginePhase > 1.0f) sEnginePhase -= 1.0f;

        // 2. Procedural Engines (Remote)
        for (int r = 0; r < sNumRemoteEngines; r++) {
            if (remoteVol[r] > 0.0f) {
                float rStep = remoteFreq[r] / SAMPLE_RATE;

                // Different waveform for AI (Sine or Triangle) to separate them from Player
                // Triangle wave
                float rSample = fabsf(fmodf(remotePhases[r] * 4.0f, 4.0f) - 2.0f) - 1.0f;
                mix += (int32_t)(rSample * remoteVol[r]);

                remotePhases[r] += rStep;
                if (remotePhases[r] > 1.0f) remotePhases[r] -= 1.0f;
            }
        }

        // 3. Mix Active SFX Channels
        for (int c = 0; c < MAX_AUDIO_CHANNELS; c++) {
            if (sChannels[c].active) {
                float progress = 1.0f - ((float)sChannels[c].samplesLeft / sChannels[c].totalSamples);

                if (sChannels[c].type == SFX_BOOST) {
                    float boostFreq = 800.0f + (progress * 2000.0f);
                    sChannels[c].phase += boostFreq / SAMPLE_RATE;
                    if (sChannels[c].phase > 1.0f) sChannels[c].phase -= 1.0f;

                    float boostSample = sinf(sChannels[c].phase * 2.0f * 3.14159f);
                    float env = 1.0f;
                    if (progress > 0.8f) env = (1.0f - progress) * 5.0f;

                    mix += (int32_t)(boostSample * 5000.0f * env);
                }
                else if (sChannels[c].type == SFX_CRASH) {
                    float crashSample = ((float)rand() / RAND_MAX) * 2.0f - 1.0f;
                    float env = 1.0f - progress;
                    mix += (int32_t)(crashSample * 15000.0f * env);
                }

                sChannels[c].samplesLeft--;
                if (sChannels[c].samplesLeft <= 0) {
                    sChannels[c].active = false;
                }
            }
        }

        // 4. Hard Clip and Write
        if (mix > 32767) mix = 32767;
        if (mix < -32768) mix = -32768;

        int32_t finalOut = buffer[i] + mix;
        if (finalOut > 32767) finalOut = 32767;
        if (finalOut < -32768) finalOut = -32768;

        buffer[i] = (int16_t)finalOut;
    }
}
