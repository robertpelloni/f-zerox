#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdint.h>
#include "pc/nuklear.h"

typedef enum {
    STATE_BOOT,
    STATE_TITLE,
    STATE_MENU,
    STATE_MACHINE_SELECT,
    STATE_RACE,
    STATE_RESULT
} GameStateID;

void GameState_Init(void);
void GameState_Change(GameStateID nextState);
void GameState_Update(void);
void GameState_Render(void);

// Nuklear overlay hook for state-specific UI
void GameState_UI(struct nk_context* ctx);

#endif
