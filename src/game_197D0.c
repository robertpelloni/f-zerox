#include "common.h"

void func_8007F7D0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007F86C(void) { /* TODO: SHIFTABLE - Implement */ }

void DebugText_InitBuffers(void);

void func_8007F904(void) {
    DebugText_InitBuffers();
}

/**
 * @brief Updates the global framebuffer pointer with the current framebuffer.
 */
void DebugText_UpdateFramebuffer(void) {
    gCurrentFramebuffer = osViGetCurrentFramebuffer();
}

void func_8007F94C(void) {
    DebugText_UpdateFramebuffer();
}

void func_8007F970(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007F9E0(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007FA64(void) { /* TODO: SHIFTABLE - Implement */ }

void func_8007FB80(void) { /* TODO: SHIFTABLE - Implement */ }

void DebugText_DrawInteger(s32 val);

void func_8007FC68(s32 arg0) {
    DebugText_DrawInteger(arg0);
}

void DebugText_DrawScreenNotice(void);

void func_8007FCB8(void) {
    DebugText_DrawScreenNotice();
}

void DebugText_DrawInfoGroupA(void);

void func_8007FCF4(void) {
    DebugText_DrawInfoGroupA();
}

void DebugText_DrawInfoGroupB(void);

void func_8007FD58(void) {
    DebugText_DrawInfoGroupB();
}

void DebugText_DrawInfoGroupC(void);

void func_8007FDD0(void) {
    DebugText_DrawInfoGroupC();
}

void DebugText_DrawInfoGroupD(void);
void DebugText_DrawInfoGroupE(void);
void DebugText_DrawInfoGroupF(void);

void func_8007FE48(void) {
    DebugText_DrawInfoGroupD();
}

void func_8007FE98(void) {
    DebugText_DrawInfoGroupE();
}

void func_8007FED4(void) {
    DebugText_DrawInfoGroupF();
}
