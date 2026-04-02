#include "pc/camera.h"
#include "pc/configfile.h"
#include "pc/gfx/fast3d.h"
#include "pc/track_data.h"
#include <math.h>
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>

Camera gCamera;

// Vector Helpers
static void Vec3_Sub(float* out, float* a, float* b) {
    out[0] = a[0] - b[0]; out[1] = a[1] - b[1]; out[2] = a[2] - b[2];
}

static void Vec3_Add(float* out, float* a, float* b) {
    out[0] = a[0] + b[0]; out[1] = a[1] + b[1]; out[2] = a[2] + b[2];
}

static void Vec3_Scale(float* out, float* v, float s) {
    out[0] = v[0] * s; out[1] = v[1] * s; out[2] = v[2] * s;
}

static void Vec3_Normalize(float* v) {
    float mag = sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    if (mag > 0.0001f) {
        v[0] /= mag; v[1] /= mag; v[2] /= mag;
    }
}

static void Vec3_Cross(float* out, float* a, float* b) {
    out[0] = a[1]*b[2] - a[2]*b[1];
    out[1] = a[2]*b[0] - a[0]*b[2];
    out[2] = a[0]*b[1] - a[1]*b[0];
}

void Camera_Update(Vehicle* target) {
    // 1. Dynamic FOV based on speed
    float baseFov = 60.0f;
    float speedZoom = target->velocity * 0.1f * gConfig.dynamic_fov;
    float targetFov = baseFov + speedZoom;
    if (targetFov > 120.0f) targetFov = 120.0f;

    gCamera.fov += (targetFov - gCamera.fov) * 0.1f;
    Fast3D_SetFOV(gCamera.fov);

    // 2. Calculate Desired Position
    float dist = 250.0f;
    float height = 80.0f;

    if (gConfig.cockpit_view) {
        dist = -20.0f; // Inside machine
        height = 30.0f;
    }

    float offset[3];
    float upOffset[3];
    float backOffset[3];

    Vec3_Scale(backOffset, target->forward, -dist);
    Vec3_Scale(upOffset, target->up, height);

    float targetPos[3] = {target->x, target->y, target->z};
    float desiredPos[3];

    Vec3_Add(desiredPos, targetPos, backOffset);
    Vec3_Add(desiredPos, desiredPos, upOffset);

    // 3. Interpolate Position
    float camPos[3] = {gCamera.x, gCamera.y, gCamera.z};
    float diff[3];
    Vec3_Sub(diff, desiredPos, camPos);

    float spring = 0.2f + (target->velocity * 0.001f);
    if (spring > 0.5f) spring = 0.5f;

    gCamera.x += diff[0] * spring;
    gCamera.y += diff[1] * spring;
    gCamera.z += diff[2] * spring;

    // 3b. Camera Collision with Track
    // Prevent camera from clipping through the floor on steep hills
    if (!gConfig.cockpit_view) {
        TrackSurfaceInfo camInfo = Track_GetInfoAt(gCamera.x, gCamera.y, gCamera.z);
        if (camInfo.isValid) {
            float minHeight = camInfo.y + 10.0f;
            if (gCamera.y < minHeight) {
                gCamera.y = minHeight;
            }
        }
    }

    // 4. Interpolate Up Vector
    if (gCamera.up[0] == 0 && gCamera.up[1] == 0 && gCamera.up[2] == 0) {
        gCamera.up[1] = 1.0f;
    }

    float upDiff[3];
    Vec3_Sub(upDiff, target->up, gCamera.up);
    gCamera.up[0] += upDiff[0] * 0.2f;
    gCamera.up[1] += upDiff[1] * 0.2f;
    gCamera.up[2] += upDiff[2] * 0.2f;
    Vec3_Normalize(gCamera.up);

    // 5. Update Focus Point
    gCamera.focus[0] = target->x;
    gCamera.focus[1] = target->y + 20.0f;
    gCamera.focus[2] = target->z;

    // Shake
    if (target->velocity > 70.0f) {
        float shake = (target->velocity - 70.0f) * 0.01f * gConfig.camera_shake;
        gCamera.x += ((rand() % 100) / 50.0f - 1.0f) * shake;
        gCamera.y += ((rand() % 100) / 50.0f - 1.0f) * shake;
    }
}

void Camera_Apply(void) {
    float eye[3] = {gCamera.x, gCamera.y, gCamera.z};
    float center[3] = {gCamera.focus[0], gCamera.focus[1], gCamera.focus[2]};
    float up[3] = {gCamera.up[0], gCamera.up[1], gCamera.up[2]};

    float f[3];
    Vec3_Sub(f, center, eye);
    Vec3_Normalize(f);

    float s[3];
    Vec3_Cross(s, f, up);
    Vec3_Normalize(s);

    float u[3];
    Vec3_Cross(u, s, f);

    float M[16];
    M[0] = s[0]; M[4] = s[1]; M[8] = s[2];  M[12] = 0.0f;
    M[1] = u[0]; M[5] = u[1]; M[9] = u[2];  M[13] = 0.0f;
    M[2] = -f[0]; M[6] = -f[1]; M[10] = -f[2]; M[14] = 0.0f;
    M[3] = 0.0f; M[7] = 0.0f; M[11] = 0.0f; M[15] = 1.0f;

    glMultMatrixf(M);
    glTranslatef(-eye[0], -eye[1], -eye[2]);
}
