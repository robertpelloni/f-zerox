#ifndef FAST3D_H
#define FAST3D_H

#include "pc/ultra64.h"
#include "pc/PR/gbi.h"

// Initialize the Fast3D Graphics system (OpenGLES 2.0 or OpenGL 3.3)
void Fast3D_Init(void);

// Process a Display List (Gfx*)
void Fast3D_ProcessDisplayList(Gfx* dl);

// Render the processed Display List to the screen
void Fast3D_Render(void);

// Clean up resources
void Fast3D_Shutdown(void);

// Update directional lighting dynamically
void Fast3D_SetLightDirection(float x, float y, float z);
void Fast3D_SetLightColor(float r, float g, float b);

#endif
