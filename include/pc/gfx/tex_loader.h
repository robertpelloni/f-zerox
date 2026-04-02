#ifndef TEX_LOADER_H
#define TEX_LOADER_H

#include <SDL2/SDL_opengl.h>

// Returns GL Texture ID
GLuint Tex_Load(const char* path);

// Returns GL Texture ID from memory (ROM pointer)
GLuint Tex_LoadFromMemory(const void* data, int len);

#endif
