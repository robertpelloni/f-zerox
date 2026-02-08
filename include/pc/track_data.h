#ifndef TRACK_DATA_H
#define TRACK_DATA_H

#include <stdint.h>

typedef enum {
    SEG_STRAIGHT,
    SEG_CURVE,
    SEG_LOOP,
    SEG_PIPE,
    SEG_CYLINDER,
    SEG_CORKSCREW
} SegmentType;

typedef struct TrackSegment {
    SegmentType type;
    float length; // For straights
    float angle;  // For curves (degrees)
    float radius; // For curves
    float width;
    float bank;   // Banking angle
    struct TrackSegment* next;
} TrackSegment;

// Global list of segments
extern TrackSegment* gTrackHead;
extern TrackSegment* gTrackTail;

void Track_Editor_Init(void);
void Track_Editor_AddSegment(SegmentType type);
void Track_Editor_Clear(void);
void Track_Editor_Build(void); // Generates the Vtx/DL

#endif
