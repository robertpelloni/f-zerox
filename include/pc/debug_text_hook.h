#ifndef DEBUG_TEXT_HOOK_H
#define DEBUG_TEXT_HOOK_H

#include <stdint.h>

#define MAX_DEBUG_STRINGS 32

typedef struct {
    int x, y;
    char text[128];
    bool active;
} DebugString;

extern DebugString gDebugStrings[MAX_DEBUG_STRINGS];

void DebugText_Clear(void);
void DebugText_AddString(int x, int y, const char* str);

#endif
