#include "pc/gfx/tex_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "pc/stb_image.h"
#include <stdio.h>

GLuint Tex_Load(const char* path) {
    int w, h, comp;
    unsigned char* image = stbi_load(path, &w, &h, &comp, STBI_rgb_alpha);
    if (!image) {
        printf("TexLoader: Failed to load %s\n", path);
        return 0;
    }

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);
    printf("TexLoader: Loaded %s (%dx%d)\n", path, w, h);
    return tex;
}

GLuint Tex_LoadFromMemory(const void* data, int len) {
    // For N64 textures (raw bytes), we would need a proper converter (N64 to RGBA32).
    // For now, this is a placeholder.
    (void)data;
    (void)len;
    return 0;
}
