#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/ui/ui_manual.h"
#include "pc/ui/ui_tabs_content.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void UI_DrawTabs(struct nk_context *ctx) {
    if (nk_begin(ctx, "F-Zero X Settings Dashboard", nk_rect(50, 50, 1100, 800),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

        nk_layout_row_dynamic(ctx, 0, 2); // 2 main columns for dashboard

        // COLUMN 1: Settings
        if (nk_group_begin(ctx, "Col1", NK_WINDOW_BORDER)) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- SYSTEM CONFIGURATION ---", NK_TEXT_CENTERED);
            UI_Tab_System(ctx); // Fullscreen, Vsync, Bloom, Audio, Cheats

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- INPUT CONFIGURATION ---", NK_TEXT_CENTERED);
            UI_Tab_Input(ctx);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- ARCADE / TELEMETRY ---", NK_TEXT_CENTERED);
            UI_Tab_Arcade(ctx);

            nk_group_end(ctx);
        }

        // COLUMN 2: Gameplay & Assets
        if (nk_group_begin(ctx, "Col2", NK_WINDOW_BORDER)) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- VISUAL EFFECTS (GX) ---", NK_TEXT_CENTERED);
            UI_Tab_Visuals(ctx); // Post processing, FOV, Lighting

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- PHYSICS & GAMEPLAY ---", NK_TEXT_CENTERED);
            UI_Tab_Gameplay(ctx); // Gravity, Grip, Shield

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- MULTIPLAYER ---", NK_TEXT_CENTERED);
            UI_Tab_Netplay(ctx);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- TRACK EDITOR ---", NK_TEXT_CENTERED);
            UI_Tab_Editor(ctx);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- GARAGE ---", NK_TEXT_CENTERED);
            UI_Tab_Garage(ctx);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- DEVELOPER ---", NK_TEXT_CENTERED);
            UI_Tab_Debug(ctx);

            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "--- SYSTEM ACTIONS ---", NK_TEXT_CENTERED);
            if (nk_button_label(ctx, "Reset Config to Defaults")) {
                Config_SetDefaults();
            }
            if (nk_button_label(ctx, "Exit Game")) {
                Config_Save("fzerox_pc.bin");
                exit(0);
            }

            nk_group_end(ctx);
        }
    }
    nk_end(ctx);
}
