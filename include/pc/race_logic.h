#ifndef RACE_LOGIC_H
#define RACE_LOGIC_H

#include "pc/physics.h"

void Race_Init(void);
void Race_Update(void);

// Calculate Rank for all machines
void Race_UpdateRankings(Vehicle* machines, int count);

// Return current rank (1-30) for a machine
int Race_GetRank(Vehicle* v);

// Return current lap
int Race_GetLap(Vehicle* v);

#endif
