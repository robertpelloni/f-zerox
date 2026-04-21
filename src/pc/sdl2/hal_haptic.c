#include "pc/hal_haptic.h"
#include <SDL2/SDL.h>
#include <stdio.h>

static SDL_Haptic* sHaptic = NULL;
static SDL_Joystick* sJoystick = NULL;
static int sRumbleEffectId = -1;

bool HAL_Haptic_Init(void) {
    if (SDL_Init(SDL_INIT_HAPTIC) < 0) {
        printf("Haptic Init Failed: %s\n", SDL_GetError());
        return false;
    }

    // Try to open first haptic device
    // Ideally we match this with the Controller API selection
    if (SDL_NumHaptics() > 0) {
        sHaptic = SDL_HapticOpen(0);
        if (sHaptic) {
            printf("Haptic: Opened device 0 (%s)\n", SDL_HapticName(0));

            // Initialize Simple Rumble
            if (SDL_HapticRumbleInit(sHaptic) != 0) {
                printf("Haptic: Rumble Init Failed: %s\n", SDL_GetError());
            }
        }
    }
    return true;
}

void HAL_Haptic_Rumble(float strength, uint32_t duration) {
    if (sHaptic) {
        SDL_HapticRumblePlay(sHaptic, strength, duration);
    }
}

void HAL_Haptic_SetConstantForce(float x) {
    // Constant force requires more complex SDL_HapticEffect setup
    // Placeholder for now: use Rumble as poor man's FFB
    // Ideally: Create SDL_HAPTIC_CONSTANT effect, update direction/magnitude
    (void)x;
}

void HAL_Haptic_Shutdown(void) {
    if (sHaptic) {
        SDL_HapticClose(sHaptic);
        sHaptic = NULL;
    }
}
