#ifndef GAME_STUBS_H
#define GAME_STUBS_H

#include <stdint.h>
#include "pc/ultra64.h"

// Stubs for math functions
float Math_SinS(int16_t angle);
float Math_CosS(int16_t angle);
float Math_SqrtF(float value);

// Decompiled Math Functions (Linked from src/math_utils.c)
s32 Math_Rand(void);
s32 Math_RoundF(f32 value);

#endif
