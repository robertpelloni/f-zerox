#include <stdbool.h>
#include <stdint.h>
#include "pc/audio/music_player.h"
#include "pc/configfile.h"
#include <SDL2/SDL.h>
#include <stdio.h>

static uint8_t* sMusicBuffer = NULL;
static uint32_t sMusicLength = 0;
static uint32_t sMusicPos = 0;
static bool sMusicPlaying = false;

// Dynamic music intensity based on lap
static float sCurrentIntensity = 1.0f;
static float sTargetIntensity = 1.0f;

void Music_Init(void) {
    sMusicPlaying = false;
}

void Music_SetIntensity(float intensity) {
    // Sets the target multiplier for the music volume (e.g. 1.0 = normal, 1.5 = final lap hype)
    sTargetIntensity = intensity;
}

void Music_Play(const char* path) {
    if (sMusicBuffer) {
        SDL_FreeWAV(sMusicBuffer);
        sMusicBuffer = NULL;
    }

    SDL_AudioSpec spec;
    if (SDL_LoadWAV(path, &spec, &sMusicBuffer, &sMusicLength) == NULL) {
        printf("Music: Failed to load %s: %s\n", path, SDL_GetError());
        return;
    }

    // Ensure format matches our output (AUDIO_S16SYS, 44100, 2ch)
    // In a real engine we would use SDL_AudioCVT.
    // For now we assume the WAV is correct or let it sound weird (glitch aesthetic).

    sMusicPos = 0;
    sMusicPlaying = true;
    printf("Music: Playing %s\n", path);
}

void Music_Update(int16_t* buffer, int samples) {
    if (!sMusicPlaying || !sMusicBuffer) return;

    // Mix music into output buffer
    // buffer is int16_t[samples] (stereo interleaved?)
    // SDL_LoadWAV usually returns uint8_t raw bytes.
    // Assuming 16-bit stereo input for simplicity of mixing.

    int bytesNeeded = samples * sizeof(int16_t);
    int16_t* src = (int16_t*)(sMusicBuffer + sMusicPos);

    // Smoothly interpolate intensity (do not jump instantly)
    if (sCurrentIntensity < sTargetIntensity) {
        sCurrentIntensity += 0.005f;
        if (sCurrentIntensity > sTargetIntensity) sCurrentIntensity = sTargetIntensity;
    } else if (sCurrentIntensity > sTargetIntensity) {
        sCurrentIntensity -= 0.005f;
        if (sCurrentIntensity < sTargetIntensity) sCurrentIntensity = sTargetIntensity;
    }

    float masterVol = (float)gConfig.music_volume / 100.0f;

    for (int i = 0; i < samples; i++) {
        if (sMusicPos >= sMusicLength) {
            sMusicPos = 0; // Loop
            src = (int16_t*)sMusicBuffer;
        }

        // Safer clipping addition:
        float rawSample = (float)src[i] * 0.5f; // Base 50% max mix
        rawSample *= masterVol * sCurrentIntensity;

        int32_t mixed = buffer[i] + (int32_t)rawSample;
        if (mixed > 32767) mixed = 32767;
        if (mixed < -32768) mixed = -32768;
        buffer[i] = (int16_t)mixed;

        sMusicPos += sizeof(int16_t); // Advance by sample
    }
}
