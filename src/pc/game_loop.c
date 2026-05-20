#include <stdlib.h>
#include <math.h>
#include "pc/game_loop.h"
#include "pc/gfx/fast3d.h"
#include "pc/ultra64.h"
#include "pc/configfile.h"
#include "pc/assets.h"
#include "pc/track_system.h"
#include "pc/physics.h"
#include "pc/camera.h"
#include "pc/gfx/particles.h"
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

    // Particle: Smoke trails for damage
    if (gPlayerVehicle.energy < 30.0f) {
        // Spawn black smoke
        Particles_Spawn(gPlayerVehicle.x, gPlayerVehicle.y, gPlayerVehicle.z, 0.2f, 0.2f, 0.2f, 8.0f);

        // Spawn fire if very low
        if (gPlayerVehicle.energy < 10.0f && (rand() % 100) > 50) {
            Particles_Spawn(gPlayerVehicle.x, gPlayerVehicle.y, gPlayerVehicle.z, 1.0f, 0.4f, 0.0f, 12.0f);
        }
    }

    // 3. Camera Update
    Camera_Update(&gPlayerVehicle);

    // 4. Render
    glLoadIdentity();

    // Apply Camera Transform
    Camera_Apply();

    // Draw Track
    Track_Render();

    // Render Blob Shadow
    glPushMatrix();
    // Translate slightly above ground (assuming y is altitude, we should query TrackSurfaceInfo)
    // For now, render it just below the vehicle's center.
    glTranslatef(gPlayerVehicle.x, gPlayerVehicle.y - 15.0f, gPlayerVehicle.z);
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Face flat on the ground

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE); // Don't write to depth buffer

    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Semi-transparent black

    // Draw an octagon as a blob shadow approximation
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f); // Center
    for (int i = 0; i <= 8; i++) {
        float angle = i * (3.14159f * 2.0f / 8.0f);
        glVertex3f(cosf(angle) * 20.0f, sinf(angle) * 20.0f, 0.0f);
    }
    glEnd();

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // Draw Player Machine
    glPushMatrix();
    glTranslatef(gPlayerVehicle.x, gPlayerVehicle.y, gPlayerVehicle.z);
    glRotatef(-gPlayerVehicle.yaw, 0.0f, 1.0f, 0.0f); // Rotate model to match yaw
    // Tilt for banking/turning?
    Fast3D_ProcessDisplayList(blue_falcon_dl);
    glPopMatrix();

    // Render & Update Particles
    Particles_Update();
    Particles_Render();

    Fast3D_Render();
}
