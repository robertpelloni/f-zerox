#include "pc/hal.h"
#include "pc/configfile.h"
#include "pc/audio/audio_engine.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

static SDL_AudioDeviceID sAudioDevice = 0;
// Internal reference to player speed (hacky, but effective for now)
extern float gPlayerSpeedRatio; // defined in hal_audio usually, but we'll expose it from game_loop
float gPlayerSpeedRatio = 0.0f; // Default

bool HAL_Audio_Init(AudioConfig* config) {
    SDL_AudioSpec want, have;
    SDL_memset(&want, 0, sizeof(want));

    want.freq = config->frequency;
    want.format = AUDIO_S16SYS;
    want.channels = config->channels;
    want.samples = config->samples;
    want.callback = NULL; // We will use SDL_QueueAudio

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("SDL Audio Init Failed: %s\n", SDL_GetError());
        return false;
    }

    sAudioDevice = SDL_OpenAudioDevice(NULL, 0, &want, &have, 0);
    if (sAudioDevice == 0) {
        printf("Failed to open audio device: %s\n", SDL_GetError());
        return false;
    }

    SDL_PauseAudioDevice(sAudioDevice, 0); // Start playing
    Music_Init();
    Music_Play("music/big_blue.wav"); // Placeholder autostart
    return true;
}

void HAL_Audio_Shutdown(void) {
    if (sAudioDevice != 0) {
        SDL_CloseAudioDevice(sAudioDevice);
        sAudioDevice = 0;
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void HAL_Audio_QueueSamples(const int16_t* samples, int count) {
    if (sAudioDevice != 0) {
        // Apply Volume
        int16_t* processed = (int16_t*)malloc(count * sizeof(int16_t));
        if (!processed) return;

        float volume = (gConfig.master_volume / 100.0f) * (gConfig.sfx_volume / 100.0f);
        if (volume > 1.0f) volume = 1.0f;
        if (volume < 0.0f) volume = 0.0f;

        for (int i = 0; i < count; i++) {
            processed[i] = (int16_t)(samples[i] * volume);
        }

        // Generate Engine Sound
        AudioEngine_Generate(processed, count, gPlayerSpeedRatio);

        // count is number of samples (int16), size in bytes is count * sizeof(int16)
        SDL_QueueAudio(sAudioDevice, processed, count * sizeof(int16_t));
        free(processed);
    }
}
