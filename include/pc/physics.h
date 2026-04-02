#ifndef PHYSICS_H
#define PHYSICS_H

#include "pc/ultra64.h"

typedef struct {
    float x, y, z;
    float velocity;

    // Advanced Physics (Vectors instead of Euler angles for 360 gravity)
    float up[3];      // Normal to surface
    float forward[3]; // Direction of travel
    float right[3];   // Cross product

    // Legacy Euler (for display/arcade IO compatibility)
    float yaw;
    float pitch;
    float roll;

    // Gameplay state
    float energy; // 0.0 - 100.0
    bool boost_active;

    // Advanced Mechanics State
    int spin_timer;    // >0 means spinning
    int side_atk_timer;// >0 means attacking
    int side_atk_dir;  // -1 left, 1 right

    // Derived state
    float speed_kph;
} Vehicle;

void Physics_Init(Vehicle* v);
void Physics_Update(Vehicle* v, OSContPad* pad);

#endif
