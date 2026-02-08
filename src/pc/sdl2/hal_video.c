#include "pc/hal.h"
#include "pc/configfile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

static SDL_Window* sWindow = NULL;
static SDL_GLContext sContext = NULL;

bool HAL_Video_Init(VideoConfig* config) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        return false;
    }

    int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (config->fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    sWindow = SDL_CreateWindow(
        config->title ? config->title : "F-Zero X PC",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        config->width, config->height,
        flags
    );

    if (!sWindow) {
        printf("Window Error: %s\n", SDL_GetError());
        return false;
    }

    sContext = SDL_GL_CreateContext(sWindow);
    return true;
}

void HAL_Video_Shutdown(void) {
    if (sContext) SDL_GL_DeleteContext(sContext);
    if (sWindow) SDL_DestroyWindow(sWindow);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void HAL_Video_BeginFrame(void) {
    // Check for config changes
    static int sLastWidth = 0;
    static int sLastHeight = 0;
    static bool sLastFullscreen = false;
    static bool sLastVSync = true;

    if (sWindow) {
        // Fullscreen Toggle
        if (gConfig.fullscreen != sLastFullscreen) {
            SDL_SetWindowFullscreen(sWindow, gConfig.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
            sLastFullscreen = gConfig.fullscreen;
        }

        // VSync Toggle
        if (gConfig.vsync != sLastVSync) {
            SDL_GL_SetSwapInterval(gConfig.vsync ? 1 : 0);
            sLastVSync = gConfig.vsync;
        }

        // Resolution Change (Windowed Only)
        if (!gConfig.fullscreen && (gConfig.width != sLastWidth || gConfig.height != sLastHeight)) {
            SDL_SetWindowSize(sWindow, gConfig.width, gConfig.height);
            SDL_SetWindowPosition(sWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
            sLastWidth = gConfig.width;
            sLastHeight = gConfig.height;
        }
    }

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark Blue background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void HAL_Video_EndFrame(void) {
    SDL_GL_SwapWindow(sWindow);
}

void HAL_Video_SetFullscreen(bool enabled) {
    if (sWindow) {
        SDL_SetWindowFullscreen(sWindow, enabled ? SDL_WINDOW_FULLSCREEN : 0);
    }
}

void* HAL_Video_GetWindow(void) {
    return (void*)sWindow;
}
