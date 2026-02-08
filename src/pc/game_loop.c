#include "pc/game_loop.h"
#include "pc/gfx/fast3d.h"
#include "pc/ultra64.h"
#include "pc/configfile.h"
#include "pc/assets.h"
#include "pc/track_system.h"
#include "pc/physics.h"
#include "pc/camera.h"
#include "pc/hal.h"
#include <stdio.h>
#include <SDL2/SDL_opengl.h>

// Game State
Vehicle gPlayerVehicle; // Exposed for HUD
static OSContPad gInputState;

// Shared with HAL Audio
extern float gPlayerSpeedRatio;

void Game_Init(void) {
    printf("Game Loop: Initializing...\n");
    Physics_Init(&gPlayerVehicle);
    Track_Init();
}

void Game_RunFrame(void) {
    // 1. Update Input
    HAL_Input_Poll();
    HAL_Input_GetState(0, &gInputState);

    // 2. Physics Update
    Physics_Update(&gPlayerVehicle, &gInputState);

    // Update Audio Pitch
    gPlayerSpeedRatio = gPlayerVehicle.velocity / 300.0f; // Normalize max speed ~1200 kph

    // 3. Camera Update
    Camera_Update(&gPlayerVehicle);

    // 4. Render
    glLoadIdentity();

    // Apply Camera Transform
    Camera_Apply();

    // Draw Track
    Track_Render();

    // Draw Player Machine
    glPushMatrix();
    glTranslatef(gPlayerVehicle.x, gPlayerVehicle.y, gPlayerVehicle.z);
    glRotatef(-gPlayerVehicle.yaw, 0.0f, 1.0f, 0.0f); // Rotate model to match yaw
    // Tilt for banking/turning?
    Fast3D_ProcessDisplayList(blue_falcon_dl);
    glPopMatrix();

    Fast3D_Render();
}
