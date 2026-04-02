#include "pc/ui/hud.h"
#include "pc/configfile.h"
#include "pc/race_logic.h"
#include "pc/ghost_recorder.h"
#include "pc/debug_text_hook.h"
#include <stdio.h>

void HUD_Render(struct nk_context* ctx, Vehicle* v) {
    int rank = Race_GetRank(v);
    int lap = Race_GetLap(v);

    // Style adjustments for HUD
    struct nk_style_window original_style = ctx->style.window;

    // Transparent background for HUD elements
    ctx->style.window.background = nk_rgba(0, 0, 0, 0);
    ctx->style.window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,0));
    ctx->style.window.border = 0.0f;

    if (gConfig.hud_style == 0) { // Classic F-Zero X Style

        // --- Bottom Right: Speedometer & Energy ---
        if (nk_begin(ctx, "Speedo", nk_rect(gConfig.width - 250, gConfig.height - 120, 240, 110), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
            nk_layout_row_dynamic(ctx, 40, 1);
            char buf[32];
            snprintf(buf, 32, "%04d km/h", (int)v->speed_kph);
            nk_label_colored(ctx, buf, NK_TEXT_RIGHT, nk_rgb(255, 255, 0)); // Yellow text

            // Energy Strip (Live)
            nk_layout_row_dynamic(ctx, 20, 1);
            size_t prog = (size_t)v->energy;

            // Color energy bar based on amount
            struct nk_color energyColor = nk_rgb(0, 255, 0); // Green
            if (v->energy < 30.0f) energyColor = nk_rgb(255, 0, 0); // Red
            else if (v->energy < 60.0f) energyColor = nk_rgb(255, 255, 0); // Yellow

            struct nk_style_progress prog_style = ctx->style.progress;
            ctx->style.progress.cursor_normal = nk_style_item_color(energyColor);
            ctx->style.progress.cursor_hover = nk_style_item_color(energyColor);
            ctx->style.progress.cursor_active = nk_style_item_color(energyColor);

            nk_progress(ctx, &prog, 100, NK_FIXED);

            ctx->style.progress = prog_style; // Restore
        }
        nk_end(ctx);

        // --- Top Right: Timer, Rank, Lap ---
        if (nk_begin(ctx, "RaceInfo", nk_rect(gConfig.width - 200, 20, 180, 120), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
            nk_layout_row_dynamic(ctx, 30, 1);

            char lapBuf[32];
            snprintf(lapBuf, 32, "LAP %d/3", lap); // Assume 3 laps
            nk_label_colored(ctx, lapBuf, NK_TEXT_RIGHT, nk_rgb(255, 255, 255));

            // Timer (Mocked for now, need actual race timer)
            nk_label_colored(ctx, "TIME 00'24\"56", NK_TEXT_RIGHT, nk_rgb(255, 255, 255));

            char rankBuf[16];
            snprintf(rankBuf, 16, "RANK %d/30", rank);

            struct nk_color rankColor = nk_rgb(255, 255, 255);
            if (rank == 1) rankColor = nk_rgb(255, 215, 0); // Gold

            nk_label_colored(ctx, rankBuf, NK_TEXT_RIGHT, rankColor);
        }
        nk_end(ctx);

        // --- Top Center: REPLAY Indicator ---
        if (gGhost.playing) {
            if (nk_begin(ctx, "ReplayInd", nk_rect((gConfig.width/2)-100, 20, 200, 50), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
                nk_layout_row_dynamic(ctx, 30, 1);
                // Blinking effect
                static int blink = 0;
                blink++;
                if ((blink / 30) % 2 == 0) {
                    nk_label_colored(ctx, "• REPLAY", NK_TEXT_CENTERED, nk_rgb(255, 0, 0));
                }
            }
            nk_end(ctx);
        }
    }
    else if (gConfig.hud_style == 1) { // Modern (GX Style)
        // Central Digital Speedo
        if (nk_begin(ctx, "GX_HUD", nk_rect((gConfig.width/2)-100, gConfig.height - 150, 200, 100), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
            nk_layout_row_dynamic(ctx, 60, 1);
            char buf[32];
            snprintf(buf, 32, "%.0f", v->speed_kph);
            nk_label_colored(ctx, buf, NK_TEXT_CENTERED, nk_rgb(0, 255, 255)); // Cyan
        }
        nk_end(ctx);
    }

    // --- N64 Debug Text Overlay ---
    // Render strings captured from func_8007FB80
    if (nk_begin(ctx, "N64DebugText", nk_rect(0, 0, gConfig.width, gConfig.height), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
        for (int i = 0; i < MAX_DEBUG_STRINGS; i++) {
            if (gDebugStrings[i].active) {
                // Simple mapping of N64 coordinates to PC screen
                // N64 screen is typically 320x240
                float scaleX = (float)gConfig.width / 320.0f;
                float scaleY = (float)gConfig.height / 240.0f;

                // Use a free-floating rect for arbitrary text placement
                // Note: Nuklear's layout system is immediate, but we can position arbitrarily
                // using nk_layout_space_begin/push/end.
                nk_layout_space_begin(ctx, NK_STATIC, gConfig.height, MAX_DEBUG_STRINGS);
                nk_layout_space_push(ctx, nk_rect(gDebugStrings[i].x * scaleX, gDebugStrings[i].y * scaleY, 400, 30));
                nk_label_colored(ctx, gDebugStrings[i].text, NK_TEXT_LEFT, nk_rgb(255, 255, 255));
                nk_layout_space_end(ctx);
            }
        }
    }
    nk_end(ctx);

    // Clear debug strings for next frame
    DebugText_Clear();

    // Restore original window style
    ctx->style.window = original_style;
}
