#include "pc/gfx/fast3d.h"
#include "pc/gfx/tex_loader.h"
#include "pc/configfile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

// Internal Vertex Buffer
#define MAX_VERTICES 64 // Increased for sub-mesh safety
static Vtx sVertexBuffer[MAX_VERTICES];
static GLuint sCurrentTexture = 0;

// Texture Cache
typedef struct {
    uintptr_t addr;
    GLuint id;
} TextureCacheEntry;

#define MAX_TEXTURES 64
static TextureCacheEntry sTextureCache[MAX_TEXTURES];
static int sTextureCount = 0;

GLuint Fast3D_GetTexture(uintptr_t addr) {
    for (int i = 0; i < sTextureCount; i++) {
        if (sTextureCache[i].addr == addr) return sTextureCache[i].id;
    }
    return 0;
}

void Fast3D_RegisterTexture(uintptr_t addr, GLuint id) {
    if (sTextureCount < MAX_TEXTURES) {
        sTextureCache[sTextureCount].addr = addr;
        sTextureCache[sTextureCount].id = id;
        sTextureCount++;
    }
}

void Fast3D_Init(void) {
    printf("Fast3D: Initializing OpenGL...\n");
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable Alpha Testing for punch-through decals
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    // Enable Basic Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    // Setup Light 0
    GLfloat light_pos[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Set perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

void Fast3D_SetFOV(float fov_degrees) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = 16.0f / 9.0f;
    float zNear = 1.0f;
    float zFar = 20000.0f;
    float fH = tanf(fov_degrees * 3.14159f / 360.0f) * zNear;
    float fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
}

void Fast3D_DrawTriangle(int v0, int v1, int v2) {
    if (sCurrentTexture) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, sCurrentTexture);

        // Apply Bilinear Filtering Config dynamically
        if (gConfig.bilinear_filtering) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        }
    } else {
        glDisable(GL_TEXTURE_2D);
    }

    glBegin(GL_TRIANGLES);
        float ax = sVertexBuffer[v1].v.ob[0] - sVertexBuffer[v0].v.ob[0];
        float ay = sVertexBuffer[v1].v.ob[1] - sVertexBuffer[v0].v.ob[1];
        float az = sVertexBuffer[v1].v.ob[2] - sVertexBuffer[v0].v.ob[2];

        float bx = sVertexBuffer[v2].v.ob[0] - sVertexBuffer[v0].v.ob[0];
        float by = sVertexBuffer[v2].v.ob[1] - sVertexBuffer[v0].v.ob[1];
        float bz = sVertexBuffer[v2].v.ob[2] - sVertexBuffer[v0].v.ob[2];

        float nx = ay*bz - az*by;
        float ny = az*bx - ax*bz;
        float nz = ax*by - ay*bx;

        float len = sqrtf(nx*nx + ny*ny + nz*nz);
        if (len > 0.0001f) { nx/=len; ny/=len; nz/=len; }

        glNormal3f(nx, ny, nz);

        float s_scale = 1.0f / (32.0f * 32.0f);

        glTexCoord2f(sVertexBuffer[v0].v.tc[0] * s_scale, sVertexBuffer[v0].v.tc[1] * s_scale);
        glColor4ub(sVertexBuffer[v0].v.cn[0], sVertexBuffer[v0].v.cn[1], sVertexBuffer[v0].v.cn[2], sVertexBuffer[v0].v.cn[3]);
        glVertex3s(sVertexBuffer[v0].v.ob[0], sVertexBuffer[v0].v.ob[1], sVertexBuffer[v0].v.ob[2]);

        glTexCoord2f(sVertexBuffer[v1].v.tc[0] * s_scale, sVertexBuffer[v1].v.tc[1] * s_scale);
        glColor4ub(sVertexBuffer[v1].v.cn[0], sVertexBuffer[v1].v.cn[1], sVertexBuffer[v1].v.cn[2], sVertexBuffer[v1].v.cn[3]);
        glVertex3s(sVertexBuffer[v1].v.ob[0], sVertexBuffer[v1].v.ob[1], sVertexBuffer[v1].v.ob[2]);

        glTexCoord2f(sVertexBuffer[v2].v.tc[0] * s_scale, sVertexBuffer[v2].v.tc[1] * s_scale);
        glColor4ub(sVertexBuffer[v2].v.cn[0], sVertexBuffer[v2].v.cn[1], sVertexBuffer[v2].v.cn[2], sVertexBuffer[v2].v.cn[3]);
        glVertex3s(sVertexBuffer[v2].v.ob[0], sVertexBuffer[v2].v.ob[1], sVertexBuffer[v2].v.ob[2]);
    glEnd();
}

void Fast3D_ProcessDisplayList(Gfx* dl) {
    if (!dl) return;

    while (true) {
        uint32_t w0 = dl->words.w0;
        uintptr_t w1 = dl->words.w1;
        uint8_t opcode = (w0 >> 24) & 0xFF;

        switch (opcode) {
            case G_VTX: {
                int num = (w0 >> 12) & 0xFF;
                int raw_idx = (w0 >> 1) & 0x7F;
                int dest_idx = raw_idx - num;

                if (dest_idx < 0) dest_idx = 0;

                Vtx* src = (Vtx*)w1;
                if (src && (dest_idx + num) <= MAX_VERTICES) {
                    for (int i = 0; i < num; i++) {
                        sVertexBuffer[dest_idx + i] = src[i];
                    }
                }
                break;
            }
            case G_SETTIMG: {
                uintptr_t addr = (uintptr_t)dl->words.w1;
                GLuint cached = Fast3D_GetTexture(addr);
                if (cached) {
                    sCurrentTexture = cached;
                } else {
                    static bool triedLoad = false;
                    if (!triedLoad) {
                        GLuint newTex = Tex_Load("falcon.png");
                        if (newTex) {
                            Fast3D_RegisterTexture(addr, newTex);
                            sCurrentTexture = newTex;
                        }
                        triedLoad = true;
                    }
                }
                break;
            }
            case G_TRI1: {
                int v0 = ((w1 >> 16) & 0xFF) / 2;
                int v1 = ((w1 >> 8) & 0xFF) / 2;
                int v2 = (w1 & 0xFF) / 2;
                Fast3D_DrawTriangle(v0, v1, v2);
                break;
            }
            case G_SETOTHERMODE_H: {
                break;
            }
            case G_ENDDL:
                return;
            default:
                break;
        }
        dl++;
    }
}

void Fast3D_Render(void) {
}

void Fast3D_Shutdown(void) {
    printf("Fast3D: Shutdown.\n");
}
