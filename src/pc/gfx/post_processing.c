#include "pc/gfx/post_processing.h"
#include "pc/configfile.h"
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

// Simple accumulation buffer for motion blur simulation on legacy GL
static bool sInitialized = false;

void Post_Init(int width, int height) {
    // In a full implementation, we would setup FBOs here.
    // For legacy GL, we rely on the window having an accumulation buffer requested.
    // (This requires SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE, 8) in main.c)
    (void)width;
    (void)height;
    sInitialized = true;
    printf("Post-Processing Initialized.\n");
}

void Post_Resize(int width, int height) {
    (void)width;
    (void)height;
}

void Post_Process(void) {
    if (!sInitialized) return;

    // 1. Motion Blur
    if (gConfig.motion_blur > 0.05f) {
        // Accumulate current frame
        // Factor depends on shutter speed (gConfig.motion_blur)
        // High blur = retain more of previous frame

        // This is a simplified "Trails" effect suitable for legacy GL
        float blur = gConfig.motion_blur;
        glAccum(GL_MULT, 1.0f - blur);
        glAccum(GL_ACCUM, blur);
        glAccum(GL_RETURN, 1.0f);
    }

    // 2. Chromatic Aberration (Simulated via multipass render logic in shader-less pipeline)
    // Real Chromatic Aberration requires shaders or complex FBO blitting.
    // For now, we stub it or implement a simple Red/Blue shift if we had FBO access.
}

void Post_Shutdown(void) {
    sInitialized = false;
}
