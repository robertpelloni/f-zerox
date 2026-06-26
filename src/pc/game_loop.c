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
#include "pc/audio/audio_engine.h"
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

    // Hook up 3D Doppler audio for remote machines
    // For simplicity, we query a few global slots (e.g. slot 1-4) or mock it.
    // In a full implementation, we'd loop over active `gMachines`.
    extern Vehicle gMachines[30];
    float em_x[4], em_y[4], em_z[4];
    float em_vx[4], em_vy[4], em_vz[4];
    int em_count = 0;

    for (int i = 1; i < 5 && i < 30; i++) {
        if (gMachines[i].y > -9000.0f) { // Active check
            em_x[em_count] = gMachines[i].x;
            em_y[em_count] = gMachines[i].y;
            em_z[em_count] = gMachines[i].z;

            // Reconstruct velocity vector approximation for Doppler
            float yaw_rad = gMachines[i].yaw * 0.017453f;
            em_vx[em_count] = sinf(yaw_rad) * gMachines[i].velocity;
            em_vy[em_count] = 0.0f;
            em_vz[em_count] = -cosf(yaw_rad) * gMachines[i].velocity;
            em_count++;
        }
    }

    if (em_count > 0) {
        float p_yaw = gPlayerVehicle.yaw * 0.017453f;
        AudioEngine_Update3D(
            gPlayerVehicle.x, gPlayerVehicle.y, gPlayerVehicle.z,
            sinf(p_yaw) * gPlayerVehicle.velocity, 0.0f, -cosf(p_yaw) * gPlayerVehicle.velocity,
            em_x, em_y, em_z,
            em_vx, em_vy, em_vz,
            em_count
        );
    }

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

    // 3.5 Update Dynamic Lighting (Simulate a moving sun/light source)
    static float sLightAngle = 0.0f;
    sLightAngle += 0.005f;
    gLightDirection[0] = cosf(sLightAngle) * 0.8f;
    gLightDirection[1] = 1.0f;
    gLightDirection[2] = sinf(sLightAngle) * 0.8f;
    Fast3D_UpdateLighting();

    // Render Blob Shadow
    glPushMatrix();
    // Find the track surface normal below the vehicle to orient the shadow
    TrackSurfaceInfo surf = Track_GetInfoAt(gPlayerVehicle.x, gPlayerVehicle.y, gPlayerVehicle.z);

    // Translate slightly above ground
    glTranslatef(gPlayerVehicle.x, gPlayerVehicle.y - 15.0f, gPlayerVehicle.z);

    // Rotate to match surface normal using cross product approximation for simple pitch/roll
    // The default up vector is (0, 1, 0)
    // We want the Y axis to align with surf.nx, surf.ny, surf.nz
    if (surf.isValid) {
        float dot = surf.ny; // Dot product with (0,1,0)
        float angle = acosf(dot) * 180.0f / 3.14159f;

        // Axis of rotation is cross product of (0,1,0) and normal
        float axisX = surf.nz;
        float axisY = 0.0f;
        float axisZ = -surf.nx;

        if (angle > 0.01f) {
            glRotatef(angle, axisX, axisY, axisZ);
        }
    }

    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Face flat on the local plane

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
