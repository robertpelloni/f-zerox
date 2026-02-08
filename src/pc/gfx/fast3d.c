#include "pc/gfx/fast3d.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

// Simple placeholder implementation for testing
// In a full implementation, this would parse GBI commands like G_TRI1, G_SETOTHERMODE, etc.

void Fast3D_Init(void) {
    printf("Fast3D: Initializing OpenGL...\n");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Set perspective projection (for testing)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Simple mock projection (normally handled by G_MTX commands)
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 1000.0);
    glMatrixMode(GL_MODELVIEW);
}

void Fast3D_ProcessDisplayList(Gfx* dl) {
    if (!dl) return;

    // In a real parser, we would iterate through the Gfx* array
    // processing commands like gsSPVertex, gsSP1Triangle, etc.
    // For now, we'll just render a hardcoded triangle to prove the hook works.
}

void Fast3D_Render(void) {
    // Draw a spinning triangle to test depth/rendering behind UI
    static float angle = 0.0f;
    angle += 1.0f;

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f); glVertex3f( 0.0f,  1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, 0.0f);
    glEnd();
}

void Fast3D_Shutdown(void) {
    printf("Fast3D: Shutdown.\n");
}
