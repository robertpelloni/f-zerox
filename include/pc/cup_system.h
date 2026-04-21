#ifndef CUP_SYSTEM_H
#define CUP_SYSTEM_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_CUP_TRACKS 6
#define MAX_CUP_MACHINES 30

typedef enum {
    CUP_JACK,
    CUP_QUEEN,
    CUP_KING,
    CUP_JOKER,
    CUP_X
} CupID;

typedef struct {
    bool active;
    CupID currentCup;
    int currentTrackIndex;
    int points[MAX_CUP_MACHINES]; // Parallel to gMachines array
} CupState;

extern CupState gCupState;

void Cup_Start(CupID cup);
void Cup_AdvanceToNextTrack(void);
void Cup_RecordRaceResults(int* ranks, int numMachines); // ranks[i] = rank of machine i
int Cup_GetPointsForRank(int rank);
bool Cup_IsFinished(void);
void Cup_End(void);

#endif
