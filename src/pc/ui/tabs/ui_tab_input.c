#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/controller_api.h"
#include <stdio.h>
#include <string.h>

// Helper to check if we are binding this specific button
static int sBindIdx = -1;
static int sBindSlot = -1;

void UI_BindButton(struct nk_context *ctx, int btn_idx, const char* label) {
    nk_label(ctx, label, NK_TEXT_LEFT);

    // Primary Bind
    char buf[64];
    uint32_t scancode = Config_GetBind(btn_idx, 0);

    if (sBindIdx == btn_idx && sBindSlot == 0) {
        snprintf(buf, sizeof(buf), "[Press Key...]");
    } else {
        snprintf(buf, sizeof(buf), "%s", Controller_GetKeyName(scancode));
    }

    if (nk_button_label(ctx, buf)) {
        sBindIdx = btn_idx;
        sBindSlot = 0;
        Controller_StartBinding(btn_idx, 0);
    }
}

void UI_Tab_Input(struct nk_context *ctx) {
    // Check if binding finished
    if (!Controller_IsBinding()) {
        sBindIdx = -1;
        sBindSlot = -1;
    }

    UI_Header(ctx, "Input Configuration");

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Current Device: Keyboard / SDL2", NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Button Mapping", NK_MAXIMIZED)) {
        nk_layout_row_dynamic(ctx, 25, 2);

        UI_BindButton(ctx, 0, "A Button (Accelerate)");
        UI_BindButton(ctx, 1, "B Button (Boost)");
        UI_BindButton(ctx, 2, "Z Trigger (Attack)");
        UI_BindButton(ctx, 3, "Start Button");

        UI_BindButton(ctx, 8, "L Trigger (Slide Left)");
        UI_BindButton(ctx, 9, "R Trigger (Slide Right)");

        UI_BindButton(ctx, 4, "D-Pad Up");
        UI_BindButton(ctx, 5, "D-Pad Down");
        UI_BindButton(ctx, 6, "D-Pad Left");
        UI_BindButton(ctx, 7, "D-Pad Right");

        nk_tree_pop(ctx);
    }

    UI_Header(ctx, "Sensitivity");
    UI_Property_Float(ctx, "Deadzone", 0.0f, &gConfig.deadzone, 1.0f, 0.05f, 0.01f, "Adjusts the analog stick deadzone to prevent drift.");
}
