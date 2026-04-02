#ifndef FAST3D_H
#define FAST3D_H

#include "pc/ultra64.h"
#include "pc/PR/gbi.h"
#include <SDL2/SDL_opengl.h>

void Fast3D_Init(void);
void Fast3D_ProcessDisplayList(Gfx* dl);
void Fast3D_Render(void);
void Fast3D_Shutdown(void);
void Fast3D_RegisterTexture(uintptr_t addr, GLuint id);

// New features
void Fast3D_SetFOV(float fov_degrees);

#endif
