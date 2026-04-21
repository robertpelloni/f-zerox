#include "pc/game_state.h"
#include "pc/gfx/fast3d.h"
#include "pc/hal.h"
#include "pc/ui/ui.h"
#include <SDL2/SDL_opengl.h>
#include <math.h>

void Scene_Title_Update(void) {
    HAL_Input_Poll();
    OSContPad pad;
    HAL_Input_GetState(0, &pad);

    if (pad.button & CONT_START) {
        GameState_Change(STATE_MENU);
    }
}

void Scene_Title_Render(void) {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -50.0f);

    // Rotating Logo (Yellow X)
    static float angle = 0.0f;
    angle += 2.0f;
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    glBegin(GL_TRIANGLES);
    // Yellow
    glColor3f(1.0f, 0.9f, 0.0f);
    glVertex3f(-15, -15, 0);
    glVertex3f(15, -15, 0);
    glVertex3f(0, 20, 0);

    // Red Outline (Backside)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0, 20, -1);
    glVertex3f(15, -15, -1);
    glVertex3f(-15, -15, -1);
    glEnd();
}
