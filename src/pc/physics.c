#include "pc/physics.h"
#include "pc/configfile.h"
#include "pc/track_system.h"
#include <math.h>
#include <stdlib.h>

#define DEGTORAD 0.0174532925f

void Physics_Init(Vehicle* v) {
    v->x = 0.0f;
    v->y = 20.0f; // Hover height
    v->z = 0.0f;
    v->velocity = 0.0f;
    v->yaw = 0.0f;
    v->pitch = 0.0f;
    v->roll = 0.0f;
    v->energy = 100.0f;
    v->boost_active = false;
    v->speed_kph = 0.0f;
}

void Physics_Update(Vehicle* v, OSContPad* pad) {
    // 1. Acceleration (Button A)
    float accel = 0.0f;
    v->boost_active = false;

    if (pad->button & CONT_B) {
        // Boost Logic
        if (v->energy > 0.0f) {
            accel = 50.0f;
            v->boost_active = true;
            v->energy -= 0.5f; // Drain energy
        } else {
            accel = 10.0f; // No energy, weak accel
        }
    } else if (pad->button & CONT_A) {
        accel = 20.0f; // Standard Power
        // Regen Energy slightly if Shield Regen is on (Redout)
        if (gConfig.shield_regen && v->energy < 100.0f) {
            v->energy += gConfig.shield_regen_rate;
        }
    }

    // 2. Drag / Friction
    float drag = v->velocity * 0.02f;
    v->velocity += accel - drag;
    if (v->velocity < 0) v->velocity = 0;

    // 3. Steering (Stick X)
    // Map -127..127 to -1.0..1.0
    float turn = (float)pad->stick_x / 127.0f;

    // Quick Turn (L+R)
    if (gConfig.quick_turn && (pad->button & CONT_L) && (pad->button & CONT_R)) {
        turn *= 2.5f;
    } else if (pad->button & CONT_L) {
        // Slide / Strafe
        if (gConfig.strafe_power > 0.0f) {
            v->x -= gConfig.strafe_power;
        }
    } else if (pad->button & CONT_R) {
        if (gConfig.strafe_power > 0.0f) {
            v->x += gConfig.strafe_power;
        }
    }

    // Apply Turn
    // Turning is harder at high speeds (Grip Loss)
    float grip = 1.0f / (1.0f + (v->velocity * 0.001f * gConfig.grip_loss_factor));
    v->yaw -= turn * 3.0f * grip;

    // 4. Update Position
    float rad = v->yaw * DEGTORAD;
    float nextX = v->x + sinf(rad) * v->velocity;
    float nextZ = v->z - cosf(rad) * v->velocity; // Forward is -Z

    // Collision Check
    float trackHeight, trackWidth;
    if (Track_GetSurfaceInfo(nextX, nextZ, &trackHeight, &trackWidth)) {
        // Wall Collision
        if (nextX > trackWidth || nextX < -trackWidth) {
            // Hit Wall
            v->velocity *= 0.7f; // Lose speed
            v->energy -= 5.0f; // Take Damage
            if (v->energy < 0.0f) v->energy = 0.0f;

            // Bounce
            if (nextX > trackWidth) nextX = trackWidth - 5.0f;
            if (nextX < -trackWidth) nextX = -trackWidth + 5.0f;
            // Simple visual shake or yaw change could happen here
        }

        v->x = nextX;
        v->z = nextZ;

        // Floor Glue / Hover
        float hoverTarget = trackHeight + 10.0f; // 10 units above ground
        v->y += (hoverTarget - v->y) * 0.2f; // Smooth spring
    } else {
        // Off-Road / Fall
        v->x = nextX;
        v->z = nextZ;
        v->y -= 10.0f; // Gravity Fall
        if (v->y < -500.0f) {
            // Respawn
            v->y = 100.0f;
            v->x = 0.0f;
            v->z = 0.0f;
            v->velocity = 0.0f;
        }
    }

    // 5. Update Speed KPH
    v->speed_kph = v->velocity * 10.0f; // Arbitrary scale
}
