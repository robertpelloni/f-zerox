#include "pc/fs/file_system.h"
#include "pc/track_data.h"
#include <stdio.h>

// Simple binary serialization for now
bool FS_SaveTrack(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (!file) return false;

    TrackSegment* seg = gTrackHead;
    while (seg) {
        // Write Type ID
        int type = (int)seg->type;
        fwrite(&type, sizeof(int), 1, file);
        // Write Data
        fwrite(&seg->length, sizeof(float), 1, file);
        fwrite(&seg->angle, sizeof(float), 1, file);
        fwrite(&seg->radius, sizeof(float), 1, file);
        fwrite(&seg->width, sizeof(float), 1, file);
        fwrite(&seg->bank, sizeof(float), 1, file);

        seg = seg->next;
    }

    // Sentinel
    int end = -1;
    fwrite(&end, sizeof(int), 1, file);

    fclose(file);
    printf("FS: Saved track to %s\n", filename);
    return true;
}

bool FS_LoadTrack(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return false;

    Track_Editor_Clear();

    while (true) {
        int type;
        if (fread(&type, sizeof(int), 1, file) != 1) break;
        if (type == -1) break;

        Track_Editor_AddSegment((SegmentType)type);
        // We just added the tail, now update its props
        // This is inefficient (O(N) search inside AddSegment usually) but for a load loop it's fine or we optimize later.
        // Actually AddSegment sets defaults. We need to overwrite them.

        if (gTrackTail) {
            fread(&gTrackTail->length, sizeof(float), 1, file);
            fread(&gTrackTail->angle, sizeof(float), 1, file);
            fread(&gTrackTail->radius, sizeof(float), 1, file);
            fread(&gTrackTail->width, sizeof(float), 1, file);
            fread(&gTrackTail->bank, sizeof(float), 1, file);
        }
    }

    Track_Editor_Build();
    fclose(file);
    printf("FS: Loaded track from %s\n", filename);
    return true;
}
