#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include <stdio.h>

// Mock Game State for Debugging (since we don't have full decomp yet)
static float sMockPlayerX = 100.0f;
static float sMockPlayerY = 50.0f;
static float sMockPlayerZ = -200.0f;
static float sMockSpeed = 1200.0f;

void UI_Tab_Debug(struct nk_context *ctx) {
    UI_Header(ctx, "Developer Tools");

    nk_layout_row_dynamic(ctx, 25, 1);
    int overlay = gConfig.debug_overlay;
    UI_Checkbox(ctx, "Enable Debug Overlay", &overlay, "Show debug text.");
    gConfig.debug_overlay = overlay;

    if (nk_tree_push(ctx, NK_TREE_TAB, "Variable Watcher (Live)", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 20, 1);

        char buf[64];
        snprintf(buf, 64, "Player 1 X: %.2f", sMockPlayerX);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        snprintf(buf, 64, "Player 1 Y: %.2f", sMockPlayerY);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        snprintf(buf, 64, "Player 1 Z: %.2f", sMockPlayerZ);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        snprintf(buf, 64, "Speed: %.0f km/h", sMockSpeed);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        // Mock Updates
        sMockPlayerZ += sMockSpeed * 0.016f;
        if (sMockPlayerZ > 1000.0f) sMockPlayerZ = -1000.0f;

        nk_tree_pop(ctx);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "Memory Editor (Mock)", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 25, 2);
        nk_label(ctx, "Address: 0x80001000", NK_TEXT_LEFT);
        nk_label(ctx, "Value: 0xDEADBEEF", NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }
}
