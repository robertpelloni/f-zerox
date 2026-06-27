#include "pc/gfx/fast3d.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdbool.h>
#include <stdio.h>

// Internal Vertex Buffer
#define MAX_VERTICES 32
static Vtx sVertexBuffer[MAX_VERTICES];

void Fast3D_Init(void) {
    printf("Fast3D: Initializing OpenGL...\n");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Setup Basic Directional Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL); // Allow glColor to control material properties
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_dir[] = { 0.5f, 1.0f, 0.5f, 0.0f }; // Directional light
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Set perspective projection (for testing)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10000.0); // Increased far plane
    glMatrixMode(GL_MODELVIEW);
}

void Fast3D_DrawTriangle(int v0, int v1, int v2) {
    // Basic Geometry Drawing
    glBegin(GL_TRIANGLES);
        glColor3ub(sVertexBuffer[v0].v.cn[0], sVertexBuffer[v0].v.cn[1], sVertexBuffer[v0].v.cn[2]);
        glVertex3s(sVertexBuffer[v0].v.ob[0], sVertexBuffer[v0].v.ob[1], sVertexBuffer[v0].v.ob[2]);

        glColor3ub(sVertexBuffer[v1].v.cn[0], sVertexBuffer[v1].v.cn[1], sVertexBuffer[v1].v.cn[2]);
        glVertex3s(sVertexBuffer[v1].v.ob[0], sVertexBuffer[v1].v.ob[1], sVertexBuffer[v1].v.ob[2]);

        glColor3ub(sVertexBuffer[v2].v.cn[0], sVertexBuffer[v2].v.cn[1], sVertexBuffer[v2].v.cn[2]);
        glVertex3s(sVertexBuffer[v2].v.ob[0], sVertexBuffer[v2].v.ob[1], sVertexBuffer[v2].v.ob[2]);
    glEnd();
}

void Fast3D_ProcessDisplayList(Gfx* dl) {
    if (!dl) return;

    // Simple state machine for GBI parsing
    // NOTE: This assumes big-endian commands are swapped or handled (PC is little-endian)
    // But since we define the DL in C source, the values are native struct fields.
    // The macros in gbi.h pack them into 64-bit words (w0, w1).
    // We need to unpack them.

    while (true) {
        uint32_t w0 = dl->words.w0;
        uint32_t w1 = dl->words.w1;
        uint8_t opcode = (w0 >> 24) & 0xFF;

        switch (opcode) {
            case G_VTX: {
                // G_VTX: Load vertices into buffer
                // w0: (num << 12) | (len) -> num is number of vertices
                // w1: address of Vtx array
                int num = (w0 >> 12) & 0xFF;
                int idx = (w0 >> 1) & 0xFF; // Start index in buffer (often just 0 or v0)
                // Note: The macro encoding in gbi.h is complex.
                // For our manual DL: gsSPVertex(v, n, v0)
                // _SHIFTL(cmd, 24, 8) | _SHIFTL(n, 12, 8) | _SHIFTL(v0+n, 1, 7)

                // Correction for standard F3DEX:
                // num is actually (N)
                // dest index is ((v0) - (n)) ... wait, let's just use the pointer directly for now.

                // In our C definition, dl->words.w1 IS the pointer to the array (on 32-bit systems)
                // On 64-bit, we might have issues if gbi.h defines it as uint32_t.
                // Assuming standard 32-bit pointer casting or "segment address" logic.

                Vtx* src = (Vtx*)(uintptr_t)w1;
                if (src && num < MAX_VERTICES) {
                    for (int i = 0; i < num; i++) {
                        sVertexBuffer[i] = src[i];
                    }
                }
                break;
            }
            case G_TRI1: {
                // G_TRI1: Draw one triangle
                // w0: opcode
                // w1: indices packed
                // gsSP1Triangle(v0, v1, v2, flag)
                // w1: (v0*2)<<16 | (v1*2)<<8 | (v2*2)

                int v0 = ((w1 >> 16) & 0xFF) / 2;
                int v1 = ((w1 >> 8) & 0xFF) / 2;
                int v2 = (w1 & 0xFF) / 2;
                Fast3D_DrawTriangle(v0, v1, v2);
                break;
            }
            case G_SETOTHERMODE_L:
            case G_SETOTHERMODE_H:
                // TODO: Implement Render Modes (Z-Compare, Culling, Blending)
                // For now, we assume standard OPA_SURF
                break;
            case G_ENDDL:
                return;
            default:
                // Skip unknown commands
                break;
        }
        dl++;
    }
}

void Fast3D_Render(void) {
    // This function is now used to Flush any pending GL state if needed.
    // The actual drawing happens inside ProcessDisplayList via glBegin/glEnd.
}

void Fast3D_Shutdown(void) {
    printf("Fast3D: Shutdown.\n");
}

// System Hardware Shims (For compilation)
#include <stdarg.h>
void osSyncPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void osWritebackDCache(void* vaddr, int nbytes) {
    (void)vaddr; (void)nbytes;
}

void osInvalDCache(void* vaddr, int nbytes) {
    (void)vaddr; (void)nbytes;
}

void gSPDisplayList(Gfx* dl, Gfx* branch) {
    (void)dl;
    Fast3D_ProcessDisplayList(branch);
}

void gDPPipeSync(Gfx* dl) {
    (void)dl;
}

void gDPFullSync(Gfx* dl) {
    (void)dl;
}

// System Hardware Shims (For compilation)
#include <stdarg.h>
void osSyncPrintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void osWritebackDCache(void* vaddr, int nbytes) {
    (void)vaddr; (void)nbytes;
}

void osInvalDCache(void* vaddr, int nbytes) {
    (void)vaddr; (void)nbytes;
}

void gSPDisplayList(Gfx* dl, Gfx* branch) {
    (void)dl;
    Fast3D_ProcessDisplayList(branch);
}

void gDPPipeSync(Gfx* dl) {
    (void)dl;
}

void gDPFullSync(Gfx* dl) {
    (void)dl;
}
