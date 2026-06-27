#ifndef PC_COMPAT_H
#define PC_COMPAT_H

// This file is used to resolve conflicts between N64 SDK headers and standard PC headers.
// It provides shims and typedefs to allow the original N64 C code to compile cleanly
// natively on a modern PC environment alongside SDL2 and OpenGL.

#ifdef _LANGUAGE_C
    // On the PC, we can map standard types directly if not defined
    #include <stdint.h>
    #include <stddef.h>
    #include <stdbool.h>

    // N64 Types typically used in the decompiled logic
    typedef int8_t   s8;
    typedef uint8_t  u8;
    typedef int16_t  s16;
    typedef uint16_t u16;
    typedef int32_t  s32;
    typedef uint32_t u32;
    typedef int64_t  s64;
    typedef uint64_t u64;

    typedef float    f32;
    typedef double   f64;

    // Prevent some N64 macros from conflicting with standard libc
    #define _SIZE_T

    // Stub some basic hardware calls if they aren't provided by ultra_impl.c yet
    void osSyncPrintf(const char* fmt, ...);

#endif // _LANGUAGE_C

#endif // PC_COMPAT_H
