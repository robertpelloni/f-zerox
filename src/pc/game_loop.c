#include "pc/game_loop.h"
#include "pc/gfx/fast3d.h"
#include "pc/ultra64.h"
#include "pc/configfile.h"
#include <stdio.h>

// Mock Game State variables
static int sFrameCounter = 0;

void Game_Init(void) {
    printf("Game Loop: Initializing...\n");
    // TODO: Initialize real game systems (Physics, Track, Machines)
    // func_80068B20(); // N64 System Init
    // func_80069698(); // State Machine Init
}

void Game_RunFrame(void) {
    sFrameCounter++;

    // 1. Update Input (Read from HAL)
    // 2. Physics Update (Update positions based on gConfig physics settings)

    // 3. Render
    // Create a mock Display List
    Gfx* dl = NULL; // Real game would allocate Gfx buffer

    // Simulate processing the display list
    // This calls our Fast3D renderer to draw the spinning triangle
    Fast3D_ProcessDisplayList(dl);
    Fast3D_Render();
}
