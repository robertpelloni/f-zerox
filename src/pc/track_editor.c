#include "pc/track_data.h"
#include "pc/track_system.h"
#include "pc/gfx/fast3d.h"
#include "pc/assets.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

TrackSegment* gTrackHead = NULL;
TrackSegment* gTrackTail = NULL;
float gTrackTotalLength = 0.0f;

// Dynamic Buffers for Generated Mesh
#define MAX_TRACK_VTX 16384
#define MAX_TRACK_DL 32768

static Vtx sTrackVtx[MAX_TRACK_VTX];
static Gfx sTrackDL[MAX_TRACK_DL];

#define DEGTORAD 0.0174532925f

// Vector Helpers
void Vec3_Add(TrackVec3* out, TrackVec3 a, TrackVec3 b) {
    out->x = a.x + b.x; out->y = a.y + b.y; out->z = a.z + b.z;
}

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
    gTrackTotalLength = 0.0f;
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
        // Length calculated during build
        seg->length = 0.0f;
        seg->angle = 45.0f;
        seg->radius = 500.0f;
        seg->bank = 30.0f;
    } else if (type == SEG_LOOP) {
        seg->length = 0.0f;
        seg->angle = 0.0f;
        seg->radius = 400.0f;
    }

    if (!gTrackHead) {
        gTrackHead = seg;
        gTrackTail = seg;
    } else {
        gTrackTail->next = seg;
        gTrackTail = seg;
    }

    Track_Editor_Build();
}

void SetVtx(Vtx* v, short x, short y, short z, uint8_t r, uint8_t g, uint8_t b, short s, short t) {
    v->v.ob[0] = x; v->v.ob[1] = y; v->v.ob[2] = z;
    v->v.flag = 0;
    v->v.tc[0] = s; v->v.tc[1] = t;
    v->v.cn[0] = r; v->v.cn[1] = g; v->v.cn[2] = b; v->v.cn[3] = 255;
}

void Track_Editor_Build(void) {
    // Generate Mesh walking the list
    int vtxIdx = 0;
    int dlIdx = 0;

    float posX = 0.0f, posY = 0.0f, posZ = 0.0f;
    float yaw = 0.0f; // degrees (Y axis)
    float pitch = 0.0f; // degrees (X axis)

    float totalDist = 0.0f;

    TrackSegment* seg = gTrackHead;

    while (seg && vtxIdx < MAX_TRACK_VTX - 4) {
        // Cache Start Position
        seg->startPos.x = posX; seg->startPos.y = posY; seg->startPos.z = posZ;
        seg->startYaw = yaw; seg->startPitch = pitch;
        seg->distStart = totalDist;

        // Calculate Basis Vectors
        float radYaw = yaw * DEGTORAD;
        float radPitch = pitch * DEGTORAD;

        float cosP = cosf(radPitch);
        float sinP = sinf(radPitch);
        float cosY = cosf(radYaw);
        float sinY = sinf(radYaw);

        float dx = sinY * cosP;
        float dy = sinP;
        float dz = -cosY * cosP;

        float bankRad = seg->bank * DEGTORAD;
        float cosBank = cosf(bankRad);
        float sinBank = sinf(bankRad);

        // Right Vector
        float rX = -cosY;
        float rZ = -sinY;

        float wX = rX * seg->width * cosBank;
        float wY = seg->width * sinBank;
        float wZ = rZ * seg->width * cosBank;

        // Start Vertices
        short tCoord = (short)(dlIdx * 32);

        SetVtx(&sTrackVtx[vtxIdx+0], posX - wX, posY - wY, posZ - wZ, 100, 100, 100, 0 << 5, tCoord << 5);
        SetVtx(&sTrackVtx[vtxIdx+1], posX + wX, posY + wY, posZ + wZ, 100, 100, 100, 32 << 5, tCoord << 5);

        // Advance
        if (seg->type == SEG_STRAIGHT) {
            posX += dx * seg->length;
            posY += dy * seg->length;
            posZ += dz * seg->length;
            totalDist += seg->length;
        } else if (seg->type == SEG_CURVE) {
            yaw += seg->angle;

            float arcLen = 2.0f * 3.14159f * seg->radius * (fabsf(seg->angle) / 360.0f);
            seg->length = arcLen;
            totalDist += arcLen;

            // Arc approximation
            float midAngle = (seg->startYaw + (seg->angle / 2.0f)) * DEGTORAD;
            float chord = 2.0f * seg->radius * sinf((seg->angle * DEGTORAD) / 2.0f);

            posX += sinf(midAngle) * chord;
            posZ -= cosf(midAngle) * chord;
        } else if (seg->type == SEG_LOOP) {
            // Loop approx length
            seg->length = 200.0f * 1.414f; // Sqrt(2) approximation for 45 deg ramp
            totalDist += seg->length;

            pitch += 45.0f;
            posX += dx * 200.0f;
            posY += 141.0f;
            posZ += dz * 200.0f;
        }

        // Cache End
        seg->endPos.x = posX; seg->endPos.y = posY; seg->endPos.z = posZ;
        seg->endYaw = yaw; seg->endPitch = pitch;
        seg->distEnd = totalDist;

        // Recalculate Basis for End
        radYaw = yaw * DEGTORAD;
        cosY = cosf(radYaw);
        sinY = sinf(radYaw);

        rX = -cosY;
        rZ = -sinY;

        wX = rX * seg->width * cosBank;
        wZ = rZ * seg->width * cosBank;

        short tCoordEnd = tCoord + (32 * 5);

        SetVtx(&sTrackVtx[vtxIdx+2], posX + wX, posY + wY, posZ + wZ, 100, 100, 100, 32 << 5, tCoordEnd << 5);
        SetVtx(&sTrackVtx[vtxIdx+3], posX - wX, posY - wY, posZ - wZ, 100, 100, 100, 0 << 5, tCoordEnd << 5);

        sTrackDL[dlIdx] = (Gfx)gsSPVertex(&sTrackVtx[vtxIdx], 4, 0);
        dlIdx++;
        sTrackDL[dlIdx] = (Gfx)gsSP1Triangle(0, 1, 2, 0);
        dlIdx++;
        sTrackDL[dlIdx] = (Gfx)gsSP1Triangle(0, 2, 3, 0);
        dlIdx++;

        vtxIdx += 4;
        seg = seg->next;
    }

    gTrackTotalLength = totalDist;
    sTrackDL[dlIdx] = (Gfx)gsSPEndDisplayList();
}

Gfx* Track_Editor_GetDL(void) {
    if (!gTrackHead) return NULL;
    return sTrackDL;
}

TrackSurfaceInfo Track_GetInfoAt(float x, float y, float z) {
    TrackSurfaceInfo info = {0};
    info.isValid = false;

    TrackSegment* seg = gTrackHead;

    while (seg) {
        float segDX = seg->endPos.x - seg->startPos.x;
        float segDY = seg->endPos.y - seg->startPos.y;
        float segDZ = seg->endPos.z - seg->startPos.z;
        float segLenSq = segDX*segDX + segDY*segDY + segDZ*segDZ;

        if (segLenSq < 0.001f) {
            // Fix bug: If length is 0, avoid division by zero
            seg = seg->next;
            continue;
        }

        float px = x - seg->startPos.x;
        float py = y - seg->startPos.y;
        float pz = z - seg->startPos.z;

        float t = (px * segDX + py * segDY + pz * segDZ) / segLenSq;

        if (t >= -0.0f && t <= 1.0f) {
            float centerX = seg->startPos.x + t * segDX;
            float centerY = seg->startPos.y + t * segDY;
            float centerZ = seg->startPos.z + t * segDZ;

            float latX = x - centerX;
            float latY = y - centerY;
            float latZ = z - centerZ;
            float latDistSq = latX*latX + latY*latY + latZ*latZ;

            if (latDistSq <= (seg->width * seg->width * 4.0f)) {
                info.isValid = true;
                info.width = seg->width;
                info.cx = centerX;
                info.cy = centerY;
                info.cz = centerZ;
                info.trackDist = seg->distStart + (t * seg->length);

                float len = sqrtf(segLenSq);
                float fwdX = segDX / len;
                float fwdY = segDY / len;
                float fwdZ = segDZ / len;

                float rX = -fwdZ;
                float rZ = fwdX;
                float rLen = sqrtf(rX*rX + rZ*rZ);
                if (rLen > 0.001f) { rX/=rLen; rZ/=rLen; }

                float nX = fwdY * rZ;
                float nY = fwdZ * rX - fwdX * rZ;
                float nZ = -fwdY * rX;

                float bankRad = seg->bank * DEGTORAD;
                float sinB = sinf(bankRad);
                float cosB = cosf(bankRad);

                info.nx = -nX * cosB + rX * sinB;
                info.ny = -nY * cosB;
                info.nz = -nZ * cosB + rZ * sinB;

                if (info.ny < 0) {
                     info.ny = -info.ny;
                }

                if (fabs(info.ny) > 0.001f) {
                    info.y = centerY - (info.nx * (x - centerX) + info.nz * (z - centerZ)) / info.ny;
                } else {
                    info.y = centerY;
                }

                info.x = x;
                info.z = z;

                return info;
            }
        }

        seg = seg->next;
    }

    return info;
}

// AI Helper: Get point on spline
void Track_GetPointAtDist(float dist, float* x, float* y, float* z) {
    *x = 0; *y = 0; *z = 0;

    if (gTrackTotalLength <= 0.001f) return; // Fix bug: div by 0

    while (dist > gTrackTotalLength) dist -= gTrackTotalLength;
    while (dist < 0) dist += gTrackTotalLength;

    TrackSegment* seg = gTrackHead;
    while (seg) {
        if (dist >= seg->distStart && dist <= seg->distEnd) {
            float localDist = dist - seg->distStart;
            float t = 0.0f;
            if (seg->length > 0.001f) {
                t = localDist / seg->length;
            }

            *x = seg->startPos.x + (seg->endPos.x - seg->startPos.x) * t;
            *y = seg->startPos.y + (seg->endPos.y - seg->startPos.y) * t;
            *z = seg->startPos.z + (seg->endPos.z - seg->startPos.z) * t;
            return;
        }
        seg = seg->next;
    }
}
