#include "pc/ultra64.h"
#include <math.h>

// Override/Stub for N64 Math functions if they are not yet fully decompiled or linked
// This ensures the game loop can call these without crashing.

float Math_SinS(int16_t angle) {
    // N64 angle: 0-65535 map to 0-2PI
    return sinf(angle * (M_PI / 32768.0f));
}

float Math_CosS(int16_t angle) {
    return cosf(angle * (M_PI / 32768.0f));
}

float Math_SqrtF(float value) {
    return sqrtf(value);
}

// Stub for random (can be replaced with standard rand)
uint32_t Math_Rand(void) {
    return rand();
}
