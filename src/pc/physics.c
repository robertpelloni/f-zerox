#include "pc/physics.h"
#include "pc/configfile.h"
#include "pc/track_system.h"
#include "pc/track_data.h"
#include "pc/audio/audio_engine.h"
#include "pc/hal_haptic.h"
#include "pc/gfx/particles.h"
#include <math.h>
#include <stdlib.h>

#define DEGTORAD 0.0174532925f

extern Vehicle gMachines[30];

void Physics_Init(Vehicle* v) {
    v->x = 0.0f;
    v->y = 20.0f;
    v->z = 0.0f;
    v->velocity = 0.0f;

    v->up[0] = 0.0f; v->up[1] = 1.0f; v->up[2] = 0.0f;
    v->forward[0] = 0.0f; v->forward[1] = 0.0f; v->forward[2] = -1.0f;
    v->right[0] = 1.0f; v->right[1] = 0.0f; v->right[2] = 0.0f;

    v->yaw = 0.0f;
    v->pitch = 0.0f;
    v->roll = 0.0f;
    v->energy = 100.0f;
    v->boost_active = false;
    v->spin_timer = 0;
    v->side_atk_timer = 0;
    v->speed_kph = 0.0f;
}

void Physics_Update(Vehicle* v, OSContPad* pad) {
    // 1. Acceleration
    float accel = 0.0f;
    v->boost_active = false;

    if (pad->button & CONT_B) {
        if (v->energy > 0.0f) {
            accel = 50.0f;
            v->boost_active = true;
            v->energy -= 0.5f;
            if (((int)v->energy % 20) == 0) {
                Audio_PlaySound_Boost();
                HAL_Haptic_Rumble(0.3f, 200);
            }
        } else {
            accel = 10.0f;
        }
    } else if (pad->button & CONT_A) {
        accel = 20.0f;
        if (gConfig.shield_regen && v->energy < 100.0f) {
            v->energy += gConfig.shield_regen_rate;
        }
    }

    // 2. Advanced Combat Input
    static int z_debounce = 0;
    if (z_debounce > 0) z_debounce--;

    if (gConfig.spin_attack && (pad->button & CONT_Z) && z_debounce == 0 && v->spin_timer == 0) {
        v->spin_timer = 60;
        v->energy -= 5.0f;
        z_debounce = 30;
    }

    if (gConfig.side_attack && (pad->button & CONT_L) && (pad->button & CONT_Z) && z_debounce == 0 && v->side_atk_timer == 0) {
        v->side_atk_timer = 20;
        v->side_atk_dir = -1;
        v->energy -= 2.0f;
        z_debounce = 30;
    } else if (gConfig.side_attack && (pad->button & CONT_R) && (pad->button & CONT_Z) && z_debounce == 0 && v->side_atk_timer == 0) {
        v->side_atk_timer = 20;
        v->side_atk_dir = 1;
        v->energy -= 2.0f;
        z_debounce = 30;
    }

    // 3. Drag
    float drag = v->velocity * 0.02f;
    v->velocity += accel - drag;
    if (v->velocity < 0) v->velocity = 0;

    // 4. Steering (Grip config applied here)
    float turn = (float)pad->stick_x / 127.0f;

    if (gConfig.quick_turn && (pad->button & CONT_L) && (pad->button & CONT_R)) {
        turn *= 2.5f;
    }

    // Use gConfig.grip_loss_factor properly
    // 0.0 = on rails, 2.0 = ice. Higher speed = less grip.
    float grip = 1.0f / (1.0f + (v->velocity * 0.001f * gConfig.grip_loss_factor));
    v->yaw -= turn * 3.0f * grip;

    if (v->spin_timer > 0) {
        v->yaw += 36.0f;
        v->spin_timer--;

        for (int m = 0; m < 30; m++) {
            if (&gMachines[m] == v) continue;
            float dx = gMachines[m].x - v->x;
            float dy = gMachines[m].y - v->y;
            float dz = gMachines[m].z - v->z;
            if (dx*dx + dy*dy + dz*dz < 1000.0f) {
                gMachines[m].energy -= 10.0f;
                gMachines[m].velocity *= 0.5f;
                Particles_Spawn(gMachines[m].x, gMachines[m].y, gMachines[m].z, 1.0f, 1.0f, 0.0f, 20.0f);
                Audio_PlaySound_Crash();
            }
        }
    }

    if (v->side_atk_timer > 0) {
        v->roll += 30.0f * v->side_atk_dir;
        v->side_atk_timer--;

        float rX = cosf(v->yaw * DEGTORAD);
        float rZ = sinf(v->yaw * DEGTORAD);
        v->x += rX * 15.0f * v->side_atk_dir;
        v->z += rZ * 15.0f * v->side_atk_dir;

        for (int m = 0; m < 30; m++) {
            if (&gMachines[m] == v) continue;
            float dx = gMachines[m].x - v->x;
            float dz = gMachines[m].z - v->z;
            if (dx*dx + dz*dz < 600.0f) {
                gMachines[m].energy -= 15.0f;
                gMachines[m].x += dx * 0.1f;
                gMachines[m].z += dz * 0.1f;
                Particles_Spawn(gMachines[m].x, gMachines[m].y, gMachines[m].z, 1.0f, 0.0f, 0.0f, 15.0f);
                Audio_PlaySound_Crash();
            }
        }
    } else {
        v->roll *= 0.8f;
    }

    while (v->yaw > 360.0f) v->yaw -= 360.0f;
    while (v->yaw < 0.0f) v->yaw += 360.0f;

    // 5. Movement Calculation
    float radYaw = v->yaw * DEGTORAD;
    float radPitch = v->pitch * DEGTORAD;

    float fwdX = sinf(radYaw) * cosf(radPitch);
    float fwdY = sinf(radPitch);
    float fwdZ = -cosf(radYaw) * cosf(radPitch);

    v->forward[0] = fwdX; v->forward[1] = fwdY; v->forward[2] = fwdZ;

    // Use Gravity Scale Config
    // Base gravity is ~0.5f slowing factor on vertical slopes.
    v->velocity -= 0.5f * fwdY * gConfig.gravity_scale;

    float moveX = fwdX * v->velocity;
    float moveY = fwdY * v->velocity;
    float moveZ = fwdZ * v->velocity;

    if (v->side_atk_timer == 0) {
        float rX = cosf(radYaw);
        float rZ = sinf(radYaw);

        if (pad->button & CONT_L) {
            moveX -= rX * gConfig.strafe_power;
            moveZ -= rZ * gConfig.strafe_power;
        } else if (pad->button & CONT_R) {
            moveX += rX * gConfig.strafe_power;
            moveZ += rZ * gConfig.strafe_power;
        }
    }

    // 6. Sub-stepping
    float totalMoveDist = sqrtf(moveX*moveX + moveY*moveY + moveZ*moveZ);
    int steps = (int)(totalMoveDist / 10.0f) + 1;
    if (steps > 10) steps = 10;

    float stepX = moveX / steps;
    float stepY = moveY / steps;
    float stepZ = moveZ / steps;

    for (int i = 0; i < steps; i++) {
        float nextX = v->x + stepX;
        float nextY = v->y + stepY;
        float nextZ = v->z + stepZ;

        TrackSurfaceInfo info = Track_GetInfoAt(nextX, nextY, nextZ);

        if (info.isValid) {
            float dx = nextX - info.cx;
            float dy = nextY - info.cy;
            float dz = nextZ - info.cz;
            float distSq = dx*dx + dy*dy + dz*dz;

            float limit = info.width - 10.0f;

            if (distSq > limit*limit) {
                if (i == 0) {
                    Audio_PlaySound_Crash();
                    HAL_Haptic_Rumble(0.8f, 500);
                    Particles_Spawn(nextX, nextY, nextZ, 1.0f, 0.8f, 0.0f, 8.0f);
                }

                v->velocity *= 0.9f;
                v->energy -= 1.0f;
                if (v->energy < 0.0f) v->energy = 0.0f;

                float dist = sqrtf(distSq);
                float push = (dist - limit) + 1.0f;

                float nx = -dx / dist;
                float ny = -dy / dist;
                float nz = -dz / dist;

                nextX += nx * push;
                nextY += ny * push;
                nextZ += nz * push;

                stepX += nx * push;
                stepY += ny * push;
                stepZ += nz * push;
            }

            v->x = nextX;
            v->z = nextZ;

            float hoverTarget = info.y + 10.0f * info.ny;
            v->y = nextY + (hoverTarget - nextY) * 0.2f;

            v->up[0] += (info.nx - v->up[0]) * 0.1f;
            v->up[1] += (info.ny - v->up[1]) * 0.1f;
            v->up[2] += (info.nz - v->up[2]) * 0.1f;

            float curFwdLen = sqrtf(fwdX*fwdX + fwdZ*fwdZ);
            float targetPitch = atan2f(fwdY, curFwdLen) * (180.0f/3.14159f);
            v->pitch += (targetPitch - v->pitch) * 0.1f;

        } else {
            v->x = nextX;
            // Use gravity scale for falling
            v->y = nextY - (5.0f * gConfig.gravity_scale);
            v->z = nextZ;

            if (v->y < -500.0f) {
                Physics_Init(v);
                break;
            }
        }
    }

    // 7. Update Speed KPH
    v->speed_kph = v->velocity * 10.0f;
}
