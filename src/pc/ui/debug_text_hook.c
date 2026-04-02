#include "pc/debug_text_hook.h"
#include <string.h>

DebugString gDebugStrings[MAX_DEBUG_STRINGS];

void DebugText_Clear(void) {
    for (int i = 0; i < MAX_DEBUG_STRINGS; i++) {
        gDebugStrings[i].active = false;
    }
}

void DebugText_AddString(int x, int y, const char* str) {
    for (int i = 0; i < MAX_DEBUG_STRINGS; i++) {
        if (!gDebugStrings[i].active) {
            gDebugStrings[i].active = true;
            gDebugStrings[i].x = x;
            gDebugStrings[i].y = y;
            strncpy(gDebugStrings[i].text, str, 127);
            gDebugStrings[i].text[127] = '\0';
            break;
        }
    }
}
