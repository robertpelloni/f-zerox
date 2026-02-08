#include "pc/game_loop.h"
#include "pc/gfx/fast3d.h"
#include "pc/ultra64.h"
#include "pc/configfile.h"
#include "pc/assets.h"
#include <stdio.h>
#include <SDL2/SDL_opengl.h>

// Mock Game State variables
static int sFrameCounter = 0;

void Game_Init(void) {
    printf("Game Loop: Initializing...\n");
    // TODO: Initialize real game systems (Physics, Track, Machines)
}

void Game_RunFrame(void) {
    sFrameCounter++;

    // 1. Update Input (Read from HAL)
    // 2. Physics Update (Update positions based on gConfig physics settings)

    // 3. Render
    static float angle = 0.0f;
    angle += 1.0f;

    // Apply Camera (ModelView)
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -400.0f); // Zoom out to see the model (units are ~50-150)
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // Tilt slightly

    // Pass the Blue Falcon DL to the renderer
    Fast3D_ProcessDisplayList(blue_falcon_dl);
    Fast3D_Render();
}
