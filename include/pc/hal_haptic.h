#ifndef HAL_HAPTIC_H
#define HAL_HAPTIC_H

#include <stdint.h>
#include <stdbool.h>

// Initialize Haptic subsystem (opens device associated with joystick 0)
bool HAL_Haptic_Init(void);

// Play a simple rumble effect
// strength: 0.0 - 1.0
// duration: milliseconds
void HAL_Haptic_Rumble(float strength, uint32_t duration);

// Set constant force (e.g. for steering wheel resistance)
// x: -1.0 (Left) to 1.0 (Right)
void HAL_Haptic_SetConstantForce(float x);

void HAL_Haptic_Shutdown(void);

#endif
