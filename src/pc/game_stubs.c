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

// Math_Rand is now successfully decompiled in src/math_utils.c and linked!
// Stub removed to allow linking of the authentic N64 XOR Shift RNG.
