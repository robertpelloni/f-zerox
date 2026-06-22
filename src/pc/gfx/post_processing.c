#include "pc/gfx/post_processing.h"
#include "pc/configfile.h"
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

// Simple accumulation buffer for motion blur simulation on legacy GL
static bool sInitialized = false;
static int sWidth = 640;
static int sHeight = 480;

static GLuint sBloomTex = 0;

void Post_Init(int width, int height) {
    sWidth = width;
    sHeight = height;
    sInitialized = true;
    printf("Post-Processing Initialized.\n");

    glGenTextures(1, &sBloomTex);
}

void Post_Resize(int width, int height) {
    sWidth = width;
    sHeight = height;
}

void Post_Process(void) {
    if (!sInitialized) return;

    // 1. Motion Blur
    if (gConfig.motion_blur > 0.05f) {
        // Accumulate current frame
        float blur = gConfig.motion_blur;
        glAccum(GL_MULT, 1.0f - blur);
        glAccum(GL_ACCUM, blur);
        glAccum(GL_RETURN, 1.0f);
    }

    // 1.5 Bloom
    if (gConfig.bloom) {
        // Pseudo-bloom for legacy GL without shaders:
        // We copy the framebuffer to a texture, draw it over the screen multiple times
        // with an additive blend and slight offset/scaling to simulate blurring.

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sBloomTex);

        // Calculate nearest power of two that fits the screen to avoid stretching artifacts
        int texWidth = 1; while (texWidth < sWidth) texWidth <<= 1;
        int texHeight = 1; while (texHeight < sHeight) texHeight <<= 1;

        // Cap at 1024 to save memory/VRAM on older GPUs
        if (texWidth > 1024) texWidth = 1024;
        if (texHeight > 1024) texHeight = 1024;

        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, texWidth, texHeight, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE); // Additive blending

        // Use threshold as the inverse of strength here for simplicity
        float str = 0.5f - (gConfig.bloom_threshold * 0.4f);
        if (str < 0.0f) str = 0.0f;
        glColor4f(str, str, str, 1.0f);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glDisable(GL_DEPTH_TEST);

        // Multi-pass blur approximations
        float offsets[4] = {-0.02f, 0.02f, -0.04f, 0.04f};

        // We must map texture coordinates strictly to the aspect ratio of the copied buffer
        // Note: texWidth/texHeight are locally scoped inside the previous block, so we need to recalculate or scope them out.
        // Let's recalculate locally to avoid changing earlier scopes in this merge
        int tw = 1; while (tw < sWidth) tw <<= 1;
        if (tw > 1024) tw = 1024;
        int th = 1; while (th < sHeight) th <<= 1;
        if (th > 1024) th = 1024;

        float uMax = (float)sWidth / (float)tw;
        float vMax = (float)sHeight / (float)th;

        for (int pass = 0; pass < 4; pass++) {
            float o = offsets[pass];
            glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f + o, -1.0f - o);
            glTexCoord2f(uMax, 0.0f); glVertex2f( 1.0f + o, -1.0f - o);
            glTexCoord2f(uMax, vMax); glVertex2f( 1.0f + o,  1.0f - o);
            glTexCoord2f(0.0f, vMax); glVertex2f(-1.0f + o,  1.0f - o);
            glEnd();
        }

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
    }

    // 2. Chromatic Aberration (Simulated via multipass render logic in shader-less pipeline)
    // Real Chromatic Aberration requires shaders or complex FBO blitting.
}

void Post_Shutdown(void) {
    sInitialized = false;
    if (sBloomTex) {
        glDeleteTextures(1, &sBloomTex);
        sBloomTex = 0;
    }
}
