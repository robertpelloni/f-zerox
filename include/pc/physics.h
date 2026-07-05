#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdbool.h>
#include "pc/ultra64.h"

typedef struct {
    float x, y, z;
    float velocity;
    float yaw;   // Rotation Y (0 = North)
    float pitch; // Rotation X (0 = Flat)
    float roll;  // Rotation Z (0 = Flat)

    // Physics Vectors for rendering and track alignment
    float up[3];      // Normal vector
    float forward[3]; // Direction of travel

    // Gameplay state
    float energy; // 0.0 - 100.0
    bool boost_active;

    // Derived state
    float speed_kph;
} Vehicle;

void Physics_Init(Vehicle* v);
void Physics_Update(Vehicle* v, OSContPad* pad);

#endif
