#include "../../include/pc/hal.h"
#include <SDL2/SDL.h>

void HAL_Input_Poll(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            // TODO: Signal global exit
        }
        // TODO: Map keys to N64 controller state
    }
}
