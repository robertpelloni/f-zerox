#include "pc/game_state.h"
#include "pc/hal.h"
#include "pc/race_logic.h"
#include "pc/game_loop.h"
#include "pc/cup_system.h"
#include "pc/configfile.h"
#include "pc/ui/ui_helpers.h"
#include "pc/scenes/results_screen.h"
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
                Cup_End();
                GameState_Change(STATE_MENU);
            } else {
                GameState_Change(STATE_RACE);
            }
        } else {
            GameState_Change(STATE_MENU);
        }
        debounce = 30;
    }
}

void Scene_Result_Render(void) {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -50.0f);

    // Background visualization (Podium base)
    glBegin(GL_QUADS);
    glColor3f(0.1f, 0.1f, 0.2f); // Dark blue/grey platform
    glVertex3f(-20, -5, -10);
    glVertex3f(20, -5, -10);
    glVertex3f(20, 5, -10);
    glVertex3f(-20, 5, -10);
    glEnd();

    // Draw rotating player vehicle on podium
    glTranslatef(0.0f, 0.0f, 10.0f);
    static float rot = 0.0f;
    rot += 1.0f;
    glRotatef(rot, 0, 1, 0);

    extern Model* gBlueFalconModel;
    if (gBlueFalconModel) {
        Fast3D_ProcessDisplayList(gBlueFalconModel->dl);
    } else {
        extern Gfx blue_falcon_dl[];
        Fast3D_ProcessDisplayList(blue_falcon_dl);
    }
}

void Scene_Result_UI(struct nk_context* ctx) {
    int rank = Race_GetRank(gPlayerVehicle);

    struct nk_style_window original_style = ctx->style.window;

    // Transparent background
    ctx->style.window.background = nk_rgba(0, 0, 0, 150);
    ctx->style.window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,150));
    ctx->style.window.border = 0.0f;

    // Leaderboard Overlay
    if (nk_begin(ctx, "Race Results", nk_rect(50, 50, gConfig.width - 100, gConfig.height - 100), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
        nk_layout_row_dynamic(ctx, 40, 1);

        char rankTitle[64];
        if (gCupState.active) {
            snprintf(rankTitle, sizeof(rankTitle), "CUP STANDINGS - TRACK %d/6", gCupState.currentTrackIndex + 1);
        } else {
            snprintf(rankTitle, sizeof(rankTitle), "RACE RESULTS");
        }
        nk_label_colored(ctx, rankTitle, NK_TEXT_CENTERED, nk_rgb(255, 255, 0));

        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "------------------------------------------------", NK_TEXT_CENTERED);

        // Show Top 5
        nk_layout_row_dynamic(ctx, 30, 3);
        nk_label(ctx, "RANK", NK_TEXT_LEFT);
        nk_label(ctx, "PILOT", NK_TEXT_LEFT);
        if (gCupState.active) {
            nk_label(ctx, "TOTAL POINTS", NK_TEXT_RIGHT);
        } else {
            nk_label(ctx, "TIME", NK_TEXT_RIGHT); // Or Status
        }

        for (int i = 1; i <= 5; i++) {
            nk_layout_row_dynamic(ctx, 30, 3);

            // Highlight player row
            struct nk_color rowColor = (rank == i) ? nk_rgb(0, 255, 255) : nk_rgb(255, 255, 255);

            char rankStr[8];
            snprintf(rankStr, sizeof(rankStr), "%d", i);
            nk_label_colored(ctx, rankStr, NK_TEXT_LEFT, rowColor);

            if (rank == i) {
                nk_label_colored(ctx, gConfig.player_name, NK_TEXT_LEFT, rowColor);

                char ptsStr[32];
                if (gCupState.active) {
                    extern Vehicle gMachines[];
                    int idx = gPlayerVehicle - gMachines;
                    snprintf(ptsStr, sizeof(ptsStr), "%d pts", gCupState.points[idx]);
                } else {
                    snprintf(ptsStr, sizeof(ptsStr), "FINISHED");
                }
                nk_label_colored(ctx, ptsStr, NK_TEXT_RIGHT, rowColor);

            } else {
                char botName[32];
                snprintf(botName, sizeof(botName), "CPU Pilot %d", i);
                nk_label_colored(ctx, botName, NK_TEXT_LEFT, rowColor);

                if (gCupState.active) {
                    // Mock points for AI
                    int mockPts = (6 - i) * 10;
                    char ptsStr[32];
                    snprintf(ptsStr, sizeof(ptsStr), "%d pts", mockPts);
                    nk_label_colored(ctx, ptsStr, NK_TEXT_RIGHT, rowColor);
                } else {
                    nk_label_colored(ctx, "FINISHED", NK_TEXT_RIGHT, rowColor);
                }
            }
        }

        nk_layout_row_dynamic(ctx, 40, 1);
        nk_label(ctx, " ", NK_TEXT_CENTERED); // Spacer

        // Input Prompt
        // Blinking
        static int blink = 0;
        blink++;
        if ((blink / 30) % 2 == 0) {
            nk_label_colored(ctx, "PRESS (A) OR (START) TO CONTINUE", NK_TEXT_CENTERED, nk_rgb(255, 0, 0));
        }
    }
    nk_end(ctx);

    ctx->style.window = original_style;
}
