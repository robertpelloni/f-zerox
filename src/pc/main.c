#include "pc/hal.h"
#include "pc/ui/ui.h"
#include "pc/configfile.h"
#include "pc/game_loop.h"
#include "pc/game_state.h"
#include "pc/gfx/fast3d.h"
#include "pc/gfx/post_processing.h"
#include "pc/arcade/arcade_io.h"
#include "pc/assets.h" // Assets_Init
#include "pc/hal_haptic.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    printf("F-Zero X PC Port (Shell)\n");

    Config_SetDefaults();
    Config_Load("fzerox_pc.bin");

    VideoConfig videoConfig = {
        .width = gConfig.width,
        .height = gConfig.height,
        .fullscreen = gConfig.fullscreen,
        .title = "F-Zero X (Decompilation)"
    };

    // Request Accumulation Buffer for Motion Blur
    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE, 8);

    if (!HAL_Video_Init(&videoConfig)) {
        printf("Failed to init video.\n");
        return 1;
    }

    AudioConfig audioConfig = {
        .frequency = 44100,
        .channels = 2,
        .samples = 1024
    };

    if (!HAL_Audio_Init(&audioConfig)) {
        printf("Failed to init audio.\n");
        HAL_Video_Shutdown();
        return 1;
    }

    if (!UI_Init()) {
        printf("Failed to init UI.\n");
    }

    // Initialize Game Engine (Physics, Graphics Parser)
    Fast3D_Init();
    Assets_Init(); // Generate Textures
    Post_Init(gConfig.width, gConfig.height);
    Arcade_Init("/dev/ttyUSB0"); // Default port, configurable later
    HAL_Haptic_Init();
    Game_Init();
    GameState_Init();

    printf("Initialization successful. Running main loop...\n");

    bool running = true;
    OSContPad pad;

    while (running) {
        HAL_Input_Poll();
        HAL_Input_GetState(0, &pad);

        if (pad.button & CONT_A) {
            printf("Button A Pressed!\n");
        }

        HAL_Video_BeginFrame();

        // 1. Update Game State (Logic)
        GameState_Update();

        // 2. Render 3D Scene (State Dependent)
        // Note: GameState_Render calls Fast3D internally
        GameState_Render();

        // 3. Run Race Logic only if in RACE state
        // Refactor: Game_RunFrame currently does Update+Render.
        // We need to call Game_RunFrame ONLY when in STATE_RACE.
        extern GameStateID sCurrentState; // Hack for now, expose getter later
        // Actually, let GameState_Update call Game_RunFrame if needed.

        // 4. Apply Post-Processing
        Post_Process();

        // 5. Render UI Overlay on top
        UI_Render();
        HAL_Video_EndFrame();

        // Limit FPS (simple delay)
        HAL_Delay(16);
    }

    Config_Save("fzerox_pc.bin");
    HAL_Haptic_Shutdown();
    Arcade_Shutdown();
    Fast3D_Shutdown();
    UI_Shutdown();
    HAL_Audio_Shutdown();
    HAL_Video_Shutdown();
    return 0;
}
