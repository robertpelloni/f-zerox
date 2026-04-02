#include "pc/game_loop.h"
#include "pc/gfx/fast3d.h"
#include "pc/ultra64.h"
#include "pc/configfile.h"
#include "pc/assets.h"
#include "pc/assets/obj_loader.h"
#include "pc/track_system.h"
#include "pc/physics.h"
#include "pc/ai_system.h"
#include "pc/race_logic.h"
#include "pc/weapons.h"
#include "pc/network/network.h"
#include "pc/gfx/particles.h"
#include "pc/camera.h"
#include "pc/hal.h"
#include "pc/arcade/arcade_io.h"
#include "pc/ghost_recorder.h"
#include "pc/audio/audio_engine.h"
#include <stdio.h>
#include <SDL2/SDL_opengl.h>

// Game State
#define MAX_MACHINES 30
Vehicle gMachines[MAX_MACHINES];
Vehicle* gPlayerVehicle = &gMachines[0];
static OSContPad gInputState;

extern float gPlayerSpeedRatio;
extern Model* gBlueFalconModel;

// Vector Helper for velocity extraction
static void GetVelocityVector(Vehicle* v, float* vx, float* vy, float* vz) {
    float radYaw = v->yaw * 0.0174532925f;
    float radPitch = v->pitch * 0.0174532925f;
    *vx = sinf(radYaw) * cosf(radPitch) * v->velocity;
    *vy = sinf(radPitch) * v->velocity;
    *vz = -cosf(radYaw) * cosf(radPitch) * v->velocity;
}

void Game_Init(void) {
    printf("Game Loop: Initializing...\n");

    Physics_Init(gPlayerVehicle);

    for (int i = 1; i < MAX_MACHINES; i++) {
        Physics_Init(&gMachines[i]);
        gMachines[i].x = ((i % 3) - 1) * 30.0f;
        gMachines[i].z = i * 50.0f;
    }

    Track_Init();
    Race_Init();
    Weapons_Init();
    Particles_Init();
    Ghost_Init();
    Net_Init(7000);
}

static int sFrameCount = 0;

void Game_Update(void) {
    HAL_Input_Poll();
    HAL_Input_GetState(0, &gInputState);

    Physics_Update(gPlayerVehicle, &gInputState);
    Ghost_RecordFrame(gPlayerVehicle);

    if (gGhost.playing) {
        Ghost_Update(&gMachines[29]);
    } else {
        gMachines[29].y = -10000.0f;
    }

    sFrameCount++;

    Net_BroadcastPos(gPlayerVehicle);
    Net_Receive();
    Net_UpdateRemoteMachines(gMachines, MAX_MACHINES);

    Arcade_SendMotion(gPlayerVehicle->pitch, gPlayerVehicle->roll, 0.0f);

    for (int i = 1; i < MAX_MACHINES; i++) {
        if (i == 29 && gGhost.playing) continue;

        OSContPad aiPad = {0};
        AI_Update(&gMachines[i], &aiPad);
        Physics_Update(&gMachines[i], &aiPad);
    }

    Race_UpdateRankings(gMachines, MAX_MACHINES);
    gPlayerSpeedRatio = gPlayerVehicle->velocity / 300.0f;

    Weapons_Update();
    Particles_Update();

    // Spawn Engine Trails and Damage Smoke for all machines
    for (int i = 0; i < MAX_MACHINES; i++) {
        if (gMachines[i].velocity > 50.0f) {
             Particles_Spawn(gMachines[i].x, gMachines[i].y + 5.0f, gMachines[i].z + 20.0f, 0.2f, 0.5f, 1.0f, 5.0f);
        }

        if (gMachines[i].energy < 30.0f) {
            if ((rand() % 100) > (int)(gMachines[i].energy * 3.0f)) {
                float sx = gMachines[i].x + ((rand() % 10) - 5) * 0.5f;
                float sy = gMachines[i].y + 10.0f;
                float sz = gMachines[i].z + ((rand() % 10) - 5) * 0.5f;

                float colorR = 0.2f, colorG = 0.2f, colorB = 0.2f;
                if (gMachines[i].energy < 10.0f && (rand() % 5 == 0)) {
                    colorR = 1.0f; colorG = 0.5f; colorB = 0.0f;
                }

                Particles_Spawn(sx, sy, sz, colorR, colorG, colorB, 15.0f);
            }
        }
    }

    Camera_Update(gPlayerVehicle);

    // Audio 3D Update
    float camVx, camVy, camVz;
    GetVelocityVector(gPlayerVehicle, &camVx, &camVy, &camVz);

    // In chase cam, camera vel is roughly player vel
    AudioEngine_Update3D(gCamera.x, gCamera.y, gCamera.z, camVx, camVy, camVz);

    for (int i = 1; i < MAX_MACHINES; i++) {
        if (gMachines[i].y > -1000.0f) {
            float vx, vy, vz;
            GetVelocityVector(&gMachines[i], &vx, &vy, &vz);
            float rpm = gMachines[i].velocity / 300.0f;
            AudioEngine_AddRemoteEngine(gMachines[i].x, gMachines[i].y, gMachines[i].z, vx, vy, vz, rpm);
        }
    }
}

void Game_Render(void) {
    glLoadIdentity();
    Camera_Apply();
    Track_Render();

    for (int i = 0; i < MAX_MACHINES; i++) {
        if (gMachines[i].y < -5000.0f) continue;

        // Shadow rendering
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

        glPushMatrix();
        glTranslatef(gMachines[i].x, gMachines[i].y - 8.0f, gMachines[i].z);
        float pitch = gMachines[i].pitch;
        float roll = gMachines[i].roll;
        float yaw = gMachines[i].yaw;
        glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(roll, 0.0f, 0.0f, 1.0f);

        glBegin(GL_QUADS);
        glVertex3f(-10, 0, -20);
        glVertex3f(10, 0, -20);
        glVertex3f(10, 0, 20);
        glVertex3f(-10, 0, 20);
        glEnd();
        glPopMatrix();

        glEnable(GL_LIGHTING);

        glPushMatrix();
        glTranslatef(gMachines[i].x, gMachines[i].y, gMachines[i].z);

        glRotatef(-gMachines[i].yaw, 0.0f, 1.0f, 0.0f);
        glRotatef(gMachines[i].pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(gMachines[i].roll, 0.0f, 0.0f, 1.0f);

        if (i == 29 && gGhost.playing) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            GLfloat ghost_diffuse[] = {0.5f, 0.5f, 1.0f, 0.5f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ghost_diffuse);
        } else {
            GLfloat normal_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, normal_diffuse);
        }

        if (gBlueFalconModel) {
            Fast3D_ProcessDisplayList(gBlueFalconModel->dl);
        } else {
            Fast3D_ProcessDisplayList(blue_falcon_dl);
        }

        if (i == 29 && gGhost.playing) {
            glDisable(GL_BLEND);
        }

        glPopMatrix();
    }

    glDisable(GL_LIGHTING);
    Weapons_Render();
    Particles_Render();
    Fast3D_Render();
}

void Game_RunFrame(void) {
    Game_Update();
    Game_Render();
}
