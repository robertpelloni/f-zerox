#ifndef GX_TRACK_IMPORTER_H
#define GX_TRACK_IMPORTER_H

#include <stdbool.h>

// Attempts to parse an F-Zero GX .gma/.tpl file set and load it into the engine.
// Returns true if successful.
bool GX_ImportTrack(const char* gma_path);

#endif
