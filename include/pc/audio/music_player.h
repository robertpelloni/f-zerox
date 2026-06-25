#include <stdint.h>
#ifndef MUSIC_PLAYER_H
#define MUSIC_PLAYER_H

void Music_Init(void);
void Music_Play(const char* path);
void Music_Update(int16_t* buffer, int samples);

#endif
