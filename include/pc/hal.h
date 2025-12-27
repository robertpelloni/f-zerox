#ifndef PC_HAL_H
#define PC_HAL_H

#include <stdint.h>
#include <stdbool.h>

// Basic types for HAL
typedef struct {
    int width;
    int height;
    bool fullscreen;
    char* title;
} VideoConfig;

typedef struct {
    int frequency;
    int channels;
    int samples;
} AudioConfig;

// Video Interface
bool HAL_Video_Init(VideoConfig* config);
void HAL_Video_Shutdown(void);
void HAL_Video_BeginFrame(void);
void HAL_Video_EndFrame(void);
void HAL_Video_SetFullscreen(bool enabled);

// Audio Interface
bool HAL_Audio_Init(AudioConfig* config);
void HAL_Audio_Shutdown(void);
void HAL_Audio_QueueSamples(const int16_t* samples, int count);

// Input Interface
void HAL_Input_Poll(void);
// TODO: Define struct for N64 controller state mapping

// System Interface
uint64_t HAL_GetTimeMillis(void);
void HAL_Delay(uint32_t millis);

#endif
