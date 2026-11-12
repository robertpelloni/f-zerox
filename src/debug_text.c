#include "common.h"
#include <stdio.h>

void func_8007F7D0(void) { /* TODO: SHIFTABLE - Implement */ }

#ifndef _PC
void func_8007F86C(void* a, void* b, void* c, void* d, void* e);
#endif

/**
 * @brief Initializes the debug text rendering buffers.
 */
void DebugText_InitBuffers(void) {
    func_8007F86C(&gDebugBufferState, &gDebugBufferAux, &gDebugBufferA, &gDebugBufferB, &gDebugBufferC);
}

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

#ifndef _PC
void func_8007FB80(s32 x, s32 y, void* str);
#endif

/**
 * @brief Draws a debug integer to the screen.
 *
 * Uses sprintf to format the integer and then draws it.
 *
 * @param val The integer to display.
 */
void DebugText_DrawInteger(s32 val) {
    char sp1C[32];

    sprintf(sp1C, gDebugFormatInteger, val);
    func_8007FB80(0x61, 0x50, (void*)gDebugLabelInteger);
    func_8007FB80(0xC9, 0x50, sp1C);
}

void func_8007FC68(s32 arg0) {
    DebugText_DrawInteger(arg0);
}

/**
 * @brief Draws the debug screen notice label to the screen.
 */
void DebugText_DrawScreenNotice(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x28, 0x6E, (void*)gDebugScreenNotice);
}

void func_8007FCB8(void) {
    DebugText_DrawScreenNotice();
}

/**
 * @brief Draws debug information group A strings to the screen.
 */
void DebugText_DrawInfoGroupA(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x2D, 0x5A, (void*)gDebugInfoGroupA1);
    func_8007FB80(0x2D, 0x6E, (void*)gDebugInfoGroupA2);
    func_8007FB80(0x2D, 0x82, (void*)gDebugInfoGroupA3);
}

void func_8007FCF4(void) {
    DebugText_DrawInfoGroupA();
}

/**
 * @brief Draws debug information group B strings to the screen.
 */
void DebugText_DrawInfoGroupB(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x2D, 0x6E, (void*)gDebugInfoGroupB1);
    func_8007FB80(0x2D, 0x82, (void*)gDebugInfoGroupB2);
    func_8007FB80(0x2D, 0x96, (void*)gDebugInfoGroupB3);
    func_8007FB80(0x2D, 0xAA, (void*)gDebugInfoGroupB4);
}

void func_8007FD58(void) {
    DebugText_DrawInfoGroupB();
}

/**
 * @brief Draws debug information group C strings to the screen.
 */
void DebugText_DrawInfoGroupC(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x35, 0x6E, (void*)gDebugInfoGroupC1);
    func_8007FB80(0x35, 0x82, (void*)gDebugInfoGroupC2);
    func_8007FB80(0x35, 0x96, (void*)gDebugInfoGroupC3);
    func_8007FB80(0x35, 0xAA, (void*)gDebugInfoGroupC4);
}

void func_8007FDD0(void) {
    DebugText_DrawInfoGroupC();
}

/**
 * @brief Draws debug information group D strings to the screen.
 */
void DebugText_DrawInfoGroupD(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x38, 0x64, (void*)gDebugInfoGroupD1);
    func_8007FB80(0x38, 0x78, (void*)gDebugInfoGroupD2);
}

void func_8007FE48(void) {
    DebugText_DrawInfoGroupD();
}

/**
 * @brief Draws debug information group E string to the screen.
 */
void DebugText_DrawInfoGroupE(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x3B, 0x6E, (void*)gDebugInfoGroupE1);
}

void func_8007FE98(void) {
    DebugText_DrawInfoGroupE();
}

/**
 * @brief Draws debug information group F strings to the screen.
 */
void DebugText_DrawInfoGroupF(void) {
    func_8007F94C();
    func_8007F970();
    func_8007FB80(0x37, 0x64, (void*)gDebugInfoGroupF1);
    func_8007FB80(0x37, 0x78, (void*)gDebugInfoGroupF2);
}

void func_8007FED4(void) {
    DebugText_DrawInfoGroupF();
}
