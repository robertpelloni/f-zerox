#include "pc/ai_system.h"
#include "pc/track_data.h"
#include "pc/configfile.h" // reuse grip loss if needed
#include <math.h>
#include <stdlib.h>

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

        // Calculate Steering Error
        // Vector to target
        float dx = targetX - v->x;
        float dz = targetZ - v->z;

        // Transform to Local Space (relative to car heading)
        float radYaw = v->yaw * 0.0174532925f;
        float cosY = cosf(radYaw);
        float sinY = sinf(radYaw);

        // Local X = Dot(Diff, Right) ?
        // Right Vector = (cosY, 0, sinY)
        // Forward = (sinY, 0, -cosY)

        // Let's use simple angle difference
        float targetYaw = atan2f(dx, -dz) * (180.0f / 3.14159f); // atan2(x, z) gives angle from Z axis?
        // atan2(y, x) -> angle from X.
        // We want angle from North (-Z).
        // if dx=0, dz=-1 -> atan2(0, 1) = 0. Correct.
        // if dx=1, dz=0 -> atan2(1, 0) = 90. Correct.

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
