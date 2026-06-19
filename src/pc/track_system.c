#include <SDL2/SDL_opengl.h>
#include "pc/track_system.h"
#include "pc/track_data.h"
#define G_IM_FMT_RGBA 0
#define G_IM_SIZ_16b 2
#include "pc/gfx/fast3d.h"
#include "pc/track_editor.h" // For dynamic track
#include "pc/assets.h" // For texture binding
#include <stdio.h>

// Simple placeholder track
// A long straight road

Vtx mute_city_vtx[] = {
    // Road Segment 1 (Start)
    {{{ -200,   0,    0}, 0, {0, 0}, {50, 50, 50, 255}}}, // 0: Left Near
    {{{  200,   0,    0}, 0, {0, 0}, {50, 50, 50, 255}}}, // 1: Right Near
    {{{  200,   0, -1000}, 0, {0, 0}, {50, 50, 50, 255}}}, // 2: Right Far
    {{{ -200,   0, -1000}, 0, {0, 0}, {50, 50, 50, 255}}}, // 3: Left Far
};

Gfx mute_city_dl[] = {
    gsSPVertex(mute_city_vtx, 4, 0),
    gsSP1Triangle(0, 1, 2, 0),
    gsSP1Triangle(0, 2, 3, 0),
    gsSPEndDisplayList(),
};

void Track_Init(void) {
    printf("Track System: Initializing Dynamic Test Track...\n");

    Track_Editor_Init();

    // Generate Test Track
    // 1. Start Straight (1000 units)
    Track_Editor_AddSegment(SEG_STRAIGHT);

    // 2. Curve Right (45 deg)
    Track_Editor_AddSegment(SEG_CURVE);

    // 3. Short Straight
    Track_Editor_AddSegment(SEG_STRAIGHT);

    // 4. Vertical Loop (Pseudo)
    Track_Editor_AddSegment(SEG_LOOP);

    // 5. Landing Straight
    Track_Editor_AddSegment(SEG_STRAIGHT);

    printf("Track System: Dynamic Track Generated.\n");
}

void Track_Render(void) {
    Gfx* dynamicDL = Track_Editor_GetDL();
    if (dynamicDL && dynamicDL[0].words.w0 != 0) {
        // Bind Asphalt Texture
        // Hardcoded ID 0x100 for generated asphalt (see Assets_Init)
        extern void Fast3D_RegisterTexture(uintptr_t addr, GLuint id);

        // We synthesize a G_SETTIMG command packet on the stack.
        Gfx texCmd[] = {
            gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, (void*)0x100),
            gsSPEndDisplayList()
        };
        Fast3D_ProcessDisplayList(texCmd);

        Fast3D_ProcessDisplayList(dynamicDL);
    } else {
        Fast3D_ProcessDisplayList(mute_city_dl);
    }
}

// Legacy Collision Wrapper (Forward to Dynamic System)
bool Track_GetSurfaceInfo(float x, float z, float* out_height, float* out_width) {
    // Try Dynamic System first (y=0 guess)
    TrackSurfaceInfo info = Track_GetInfoAt(x, 100.0f, z);

    if (info.isValid) {
        if (out_height) *out_height = info.y;
        if (out_width) *out_width = info.width;
        return true;
    }

    // Fallback to Mute City Flat
    if (z > 50.0f || z < -2000.0f) {
        return false;
    }
    if (out_height) *out_height = 0.0f;
    if (out_width) *out_width = 200.0f;
    return true;
}
