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
#include "pc/race_logic.h"
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
    Race_Init();
}

void Game_RunFrame(void) {
    // 1. Update Input
    HAL_Input_Poll();
    HAL_Input_GetState(0, &gInputState);


    // 2. Physics Update
    Physics_Update(&gPlayerVehicle, &gInputState);

    // Update Ghost System
    extern int gGhostState; // Assuming definition somewhere, we can just stub or declare
    // Actually let's just do Netplay

    // Netplay Update (Supervisor Priority)
    extern void Net_BroadcastPos(Vehicle* v);
    extern void Net_Receive(void);
    extern void Net_UpdateRemoteMachines(Vehicle* machines, int max_machines);
    extern Vehicle gMachines[30]; // External array from weapons/race_logic

    Net_BroadcastPos(&gPlayerVehicle);
    Net_Receive();
    Net_UpdateRemoteMachines(gMachines, 30);

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

    // 2.5 Race Logic Update
    // We update rankings by providing the active machines.
    // For now, let's construct an array with the player at index 0 and others if active.
    Vehicle activeMachines[30];
    activeMachines[0] = gPlayerVehicle;
    int numActive = 1;

    for (int i = 1; i < 30; i++) {
        if (gMachines[i].y > -9000.0f) {
            activeMachines[numActive] = gMachines[i];
            numActive++;
        }
    }

    Race_UpdateRankings(activeMachines, numActive);
    Race_Update();

    // 3. Camera Update
    Camera_Update(&gPlayerVehicle);

    // 4. Render
    glLoadIdentity();

    // Apply Camera Transform
    Camera_Apply();

    // Update dynamic lighting
    Fast3D_SetLightDirection(gConfig.light_dir[0], gConfig.light_dir[1], gConfig.light_dir[2]);
    Fast3D_SetLightColor(gConfig.light_color[0], gConfig.light_color[1], gConfig.light_color[2]);

    // Draw Track
    Track_Render();

    // Render Blob Shadow
    glPushMatrix();
    // Translate slightly above ground (assuming y is altitude, we should query TrackSurfaceInfo)
    // For now, render it just below the vehicle's center.
    glTranslatef(gPlayerVehicle.x, gPlayerVehicle.y - 15.0f, gPlayerVehicle.z);

    // Align blob shadow with vehicle's up and forward vectors using lookAt logic conceptually
    // Create a rotation matrix manually from forward and up vectors
    float up[3] = {gPlayerVehicle.up[0], gPlayerVehicle.up[1], gPlayerVehicle.up[2]};
    float fwd[3] = {gPlayerVehicle.forward[0], gPlayerVehicle.forward[1], gPlayerVehicle.forward[2]};

    // Calculate right vector = cross(fwd, up)
    float right[3] = {
        fwd[1]*up[2] - fwd[2]*up[1],
        fwd[2]*up[0] - fwd[0]*up[2],
        fwd[0]*up[1] - fwd[1]*up[0]
    };

    // Normalize right
    float rLen = sqrtf(right[0]*right[0] + right[1]*right[1] + right[2]*right[2]);
    if (rLen > 0.0f) {
        right[0] /= rLen; right[1] /= rLen; right[2] /= rLen;
    }

    // Re-orthogonalize fwd = cross(up, right)
    fwd[0] = up[1]*right[2] - up[2]*right[1];
    fwd[1] = up[2]*right[0] - up[0]*right[2];
    fwd[2] = up[0]*right[1] - up[1]*right[0];

    // Construct 4x4 matrix for OpenGL
    float matrix[16] = {
        right[0], right[1], right[2], 0.0f,
        fwd[0],   fwd[1],   fwd[2],   0.0f, // Use fwd as "up" for the polygon on the ground
        up[0],    up[1],    up[2],    0.0f, // Use up as the normal "Z"
        0.0f,     0.0f,     0.0f,     1.0f
    };
    glMultMatrixf(matrix);

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
