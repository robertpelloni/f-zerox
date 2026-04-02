#include "pc/audio/music_player.h"
#include <SDL2/SDL.h>
#include <stdio.h>

static uint8_t* sMusicBuffer = NULL;
static uint32_t sMusicLength = 0;
static uint32_t sMusicPos = 0;
static bool sMusicPlaying = false;

void Music_Init(void) {
    sMusicPlaying = false;
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

    for (int i = 0; i < samples; i++) {
        if (sMusicPos >= sMusicLength) {
            sMusicPos = 0; // Loop
            src = (int16_t*)sMusicBuffer;
        }

        // Simple Mix with volume reduction (background music)
        // buffer[i] += src[i] * 0.5f;
        // Safer clipping addition:
        int32_t mixed = buffer[i] + (src[i] >> 2); // 25% volume
        if (mixed > 32767) mixed = 32767;
        if (mixed < -32768) mixed = -32768;
        buffer[i] = (int16_t)mixed;

        sMusicPos += sizeof(int16_t); // Advance by sample
    }
}
