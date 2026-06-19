#include "pc/gfx/particles.h"
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef RAND_MAX
#define RAND_MAX 2147483647
#endif

// Increase max particles for 30-player chaos
#define MAX_PARTICLES 4096

typedef struct {
    bool active;
    float x, y, z;
    float vx, vy, vz;
    float life; // 1.0 to 0.0
    float r, g, b;
    float size;
} Particle;

static Particle sParticles[MAX_PARTICLES];
static int sNextParticleIndex = 0; // Ring buffer index

void Particles_Init(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        sParticles[i].active = false;
    }
    sNextParticleIndex = 0;
}

// Rewritten to act as a Ring Buffer (Object Pool)
// O(1) insertion time instead of O(N) search. Will overwrite oldest particles.
void Particles_Spawn(float x, float y, float z, float r, float g, float b, float size) {
    int idx = sNextParticleIndex;

    sParticles[idx].active = true;
    sParticles[idx].x = x;
    sParticles[idx].y = y;
    sParticles[idx].z = z;

    // Random velocity spread
    sParticles[idx].vx = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;
    sParticles[idx].vy = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;
    sParticles[idx].vz = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;

    sParticles[idx].r = r;
    sParticles[idx].g = g;
    sParticles[idx].b = b;
    sParticles[idx].size = size;
    sParticles[idx].life = 1.0f;

    sNextParticleIndex++;
    if (sNextParticleIndex >= MAX_PARTICLES) {
        sNextParticleIndex = 0;
    }
}

void Particles_Update(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (sParticles[i].active) {
            sParticles[i].x += sParticles[i].vx;
            sParticles[i].y += sParticles[i].vy;
            sParticles[i].z += sParticles[i].vz;

            sParticles[i].life -= 0.02f; // Fade out
            if (sParticles[i].life <= 0.0f) {
                sParticles[i].active = false;
            }
        }
    }
}

void Particles_Render(void) {
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending

    // Dynamic point size not perfectly supported in basic GL without point sprites extension
    // But glPointSize works for simple squares
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (sParticles[i].active) {
            glColor4f(sParticles[i].r, sParticles[i].g, sParticles[i].b, sParticles[i].life);
            glVertex3f(sParticles[i].x, sParticles[i].y, sParticles[i].z);
        }
    }
    glEnd();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
