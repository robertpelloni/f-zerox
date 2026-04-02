#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdint.h>

void Particles_Init(void);
void Particles_Spawn(float x, float y, float z, float r, float g, float b, float size);
void Particles_Update(void);
void Particles_Render(void);

#endif
