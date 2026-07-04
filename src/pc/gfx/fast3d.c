#include "pc/gfx/fast3d.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include "pc/configfile.h"

// Internal Vertex Buffer
#define MAX_VERTICES 32
static Vtx sVertexBuffer[MAX_VERTICES];


#include "pc/assets/dynamic_asset_loader.h"

// Texture Cache mapping N64 address to OpenGL texture ID
#define MAX_TEXTURES 256
static struct {
    uintptr_t addr;
    GLuint id;
} sTextureCache[MAX_TEXTURES];
static int sTextureCount = 0;

void Fast3D_RegisterTexture(uintptr_t addr, GLuint id) {
    if (sTextureCount < MAX_TEXTURES) {
        sTextureCache[sTextureCount].addr = addr;
        sTextureCache[sTextureCount].id = id;
        sTextureCount++;
    }
}

GLuint Fast3D_GetTexture(uintptr_t addr) {
    for (int i = 0; i < sTextureCount; i++) {
        if (sTextureCache[i].addr == addr) {
            return sTextureCache[i].id;
        }
    }

    // Fallback to Dynamic Asset Loader to retrieve dynamically mapped assets
    void* loadedAsset = Asset_GetByAddress((uint32_t)addr);
    if (loadedAsset) {
        return (GLuint)(uintptr_t)loadedAsset;
    }

    return 0; // Or a default texture
}

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

    GLfloat light_dir[] = { gConfig.light_dir[0], gConfig.light_dir[1], gConfig.light_dir[2], 0.0f }; // Directional light
    GLfloat light_diffuse[] = { gConfig.light_color[0], gConfig.light_color[1], gConfig.light_color[2], 1.0f };
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
        // Map texture coordinates if enabled. (Scale 10.0f is arbitrary to match N64 fixed point coordinates)
        glTexCoord2f(sVertexBuffer[v0].v.tc[0] / 32.0f / 10.0f, sVertexBuffer[v0].v.tc[1] / 32.0f / 10.0f);
        glColor3ub(sVertexBuffer[v0].v.cn[0], sVertexBuffer[v0].v.cn[1], sVertexBuffer[v0].v.cn[2]);
        glVertex3s(sVertexBuffer[v0].v.ob[0], sVertexBuffer[v0].v.ob[1], sVertexBuffer[v0].v.ob[2]);

        glTexCoord2f(sVertexBuffer[v1].v.tc[0] / 32.0f / 10.0f, sVertexBuffer[v1].v.tc[1] / 32.0f / 10.0f);
        glColor3ub(sVertexBuffer[v1].v.cn[0], sVertexBuffer[v1].v.cn[1], sVertexBuffer[v1].v.cn[2]);
        glVertex3s(sVertexBuffer[v1].v.ob[0], sVertexBuffer[v1].v.ob[1], sVertexBuffer[v1].v.ob[2]);

        glTexCoord2f(sVertexBuffer[v2].v.tc[0] / 32.0f / 10.0f, sVertexBuffer[v2].v.tc[1] / 32.0f / 10.0f);
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

            case G_SETTIMG: {
                // We mainly care about the address (i) for our texture swap
                uintptr_t addr = (uintptr_t)w1;
                GLuint tex = Fast3D_GetTexture(addr);
                if (tex > 0) {
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, tex);
                }
                break;
            }
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

void Fast3D_SetLightDirection(float x, float y, float z) {
    GLfloat light_dir[] = { x, y, z, 0.0f }; // 0.0f means directional light
    glLightfv(GL_LIGHT0, GL_POSITION, light_dir);
}

void Fast3D_SetLightColor(float r, float g, float b) {
    GLfloat light_diffuse[] = { r, g, b, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
}
