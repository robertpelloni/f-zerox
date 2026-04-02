#include "pc/ghost_recorder.h"
#include <stdio.h>
#include <string.h>

GhostData gGhost;

void Ghost_Init(void) {
    gGhost.frameCount = 0;
    gGhost.playbackIndex = 0;
    gGhost.recording = false;
    gGhost.playing = false;
}

void Ghost_StartRecording(void) {
    gGhost.frameCount = 0;
    gGhost.recording = true;
    gGhost.playing = false;
    printf("Ghost: Recording Started\n");
}

void Ghost_StopRecording(void) {
    gGhost.recording = false;
    printf("Ghost: Recording Stopped (%d frames)\n", gGhost.frameCount);
}

void Ghost_StartPlayback(void) {
    if (gGhost.frameCount == 0) return;
    gGhost.playbackIndex = 0;
    gGhost.playing = true;
    gGhost.recording = false;
    printf("Ghost: Playback Started\n");
}

void Ghost_RecordFrame(Vehicle* v) {
    if (!gGhost.recording) return;
    if (gGhost.frameCount >= MAX_GHOST_FRAMES) {
        Ghost_StopRecording();
        return;
    }

    GhostFrame* frame = &gGhost.frames[gGhost.frameCount];
    frame->x = v->x;
    frame->y = v->y;
    frame->z = v->z;
    frame->yaw = v->yaw;
    frame->pitch = v->pitch;
    frame->roll = v->roll;

    gGhost.frameCount++;
}

void Ghost_Update(Vehicle* ghostVehicle) {
    if (!gGhost.playing) {
        // Hide ghost if not playing
        ghostVehicle->y = -10000.0f;
        return;
    }

    if (gGhost.playbackIndex >= gGhost.frameCount) {
        // Loop or stop? Stop for now.
        gGhost.playing = false;
        return;
    }

    GhostFrame* frame = &gGhost.frames[gGhost.playbackIndex];
    ghostVehicle->x = frame->x;
    ghostVehicle->y = frame->y;
    ghostVehicle->z = frame->z;
    ghostVehicle->yaw = frame->yaw;
    ghostVehicle->pitch = frame->pitch;
    ghostVehicle->roll = frame->roll;

    gGhost.playbackIndex++;
}

bool Ghost_Save(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return false;

    fwrite(&gGhost.frameCount, sizeof(int), 1, file);
    fwrite(gGhost.frames, sizeof(GhostFrame), gGhost.frameCount, file);

    fclose(file);
    printf("Ghost: Saved to %s\n", filename);
    return true;
}

bool Ghost_Load(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return false;

    if (fread(&gGhost.frameCount, sizeof(int), 1, file) != 1) {
        fclose(file);
        return false;
    }

    if (gGhost.frameCount > MAX_GHOST_FRAMES) gGhost.frameCount = MAX_GHOST_FRAMES;

    fread(gGhost.frames, sizeof(GhostFrame), gGhost.frameCount, file);

    fclose(file);
    printf("Ghost: Loaded from %s (%d frames)\n", filename, gGhost.frameCount);
    return true;
}
