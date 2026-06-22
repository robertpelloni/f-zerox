#include <stdlib.h>
#include "pc/ai_system.h"
#include "pc/ultra64.h"
#include "pc/PR/gbi.h"
#include "pc/track_system.h"
#include "pc/track_data.h"
#include "pc/configfile.h" // reuse grip loss if needed
#include <math.h>

extern Vehicle gMachines[30];

void AI_Update(Vehicle* v, OSContPad* out_pad) {
    // 1. Throttle Logic
    out_pad->button = CONT_A;
    if (v->energy > 30.0f && (rand() % 100) < 2) { // 2% chance per frame to boost
        out_pad->button |= CONT_B;
    }

    // 2. Steering Logic - Spline Following
    // Get current position on track
    TrackSurfaceInfo info = Track_GetInfoAt(v->x, v->y, v->z);

    if (info.isValid) {
        // Look Ahead
        float lookAheadDist = 300.0f + v->velocity * 0.5f;
        float targetDist = info.trackDist + lookAheadDist;

        float targetX, targetY, targetZ;
        Track_GetPointAtDist(targetDist, &targetX, &targetY, &targetZ);

        // Vector to target
        float dx = targetX - v->x;
        float dz = targetZ - v->z;

        // Boids Separation: Avoid other machines
        float sepX = 0.0f;
        float sepZ = 0.0f;
        int neighbors = 0;
        float avoidanceRadius = 150.0f;

        for (int i = 0; i < 30; i++) {
            if (&gMachines[i] == v) continue;
            if (gMachines[i].y <= -9000.0f) continue; // Inactive

            float dmx = v->x - gMachines[i].x;
            float dmz = v->z - gMachines[i].z;
            float distSq = dmx*dmx + dmz*dmz;

            if (distSq > 0.001f && distSq < avoidanceRadius * avoidanceRadius) {
                float dist = sqrtf(distSq);
                sepX += (dmx / dist) * (avoidanceRadius - dist);
                sepZ += (dmz / dist) * (avoidanceRadius - dist);
                neighbors++;
            }
        }

        if (neighbors > 0) {
            sepX /= neighbors;
            sepZ /= neighbors;

            // Blend track following with separation (prioritize staying on track if near edge)
            // info.x is world space. We need distance to center. Since we don't have the explicit
            // lateral offset mapped yet, we can't reliably scale based on edge distance here.
            // Safe fallback:

            float sepWeight = 1.0f;
            dx += sepX * sepWeight * 2.0f; // Multiplier to tune avoidance strength
            dz += sepZ * sepWeight * 2.0f;
        }

        // Let's use simple angle difference
        float targetYaw = atan2f(dx, -dz) * (180.0f / 3.14159f); // atan2(x, z) gives angle from Z axis?

        float angleDiff = targetYaw - v->yaw;
        // Normalize angle
        while (angleDiff > 180) angleDiff -= 360;
        while (angleDiff < -180) angleDiff += 360;

        // PID
        float steer = angleDiff * 4.0f; // Proportional Gain

        if (steer > 80.0f) steer = 80.0f;
        if (steer < -80.0f) steer = -80.0f;

        out_pad->stick_x = (int8_t)steer;

        // Air Control (Pitch)
        // If targetY is significantly different, maybe pitch?
        // But physics auto-aligns to track.
    } else {
        // Lost? Steer Center
        out_pad->stick_x = 0;
    }
}
