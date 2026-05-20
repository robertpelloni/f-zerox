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

    // Gameplay state
    float energy; // 0.0 - 100.0
    bool boost_active;

    // Derived state
    float speed_kph;
} Vehicle;

void Physics_Init(Vehicle* v);
void Physics_Update(Vehicle* v, OSContPad* pad);

#endif
