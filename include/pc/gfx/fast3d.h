#ifndef FAST3D_H
#define FAST3D_H

#include "pc/ultra64.h"
#include "pc/PR/gbi.h"

// Initialize the Fast3D Graphics system (OpenGLES 2.0 or OpenGL 3.3)
void Fast3D_Init(void);

// Process a Display List (Gfx*)
void Fast3D_ProcessDisplayList(Gfx* dl);

// HAL Hardware Stubs
void osSyncPrintf(const char* fmt, ...);
void osWritebackDCache(void* vaddr, int nbytes);
void osInvalDCache(void* vaddr, int nbytes);
void gSPDisplayList(Gfx* dl, Gfx* branch);
void gDPPipeSync(Gfx* dl);
void gDPFullSync(Gfx* dl);

// Render the processed Display List to the screen
void Fast3D_Render(void);

// Clean up resources
void Fast3D_Shutdown(void);

#endif
