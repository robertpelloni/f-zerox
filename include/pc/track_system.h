#ifndef TRACK_SYSTEM_H
#define TRACK_SYSTEM_H

#include "pc/ultra64.h"

// External declaration for track assets
extern Vtx mute_city_vtx[];
extern Gfx mute_city_dl[];

void Track_Init(void);
void Track_Render(void);

// Collision Query
// Returns true if position is on valid track
bool Track_GetSurfaceInfo(float x, float z, float* out_height, float* out_width);


typedef struct {
    float y;
    float width;
    float normal[3];
    float trackDist;
    bool isValid;
} TrackSurfaceInfo;
extern TrackSurfaceInfo Track_GetInfoAt(float x, float y, float z);
extern void Track_GetPointAtDist(float dist, float* x, float* y, float* z);

#endif
