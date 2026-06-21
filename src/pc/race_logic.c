#include <stdlib.h>
#include "pc/race_logic.h"
#include "pc/track_data.h"
#include "pc/track_system.h"
#include "pc/game_state.h"
#include "pc/audio/music_player.h"
#include "pc/cup_system.h"
#include <stdio.h>

// Machine State Extension for Race Logic
// Ideally this would be in vehicle struct, but we are keeping it separate for now
// to avoid touching core physics structs if not needed.
// However, associating it by index is fragile if machines are reordered.
// We'll assume gMachines array index corresponds here.

typedef struct {
    float lapProgress; // Distance on current lap
    float totalDist;   // Total distance (Laps * Length + Progress)
    int currentLap;
    int rank;
    bool finished;
} RaceState;

static RaceState sRaceStates[30];
extern float gTrackTotalLength; // From track_editor.c

#define TOTAL_LAPS 3

void Race_Init(void) {
    for (int i = 0; i < 30; i++) {
        sRaceStates[i].lapProgress = 0.0f;
        sRaceStates[i].totalDist = 0.0f;
        sRaceStates[i].currentLap = 1;
        sRaceStates[i].rank = i + 1;
        sRaceStates[i].finished = false;
    }
}

int CompareRaceStates(const void* a, const void* b) {
    // Sort descending by totalDist
    const RaceState* rA = *(const RaceState**)a;
    const RaceState* rB = *(const RaceState**)b;

    if (rA->totalDist > rB->totalDist) return -1;
    if (rA->totalDist < rB->totalDist) return 1;
    return 0;
}

void Race_UpdateRankings(Vehicle* machines, int count) {
    if (gTrackTotalLength <= 0.0f) return;

    RaceState* sortedPtrs[30];

    for (int i = 0; i < count; i++) {
        // 1. Get Progress from Track System
        // We assume physics update has already run and position is valid
        TrackSurfaceInfo info = Track_GetInfoAt(machines[i].x, machines[i].y, machines[i].z);

        if (info.isValid) {
            float oldProgress = sRaceStates[i].lapProgress;
            float newProgress = info.trackDist;

            // Check for Lap Crossing
            // Forward: 90% -> 10%
            if (oldProgress > gTrackTotalLength * 0.9f && newProgress < gTrackTotalLength * 0.1f) {
                if (sRaceStates[i].currentLap <= TOTAL_LAPS) {
                    sRaceStates[i].currentLap++;
                }
            }
            // Backward: 10% -> 90% (Respawn/Glitch)
            else if (oldProgress < gTrackTotalLength * 0.1f && newProgress > gTrackTotalLength * 0.9f) {
                if (sRaceStates[i].currentLap > 1) {
                    sRaceStates[i].currentLap--;
                }
            }

            if (sRaceStates[i].currentLap <= TOTAL_LAPS) {
                sRaceStates[i].lapProgress = newProgress;
                sRaceStates[i].totalDist = (sRaceStates[i].currentLap - 1) * gTrackTotalLength + newProgress;
            } else {
                if (!sRaceStates[i].finished) {
                    // Just crossed the finish line
                    if (i == 0) { // Player
                        // Restore music to normal if race finished
                        Music_SetIntensity(1.0f);
                    }
                }

                sRaceStates[i].finished = true;
                // Stagger totalDist by rank to prevent unstable qsort swapping on ties
                sRaceStates[i].totalDist = TOTAL_LAPS * gTrackTotalLength + 1000.0f + (30 - sRaceStates[i].rank);
            }

        } else {
            // Off track, estimate based on last known?
            // For now, don't update progress (freeze rank until returned)
        }

        sortedPtrs[i] = &sRaceStates[i];
    }

    // Dynamic Music for Player (Index 0)
    if (sRaceStates[0].currentLap == TOTAL_LAPS && !sRaceStates[0].finished) {
        Music_SetIntensity(1.5f); // Final lap hype!
    }

    // 2. Sort
    qsort(sortedPtrs, count, sizeof(RaceState*), CompareRaceStates);

    // 3. Assign Ranks
    for (int r = 0; r < count; r++) {
        sortedPtrs[r]->rank = r + 1;
    }
}

int Race_GetRank(Vehicle* v) {
    // Hack: We need the index of v in gMachines.
    // Assuming v is &gMachines[i] or we can pointer arithmetic.
    // Or we assume caller knows index.
    // Let's assume v is gPlayerVehicle (index 0) usually.

    // Scan address
    extern Vehicle gMachines[]; // From game_loop.c
    int idx = v - gMachines;
    if (idx >= 0 && idx < 30) {
        return sRaceStates[idx].rank;
    }
    return 30;
}

int Race_GetLap(Vehicle* v) {
    extern Vehicle gMachines[];
    int idx = v - gMachines;
    if (idx >= 0 && idx < 30) {
        return sRaceStates[idx].currentLap;
    }
    return 1;
}

void Race_Update(void) {
    // If the player is finished, transition to Results Screen
    extern Vehicle gPlayerVehicle;
    extern Vehicle gMachines[];

    // gMachines[0] is strictly the player vehicle slot
    int idx = 0;
    if (idx >= 0 && idx < 30 && sRaceStates[idx].finished) {
        // Collect final ranks for all machines to pass to cup
        int finalRanks[30];
        for (int i=0; i<30; i++) {
             finalRanks[i] = sRaceStates[i].rank;
        }

        Cup_RecordRaceResults(finalRanks, 30);
        GameState_Change(STATE_RESULT);
    }
}
