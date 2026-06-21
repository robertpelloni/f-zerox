#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

#include <stdint.h>

void Music_Init(void);
void Music_Play(const char* path);
void Music_Update(int16_t* buffer, int samples);

// Modulates volume based on race tension/lap
void Music_SetIntensity(float intensity);

#endif
