#ifndef GHOST_RECORDER_H
#define GHOST_RECORDER_H

#include "pc/physics.h"
#include <stdbool.h>

#define MAX_GHOST_FRAMES 18000 // 5 minutes at 60fps

typedef struct {
    float x, y, z;
    float yaw, pitch, roll;
} GhostFrame;

typedef struct {
    GhostFrame frames[MAX_GHOST_FRAMES];
    int frameCount;
    int playbackIndex;
    bool recording;
    bool playing;
} GhostData;

extern GhostData gGhost;

void Ghost_Init(void);
void Ghost_RecordFrame(Vehicle* v);
void Ghost_Update(Vehicle* ghostVehicle); // Playback
void Ghost_StartRecording(void);
void Ghost_StopRecording(void);
void Ghost_StartPlayback(void);

// Persistence
bool Ghost_Save(const char* filename);
bool Ghost_Load(const char* filename);

#endif
