#include "pc/game_state.h"
#include "pc/hal.h"
#include "pc/race_logic.h"
#include "pc/game_loop.h"
#include "pc/cup_system.h"
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

void Scene_Result_Update(void) {
    HAL_Input_Poll();
    OSContPad pad;
    HAL_Input_GetState(0, &pad);

    // Simple debounce
    static int debounce = 30;
    if (debounce > 0) debounce--;

    if (debounce == 0 && (pad.button & CONT_A || pad.button & CONT_START)) {
        if (gCupState.active) {
            Cup_AdvanceToNextTrack();
            if (Cup_IsFinished()) {
                // To do: Show final standings, then Menu
                Cup_End();
                GameState_Change(STATE_MENU);
            } else {
                GameState_Change(STATE_RACE); // Next track
            }
        } else {
            // Free race / Time Attack
            GameState_Change(STATE_MENU);
        }
        debounce = 30;
    }
}

void Scene_Result_Render(void) {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -50.0f);

    // Draw podium color block based on rank
    int rank = Race_GetRank(gPlayerVehicle);

    glBegin(GL_QUADS);
    if (rank == 1) glColor3f(1.0f, 0.8f, 0.0f); // Gold
    else if (rank == 2) glColor3f(0.8f, 0.8f, 0.8f); // Silver
    else if (rank == 3) glColor3f(0.8f, 0.5f, 0.2f); // Bronze
    else glColor3f(0.5f, 0.5f, 0.5f); // Grey

    glVertex3f(-10, -5, 0);
    glVertex3f(10, -5, 0);
    glVertex3f(10, 5, 0);
    glVertex3f(-10, 5, 0);
    glEnd();

    // If Cup is active, draw a smaller block representing total points
    if (gCupState.active) {
        // Find player index to get points
        extern Vehicle gMachines[];
        int idx = gPlayerVehicle - gMachines;
        int pts = gCupState.points[idx];

        // Render a progress bar style indicator for points?
        // Just draw a blue quad scaled by points.
        glPushMatrix();
        glTranslatef(-10.0f, 6.0f, 0.0f);
        glScalef(pts * 0.05f, 1.0f, 1.0f); // Arbitrary scale

        glBegin(GL_QUADS);
        glColor3f(0.0f, 0.5f, 1.0f); // Blue
        glVertex3f(0, 0, 0);
        glVertex3f(1, 0, 0);
        glVertex3f(1, 2, 0);
        glVertex3f(0, 2, 0);
        glEnd();
        glPopMatrix();
    }
}
