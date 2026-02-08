#ifndef CAMERA_H
#define CAMERA_H

#include "pc/physics.h"

typedef struct {
    float x, y, z;
    float pitch, yaw;
    float fov;
} Camera;

extern Camera gCamera;

void Camera_Update(Vehicle* target);
void Camera_Apply(void);

#endif
