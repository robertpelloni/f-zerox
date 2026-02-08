#include "pc/ui/hud.h"
#include "pc/configfile.h"
#include <stdio.h>

void HUD_Render(struct nk_context* ctx, Vehicle* v) {
    if (gConfig.hud_style == 0) { // Classic
        // Bottom Right Speedometer
        if (nk_begin(ctx, "Speedo", nk_rect(gConfig.width - 200, gConfig.height - 100, 180, 80), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
            nk_layout_row_dynamic(ctx, 40, 1);
            char buf[32];
            snprintf(buf, 32, "%04d km/h", (int)v->speed_kph);
            nk_label(ctx, buf, NK_TEXT_CENTERED);

            // Energy Strip (Live)
            nk_layout_row_dynamic(ctx, 10, 1);
            size_t prog = (size_t)v->energy;
            nk_progress(ctx, &prog, 100, NK_FIXED);
        }
        nk_end(ctx);

        // Top Right Timer
        if (nk_begin(ctx, "Timer", nk_rect(gConfig.width - 200, 20, 180, 50), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
            nk_layout_row_dynamic(ctx, 30, 1);
            nk_label(ctx, "00'24\"56", NK_TEXT_CENTERED);
        }
        nk_end(ctx);
    }
    else if (gConfig.hud_style == 1) { // Modern (GX)
        // Central Digital Speedo
        if (nk_begin(ctx, "GX_HUD", nk_rect((gConfig.width/2)-100, gConfig.height - 150, 200, 100), NK_WINDOW_NO_SCROLLBAR)) {
            nk_layout_row_dynamic(ctx, 60, 1);
            char buf[32];
            snprintf(buf, 32, "%.0f", v->speed_kph);
            // In a real implementation we'd use a large custom font
            nk_label(ctx, buf, NK_TEXT_CENTERED);
        }
        nk_end(ctx);
    }
}
