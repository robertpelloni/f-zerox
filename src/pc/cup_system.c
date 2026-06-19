#include <stddef.h>
#include <string.h>
#include "pc/cup_system.h"
#include <stdio.h>

CupState gCupState;

void Cup_Start(CupID cup) {
    gCupState.active = true;
    gCupState.currentCup = cup;
    gCupState.currentTrackIndex = 0;

    memset(gCupState.points, 0, sizeof(gCupState.points));

    printf("Cup System: Started Cup %d\n", cup);
}

void Cup_AdvanceToNextTrack(void) {
    if (!gCupState.active) return;

    gCupState.currentTrackIndex++;
    if (gCupState.currentTrackIndex >= MAX_CUP_TRACKS) {
        printf("Cup System: Cup Finished!\n");
        // Should transition to final podium/ceremony here
    } else {
        printf("Cup System: Advanced to track %d\n", gCupState.currentTrackIndex);
    }
}

int Cup_GetPointsForRank(int rank) {
    // Standard F-Zero X points structure
    switch (rank) {
        case 1: return 100;
        case 2: return 93;
        case 3: return 87;
        case 4: return 81;
        case 5: return 76;
        case 6: return 71;
        default:
            // 7th down to 30th gets progressively less
            if (rank > 30) return 0;
            return 71 - ((rank - 6) * 3); // Rough approximation, clamping at 0
    }
}

void Cup_RecordRaceResults(int* ranks, int numMachines) {
    if (!gCupState.active) return;

    for (int i = 0; i < numMachines && i < MAX_CUP_MACHINES; i++) {
        int pts = Cup_GetPointsForRank(ranks[i]);
        if (pts < 0) pts = 0;
        gCupState.points[i] += pts;
    }

    printf("Cup System: Recorded points for Race %d\n", gCupState.currentTrackIndex + 1);
}

bool Cup_IsFinished(void) {
    return gCupState.active && (gCupState.currentTrackIndex >= MAX_CUP_TRACKS);
}

void Cup_End(void) {
    gCupState.active = false;
}
