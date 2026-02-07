#include "pc/ui/ui_helpers.h"

// Garage State
static int sSelectedMachine = 0;
static float sColorPrimary[3] = {0.8f, 0.1f, 0.1f};
static float sColorSecondary[3] = {0.1f, 0.1f, 0.8f};
static float sColorTertiary[3] = {0.1f, 0.1f, 0.1f};
static int sEmblemLayer = 0;

void UI_Tab_Garage(struct nk_context *ctx) {
    // Suppress unused variable warnings for placeholders
    (void)sSelectedMachine;
    (void)sColorSecondary;
    (void)sColorTertiary;
    UI_Header(ctx, "Pilot Profile & Stats");
    nk_layout_row_dynamic(ctx, 100, 2);
    if (nk_group_begin(ctx, "Machine_Preview", NK_WINDOW_NO_SCROLLBAR)) {
        nk_label(ctx, "[3D Model Preview]", NK_TEXT_CENTERED);
        nk_group_end(ctx);
    }
    if (nk_group_begin(ctx, "Stats", NK_WINDOW_NO_SCROLLBAR)) {
        nk_label(ctx, "Body: A", NK_TEXT_LEFT);
        nk_label(ctx, "Boost: B", NK_TEXT_LEFT);
        nk_label(ctx, "Grip: C", NK_TEXT_LEFT);
        nk_label(ctx, "Pilot: Captain Falcon", NK_TEXT_LEFT);
        nk_group_end(ctx);
    }

    UI_Header(ctx, "Custom Paint Job (GX Style)");
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Primary Color:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 120, 1);

    struct nk_colorf color = {sColorPrimary[0], sColorPrimary[1], sColorPrimary[2], 1.0f};
    if (nk_color_pick(ctx, &color, NK_RGB)) {
        sColorPrimary[0] = color.r;
        sColorPrimary[1] = color.g;
        sColorPrimary[2] = color.b;
    }

    UI_Header(ctx, "Emblem Editor (Canvas)");
    nk_layout_row_dynamic(ctx, 30, 2);
    nk_label(ctx, "Layer:", NK_TEXT_LEFT);
    nk_property_int(ctx, "#", 0, &sEmblemLayer, 10, 1, 1);

    nk_layout_row_dynamic(ctx, 200, 1);
    if (nk_group_begin(ctx, "Canvas", NK_WINDOW_BORDER)) {
        nk_layout_row_static(ctx, 180, 180, 1);
        nk_button_label(ctx, "[Pixel Grid Placeholder]");
        nk_group_end(ctx);
    }

    UI_Header(ctx, "Parts Shop (Story Mode)");
    nk_layout_row_dynamic(ctx, 30, 3);
    if (nk_button_label(ctx, "Buy Booster V2")) { /* Mock Buy */ }
    if (nk_button_label(ctx, "Buy Titan Body")) { /* Mock Buy */ }
    if (nk_button_label(ctx, "Buy Cyber Cockpit")) { /* Mock Buy */ }
}
