#include "pc/ultra64.h"
#include "pc/physics.h"
#include "pc/assets.h"
#include "pc/track_data.h"
#include "pc/track_system.h"
#include <math.h>

float Math_SinS(int16_t angle) { return sinf(angle * (M_PI / 32768.0f)); }
float Math_CosS(int16_t angle) { return cosf(angle * (M_PI / 32768.0f)); }
float Math_SqrtF(float value) { return sqrtf(value); }

Vehicle gMachines[30];
float gTrackTotalLength = 1000.0f;

void Fast3D_RegisterTexture(uintptr_t addr, unsigned int id) { (void)addr; (void)id; }

typedef struct Model Model;
Model* OBJ_Load(const char* filepath) { (void)filepath; return NULL; }
void Game_RunFrame(void) {}

TrackSurfaceInfo Track_GetInfoAt(float x, float y, float z) {
    TrackSurfaceInfo info;
    info.isValid = false;
    info.x = x;
    info.y = y;
    info.z = z;
    info.nx = 0; info.ny = 1; info.nz = 0;
    info.trackDist = 0;
    info.width = 100;
    return info;
}
void Track_GetPointAtDist(float dist, float* out_x, float* out_y, float* out_z) {
    *out_x = 0;
    *out_y = 0;
    *out_z = -dist;
}

Gfx blue_falcon_dl[1] = { 0 };
