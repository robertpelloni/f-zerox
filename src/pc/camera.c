#include "pc/camera.h"
#include "pc/configfile.h"
#include <math.h>
#include <SDL2/SDL_opengl.h>

#define DEGTORAD 0.0174532925f

Camera gCamera;

void Camera_Update(Vehicle* target) {
    // Basic Chase Camera
    float dist = 300.0f + (target->velocity * 0.5f * gConfig.dynamic_fov); // Zoom out with speed
    float height = 100.0f;
    float angle = target->yaw * DEGTORAD;

    // Calculate desired position
    float targetX = target->x - (sinf(angle) * dist);
    float targetZ = target->z + (cosf(angle) * dist);
    float targetY = target->y + height;

    // Interpolation (Spring)
    gCamera.x += (targetX - gCamera.x) * 0.1f;
    gCamera.y += (targetY - gCamera.y) * 0.1f;
    gCamera.z += (targetZ - gCamera.z) * 0.1f;

    // Look at target
    gCamera.yaw = target->yaw;
    gCamera.pitch = 10.0f; // Look slightly down

    // Shake
    if (target->velocity > 50.0f) {
        gCamera.x += (rand() % 10 - 5) * gConfig.camera_shake;
        gCamera.y += (rand() % 10 - 5) * gConfig.camera_shake;
    }
}

void Camera_Apply(void) {
    glRotatef(gCamera.pitch, 1.0f, 0.0f, 0.0f);
    glRotatef(-gCamera.yaw, 0.0f, 1.0f, 0.0f); // Rotate scene opposite to cam
    glTranslatef(-gCamera.x, -gCamera.y, -gCamera.z);
}
