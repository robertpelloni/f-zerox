#include "pc/game_state.h"
#include "pc/hal.h"
#include "pc/cup_system.h"
#include "pc/configfile.h" // For getting config state
#include <SDL2/SDL_opengl.h>
#include <stdio.h>

// 0: Grand Prix, 1: Time Attack, 2: Options
static int sMenuSelection = 0;
static bool sInCupSelect = false;
static int sCupSelection = 0; // 0: Jack, 1: Queen, 2: King, 3: Joker

void Scene_Menu_Update(void) {
    HAL_Input_Poll();
    OSContPad pad;
    HAL_Input_GetState(0, &pad);

    static int debounce = 0;
    if (debounce > 0) debounce--;

    if (debounce == 0) {
        if (!sInCupSelect) {
            // Main Menu
            if (pad.stick_y > 30) { sMenuSelection--; debounce = 10; }
            if (pad.stick_y < -30) { sMenuSelection++; debounce = 10; }
            if (sMenuSelection < 0) sMenuSelection = 2;
            if (sMenuSelection > 2) sMenuSelection = 0;

            if (pad.button & CONT_A) {
                if (sMenuSelection == 0) {
                    sInCupSelect = true; // Enter Cup Select Sub-menu
                    debounce = 10;
                }
                if (sMenuSelection == 1) {
                    // Time Attack (Free Race)
                    Cup_End(); // Ensure cup is inactive
                    GameState_Change(STATE_MACHINE_SELECT);
                }
                if (sMenuSelection == 2) {
                    // Options (Wait for UI overlay)
                    // Currently Nuklear renders over everything anyway.
                    // We could trigger an overlay state.
                }
            }
        } else {
            // Cup Select Sub-menu
            if (pad.stick_x > 30) { sCupSelection++; debounce = 10; }
            if (pad.stick_x < -30) { sCupSelection--; debounce = 10; }
            if (sCupSelection < 0) sCupSelection = 3;
            if (sCupSelection > 3) sCupSelection = 0;

            if (pad.button & CONT_A) {
                Cup_Start((CupID)sCupSelection);
                sInCupSelect = false;
                GameState_Change(STATE_MACHINE_SELECT);
            }
            if (pad.button & CONT_B) {
                sInCupSelect = false; // Back to main menu
                debounce = 10;
            }
        }
    }
}

void Scene_Menu_Render(void) {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -60.0f);

    if (!sInCupSelect) {
        // Draw Main Menu Items
        for (int i = 0; i < 3; i++) {
            glPushMatrix();
            glTranslatef((i - 1) * 20.0f, 0.0f, 0.0f);

            if (sMenuSelection == i) {
                glScalef(1.5f, 1.5f, 1.5f);
                static float rot = 0.0f;
                rot += 2.0f;
                glRotatef(rot, 0, 1, 0);
            }

            glBegin(GL_QUADS);
            if (i == 0) glColor3f(1.0f, 0.0f, 0.0f); // GP (Red)
            if (i == 1) glColor3f(0.0f, 1.0f, 0.0f); // TA (Green)
            if (i == 2) glColor3f(0.0f, 0.0f, 1.0f); // Options (Blue)

            // Draw a simple block
            glVertex3f(-5, -5, 0); glVertex3f(5, -5, 0); glVertex3f(5, 5, 0); glVertex3f(-5, 5, 0);

            // Optional: Draw text using glRasterPos if we don't rely fully on Nuklear for 3D UI
            // But Nuklear overlay handles text.
            glEnd();
            glPopMatrix();
        }
    } else {
        // Draw Cup Select Items
        for (int i = 0; i < 4; i++) {
            glPushMatrix();
            // Layout in a grid or line
            glTranslatef((i - 1.5f) * 15.0f, 10.0f, 0.0f);

            if (sCupSelection == i) {
                glScalef(1.5f, 1.5f, 1.5f);
                static float rot2 = 0.0f;
                rot2 += 3.0f;
                glRotatef(rot2, 0, 1, 0);
            }

            glBegin(GL_TRIANGLES);
            // Colors for cups (Jack=Yellow, Queen=Cyan, King=Magenta, Joker=Grey)
            if (i == 0) glColor3f(1.0f, 1.0f, 0.0f);
            if (i == 1) glColor3f(0.0f, 1.0f, 1.0f);
            if (i == 2) glColor3f(1.0f, 0.0f, 1.0f);
            if (i == 3) glColor3f(0.5f, 0.5f, 0.5f);

            glVertex3f(-4, -4, 0); glVertex3f(4, -4, 0); glVertex3f(0, 6, 0);

            // Add some depth
            glColor3f(0.2f, 0.2f, 0.2f);
            glVertex3f(-4, -4, -2); glVertex3f(4, -4, -2); glVertex3f(0, 6, -2);
            glEnd();
            glPopMatrix();
        }
    }
}
