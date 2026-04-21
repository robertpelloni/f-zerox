#include "pc/game_state.h"
#include "pc/hal.h"
#include "pc/assets.h"
#include "pc/assets/obj_loader.h"
#include "pc/gfx/fast3d.h"
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

extern Model* gBlueFalconModel;
static int sMachineIndex = 0;
static int sDebounce = 0;

void Scene_Select_Update(void) {
    HAL_Input_Poll();
    OSContPad pad;
    HAL_Input_GetState(0, &pad);

    if (sDebounce > 0) sDebounce--;

    if (sDebounce == 0) {
        if (pad.stick_x > 30) { sMachineIndex++; sDebounce = 10; }
        if (pad.stick_x < -30) { sMachineIndex--; sDebounce = 10; }

        if (sMachineIndex < 0) sMachineIndex = 29;
        if (sMachineIndex > 29) sMachineIndex = 0;

        if (pad.button & CONT_A) {
            // Set Player Machine ID logic here
            GameState_Change(STATE_RACE);
        }
        if (pad.button & CONT_B) {
            GameState_Change(STATE_MENU);
        }
    }
}

void Scene_Select_Render(void) {
    glLoadIdentity();
    glTranslatef(0.0f, -50.0f, -400.0f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f); // Pitch up

    static float spin = 0.0f;
    spin += 1.0f;
    glRotatef(spin, 0.0f, 1.0f, 0.0f);

    // Render the machine model
    // In future, load different OBJ based on sMachineIndex
    if (gBlueFalconModel) {
        Fast3D_ProcessDisplayList(gBlueFalconModel->dl);
    } else {
        Fast3D_ProcessDisplayList(blue_falcon_dl);
    }
}
