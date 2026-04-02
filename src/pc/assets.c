#include "pc/gfx/tex_loader.h"
#include "pc/assets/obj_loader.h"
#include "pc/assets.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>
#include <stdio.h>

// Global Assets
Model* gBlueFalconModel = NULL;

// Procedural Generation Helpers

GLuint Tex_Create(int width, int height, const void* data) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Wrap for repeated track textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    printf("TexLoader: Created texture %d (%dx%d)\n", tex, width, height);
    return tex;
}

void Assets_Init(void) {
    // Generate Asphalt Texture (64x64)
    int w = 64, h = 64;
    uint32_t* asphalt = (uint32_t*)malloc(w * h * 4);
    if (asphalt) {
        for (int i = 0; i < w * h; i++) {
            // Dark grey with noise
            uint8_t noise = rand() % 20;
            uint8_t val = 40 + noise;

            // Grid lines (border)
            int x = i % w;
            int y = i / w;
            if (x < 2 || x > 62 || y < 2 || y > 62) {
                val = 20; // Darker border
            }

            asphalt[i] = 0xFF000000 | (val << 16) | (val << 8) | val;
        }

        GLuint texID = Tex_Create(w, h, asphalt);
        free(asphalt);

        // Register globally?
        // Ideally we store this somewhere accessible by Track_Render
        // For now, let's inject it into Fast3D's cache as address 0x100 (Hack)
        extern void Fast3D_RegisterTexture(uintptr_t addr, GLuint id);
        Fast3D_RegisterTexture(0x100, texID);
    }

    // Try Load Mod
    gBlueFalconModel = OBJ_Load("mods/blue_falcon.obj");
    if (gBlueFalconModel) {
        printf("Assets: Using modded machine model.\n");
    } else {
        printf("Assets: Using internal machine model.\n");
    }
}
