#ifndef TRACK_DATA_H
#define TRACK_DATA_H

#include <stdint.h>
#include <stdbool.h>

// 3D Vector for Track Math
typedef struct {
    float x, y, z;
} TrackVec3;

typedef enum {
    SEG_STRAIGHT,
    SEG_CURVE,
    SEG_LOOP,
    SEG_PIPE,
    SEG_CYLINDER,
    SEG_CORKSCREW
} SegmentType;

// Dynamic Track Segment
struct TrackSegment {
    SegmentType type;

    // Properties
    float length; // Arc length or straight length
    float angle;  // degrees
    float radius;
    float width;
    float bank;

    // Distance Cache
    float distStart; // Cumulative distance from start
    float distEnd;

    // Cached World Space Data (Start)
    TrackVec3 startPos;
    float startYaw;
    float startPitch;
    float startRoll;

    // Cached World Space Data (End)
    TrackVec3 endPos;
    float endYaw;
    float endPitch;
    float endRoll;

    TrackVec3 normal;

    struct TrackSegment* next;
};

typedef struct TrackSegment TrackSegment;

extern TrackSegment* gTrackHead;
extern TrackSegment* gTrackTail;
extern float gTrackTotalLength;

void Track_Editor_Init(void);
void Track_Editor_AddSegment(SegmentType type);
void Track_Editor_Clear(void);
void Track_Editor_Build(void);

// 3D Math Query
typedef struct {
    float x, y, z;
    float nx, ny, nz;
    float cx, cy, cz;
    float width;
    float trackDist; // Distance along track centerline
    bool isValid;
} TrackSurfaceInfo;

// Get collision info at world position
TrackSurfaceInfo Track_GetInfoAt(float x, float y, float z);

// Get centerline position at specific track distance (for AI)
void Track_GetPointAtDist(float dist, float* x, float* y, float* z);

#endif
