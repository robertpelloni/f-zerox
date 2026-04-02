#include "pc/gfx/particles.h"
#include <SDL2/SDL_opengl.h>
#include <stdlib.h>

#define MAX_PARTICLES 1024

typedef struct {
    bool active;
    float x, y, z;
    float vx, vy, vz;
    float life; // 1.0 to 0.0
    float r, g, b;
    float size;
} Particle;

static Particle sParticles[MAX_PARTICLES];

void Particles_Init(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) sParticles[i].active = false;
}

void Particles_Spawn(float x, float y, float z, float r, float g, float b, float size) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!sParticles[i].active) {
            sParticles[i].active = true;
            sParticles[i].x = x; sParticles[i].y = y; sParticles[i].z = z;
            // Random velocity spread
            sParticles[i].vx = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;
            sParticles[i].vy = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;
            sParticles[i].vz = ((float)rand() / RAND_MAX - 0.5f) * 5.0f;

            sParticles[i].r = r; sParticles[i].g = g; sParticles[i].b = b;
            sParticles[i].size = size;
            sParticles[i].life = 1.0f;
            return;
        }
    }
}

void Particles_Update(void) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (sParticles[i].active) {
            sParticles[i].x += sParticles[i].vx;
            sParticles[i].y += sParticles[i].vy;
            sParticles[i].z += sParticles[i].vz;

            sParticles[i].life -= 0.02f; // Fade out
            if (sParticles[i].life <= 0.0f) sParticles[i].active = false;
        }
    }
}

void Particles_Render(void) {
    // Simple Immediate Mode Billboards
    // We should disable texture and enable blending
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending for glow

    // We need the Camera matrix to billboard correctly (View Plane Aligned)
    // For simplicity, we just draw quads facing Z for now, or points.
    // Points are easiest and look okay for "sparks".

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
