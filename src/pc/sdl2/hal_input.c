#include "pc/hal.h"
#include "pc/ui/ui.h"
#include "pc/configfile.h"
#include "pc/controller_api.h"
#include <SDL2/SDL.h>
#include <string.h>

static OSContPad sControllerState[4];

void HAL_Input_Poll(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        UI_HandleEvent(&event);
        if (event.type == SDL_QUIT) {
            exit(0); // Simple exit for now
        }

        // Handle Binding in UI
        if (event.type == SDL_KEYDOWN) {
            if (Controller_IsBinding()) {
                Controller_HandleBind(event.key.keysym.scancode);
            }
        }
    }

    // If UI is using input, don't update game controller state from keyboard
    if (UI_IsCapturingInput()) {
        memset(sControllerState, 0, sizeof(sControllerState));
        return;
    }

    // If UI is using input, don't update game controller state from keyboard
    if (UI_IsCapturingInput()) {
        memset(sControllerState, 0, sizeof(sControllerState));
        return;
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    OSContPad* pad = &sControllerState[0];

    // Reset state
    memset(pad, 0, sizeof(OSContPad));

    // Map bindings to OSContPad
    // We iterate over our known buttons (0-15)
    // 0: A, 1: B, 2: Z, 3: Start, 4: U, 5: D, 6: L, 7: R
    // 8: L_Trig, 9: R_Trig, 10: C-U, 11: C-D, 12: C-L, 13: C-R

    // Button Bitmasks (matching ultra64.h)
    const uint16_t masks[] = {
        CONT_A, CONT_B, CONT_Z, CONT_START,
        CONT_UP, CONT_DOWN, CONT_LEFT, CONT_RIGHT,
        CONT_L, CONT_R,
        CONT_C, CONT_C, CONT_C, CONT_C // C-Buttons handled specially for stick?
    };

    // Simple boolean checks for now
    if (state[gConfig.binds[0][0].scancode]) pad->button |= CONT_A;
    if (state[gConfig.binds[1][0].scancode]) pad->button |= CONT_B;
    if (state[gConfig.binds[2][0].scancode]) pad->button |= CONT_Z;
    if (state[gConfig.binds[3][0].scancode]) pad->button |= CONT_START;

    if (state[gConfig.binds[4][0].scancode]) pad->button |= CONT_UP;
    if (state[gConfig.binds[5][0].scancode]) pad->button |= CONT_DOWN;
    if (state[gConfig.binds[6][0].scancode]) pad->button |= CONT_LEFT;
    if (state[gConfig.binds[7][0].scancode]) pad->button |= CONT_RIGHT;

    if (state[gConfig.binds[8][0].scancode]) pad->button |= CONT_L;
    if (state[gConfig.binds[9][0].scancode]) pad->button |= CONT_R;

    // Analog Stick Emulation from D-Pad (if mapped to keys)
    if (pad->button & CONT_UP)    pad->stick_y = 127;
    if (pad->button & CONT_DOWN)  pad->stick_y = -127;
    if (pad->button & CONT_LEFT)  pad->stick_x = -127;
    if (pad->button & CONT_RIGHT) pad->stick_x = 127;
}

void HAL_Input_GetState(int controller_index, OSContPad* pad) {
    if (controller_index >= 0 && controller_index < 4) {
        *pad = sControllerState[controller_index];
    } else {
        memset(pad, 0, sizeof(OSContPad));
    }
}
