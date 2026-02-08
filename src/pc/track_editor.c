#include "pc/track_data.h"
#include "pc/track_system.h"
#include "pc/gfx/fast3d.h"
#include "pc/assets.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

TrackSegment* gTrackHead = NULL;
TrackSegment* gTrackTail = NULL;

// Dynamic Buffers for Generated Mesh
#define MAX_TRACK_VTX 4096
#define MAX_TRACK_DL 8192

static Vtx sTrackVtx[MAX_TRACK_VTX];
static Gfx sTrackDL[MAX_TRACK_DL];

void Track_Editor_Init(void) {
    Track_Editor_Clear();
}

void Track_Editor_Clear(void) {
    TrackSegment* current = gTrackHead;
    while (current) {
        TrackSegment* next = current->next;
        free(current);
        current = next;
    }
    gTrackHead = NULL;
    gTrackTail = NULL;
    printf("Track Editor: Cleared.\n");
}

void Track_Editor_AddSegment(SegmentType type) {
    TrackSegment* seg = (TrackSegment*)malloc(sizeof(TrackSegment));
    if (!seg) return;

    seg->type = type;
    seg->width = 200.0f;
    seg->bank = 0.0f;
    seg->next = NULL;

    // Defaults based on type
    if (type == SEG_STRAIGHT) {
        seg->length = 1000.0f;
        seg->angle = 0.0f;
        seg->radius = 0.0f;
    } else if (type == SEG_CURVE) {
        seg->length = 0.0f;
        seg->angle = 45.0f;
        seg->radius = 500.0f;
    }

    if (!gTrackHead) {
        gTrackHead = seg;
        gTrackTail = seg;
    } else {
        gTrackTail->next = seg;
        gTrackTail = seg;
    }

    printf("Track Editor: Added Segment Type %d\n", type);
    Track_Editor_Build();
}

// Helper to push vertex
void SetVtx(Vtx* v, short x, short y, short z, uint8_t r, uint8_t g, uint8_t b) {
    v->v.ob[0] = x; v->v.ob[1] = y; v->v.ob[2] = z;
    v->v.flag = 0;
    v->v.tc[0] = 0; v->v.tc[1] = 0;
    v->v.cn[0] = r; v->v.cn[1] = g; v->v.cn[2] = b; v->v.cn[3] = 255;
}

#define DEGTORAD 0.0174532925f

void Track_Editor_Build(void) {
    // Generate Mesh walking the list
    int vtxIdx = 0;
    int dlIdx = 0;

    float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
    float yaw = 0.0f; // degrees

    TrackSegment* seg = gTrackHead;

    // Start DL
    // Note: We can't use macros easily with dynamic indices unless we construct the Gfx structs manually or use a helper
    // For now we will manually stuff the Gfx array

    // Ideally we would link gsSPVertex commands.
    // Simplifying: Just points for now or quads?
    // Let's generate a simple ribbon.

    while (seg && vtxIdx < MAX_TRACK_VTX - 4) {
        float rad = yaw * DEGTORAD;
        float dx = sinf(rad);
        float dz = -cosf(rad);
        float rightX = -dz * seg->width;
        float rightZ = dx * seg->width; // 90 deg rotation

        // Start Vertices (Left/Right)
        SetVtx(&sTrackVtx[vtxIdx+0], posX - rightX, posY, posZ - rightZ, 100, 100, 100);
        SetVtx(&sTrackVtx[vtxIdx+1], posX + rightX, posY, posZ + rightZ, 100, 100, 100);

        // Advance
        if (seg->type == SEG_STRAIGHT) {
            posX += dx * seg->length;
            posZ += dz * seg->length;
        } else if (seg->type == SEG_CURVE) {
            // Simplified linear approximation for curve end
            yaw += seg->angle;
            float newRad = yaw * DEGTORAD;
            posX += sinf(newRad) * 500.0f; // Hacky
            posZ -= cosf(newRad) * 500.0f;
        }

        // End Vertices
        rad = yaw * DEGTORAD;
        dx = sinf(rad);
        dz = -cosf(rad);
        rightX = -dz * seg->width;
        rightZ = dx * seg->width;

        SetVtx(&sTrackVtx[vtxIdx+2], posX + rightX, posY, posZ + rightZ, 100, 100, 100); // Far Right
        SetVtx(&sTrackVtx[vtxIdx+3], posX - rightX, posY, posZ - rightZ, 100, 100, 100); // Far Left

        // Add Quad to DL
        // Load 4 verts
        // gsSPVertex(&sTrackVtx[vtxIdx], 4, 0)
        sTrackDL[dlIdx] = (Gfx)gsSPVertex(&sTrackVtx[vtxIdx], 4, 0);
        dlIdx++;

        // gsSP1Triangle(0, 1, 2, 0) -> NearLeft, NearRight, FarRight
        sTrackDL[dlIdx] = (Gfx)gsSP1Triangle(0, 1, 2, 0);
        dlIdx++;

        // gsSP1Triangle(0, 2, 3, 0) -> NearLeft, FarRight, FarLeft
        sTrackDL[dlIdx] = (Gfx)gsSP1Triangle(0, 2, 3, 0);
        dlIdx++;

        vtxIdx += 4;
        seg = seg->next;
    }

    sTrackDL[dlIdx] = (Gfx)gsSPEndDisplayList();
    printf("Track Editor: Built Mesh (%d verts)\n", vtxIdx);

    // Override global track DL (Hacky hook into track_system)
    // We need a way to tell track_system to use THIS DL instead of mute_city_dl
}

// Accessor for the system
Gfx* Track_Editor_GetDL(void) {
    if (!gTrackHead) return NULL;
    return sTrackDL;
}
