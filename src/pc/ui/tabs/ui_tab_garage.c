#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"

void UI_Tab_Garage(struct nk_context *ctx) {
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

    struct nk_colorf color = {gConfig.color_primary[0], gConfig.color_primary[1], gConfig.color_primary[2], 1.0f};
    if (nk_color_pick(ctx, &color, NK_RGB)) {
        gConfig.color_primary[0] = color.r;
        gConfig.color_primary[1] = color.g;
        gConfig.color_primary[2] = color.b;
    }

    UI_Header(ctx, "Emblem Editor (Canvas)");
    nk_layout_row_dynamic(ctx, 30, 2);
    nk_label(ctx, "Layer:", NK_TEXT_LEFT);
    UI_Property_Int(ctx, "#", 0, &gConfig.emblem_layer, 10, 1, 1, "Select emblem layer.");

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
