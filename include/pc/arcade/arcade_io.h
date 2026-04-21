#ifndef ARCADE_IO_H
#define ARCADE_IO_H

#include <stdbool.h>

// Initialize connection to AX Cabinet Motion Board
bool Arcade_Init(const char* port);

// Send telemetry data to the motion seat
// pitch: -1.0 (Nose Down) to 1.0 (Nose Up)
// roll: -1.0 (Left) to 1.0 (Right)
// heave: 0.0 (Neutral) to 1.0 (Bump/Jump)
void Arcade_SendMotion(float pitch, float roll, float heave);

void Arcade_Shutdown(void);

#endif
