#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <stdbool.h>

bool FS_SaveTrack(const char* filename);
bool FS_LoadTrack(const char* filename);

#endif
