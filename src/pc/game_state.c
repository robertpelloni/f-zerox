#include "pc/game_state.h"
#include "pc/game_loop.h"
#include <stdio.h>

// Extern references
extern void Scene_Title_Update(void);
extern void Scene_Title_Render(void);
extern void Scene_Menu_Update(void);
extern void Scene_Menu_Render(void);
extern void Scene_Select_Update(void);
extern void Scene_Select_Render(void);
extern void Scene_Result_Update(void);
extern void Scene_Result_Render(void);

static GameStateID sCurrentState = STATE_BOOT;
static GameStateID sNextState = STATE_BOOT;

void GameState_Init(void) {
    sCurrentState = STATE_TITLE;
    printf("GameState: Initialized to TITLE\n");
}

void GameState_Change(GameStateID nextState) {
    sNextState = nextState;
}

void GameState_Update(void) {
    if (sNextState != sCurrentState) {
        printf("GameState: Changing from %d to %d\n", sCurrentState, sNextState);
        sCurrentState = sNextState;

        // Init logic per state
        if (sCurrentState == STATE_RACE) {
            Game_Init(); // Reset race
        }
    }

    switch (sCurrentState) {
        case STATE_TITLE: Scene_Title_Update(); break;
        case STATE_MENU: Scene_Menu_Update(); break;
        case STATE_MACHINE_SELECT: Scene_Select_Update(); break;
        case STATE_RACE: Game_Update(); break;
        case STATE_RESULT: Scene_Result_Update(); break;
        default: break;
    }
}

void GameState_Render(void) {
    switch (sCurrentState) {
        case STATE_TITLE: Scene_Title_Render(); break;
        case STATE_MENU: Scene_Menu_Render(); break;
        case STATE_MACHINE_SELECT: Scene_Select_Render(); break;
        case STATE_RACE: Game_Render(); break;
        case STATE_RESULT: Scene_Result_Render(); break;
        default: break;
    }
}
