#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum {
    STATE_BOOT,
    STATE_TITLE,
    STATE_MENU,
    STATE_MACHINE_SELECT,
    STATE_TRACK_SELECT,
    STATE_RACE,
    STATE_RESULT
} GameStateID;

void GameState_Init(void);
void GameState_Update(void);
void GameState_Render(void);
void GameState_Change(GameStateID nextState);

#endif
