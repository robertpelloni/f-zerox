#ifndef TRACK_SYSTEM_H
#define TRACK_SYSTEM_H

#include "pc/ultra64.h"
#include "pc/PR/gbi.h"
#include <stdbool.h>

// External declaration for track assets
extern Vtx mute_city_vtx[];
extern Gfx mute_city_dl[];

typedef struct {
    bool isValid;
    float x, y, z;
    float nx, ny, nz; // Normal
    float trackDist;  // Distance along track
    float width;
} TrackSurfaceInfo;

void Track_Init(void);
void Track_Render(void);

// Collision Query
// Returns true if position is on valid track
bool Track_GetSurfaceInfo(float x, float z, float* out_height, float* out_width);

TrackSurfaceInfo Track_GetInfoAt(float x, float y, float z);
void Track_GetPointAtDist(float dist, float* out_x, float* out_y, float* out_z);

#endif
