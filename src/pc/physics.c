#include "pc/physics.h"
#include "pc/configfile.h"
#include <math.h>

#define DEGTORAD 0.0174532925f

void Physics_Init(Vehicle* v) {
    v->x = 0.0f;
    v->y = 10.0f; // Hover height
    v->z = 0.0f;
    v->velocity = 0.0f;
    v->yaw = 0.0f;
    v->pitch = 0.0f;
    v->roll = 0.0f;
    v->speed_kph = 0.0f;
}

void Physics_Update(Vehicle* v, OSContPad* pad) {
    // 1. Acceleration (Button A)
    float accel = 0.0f;
    if (pad->button & CONT_A) {
        accel = 20.0f; // Power
    } else if (pad->button & CONT_B) {
        accel = 50.0f; // Boost
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
    v->x += sinf(rad) * v->velocity;
    v->z -= cosf(rad) * v->velocity; // Forward is -Z

    // 5. Update Speed KPH
    v->speed_kph = v->velocity * 10.0f; // Arbitrary scale
}
