#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/track_data.h"

// Track Editor State (Not persisted in global config yet, usually per-track file)
static int sSelectedPiece = 0;
static float sPieceBank = 0.0f;
static float sPieceWidth = 50.0f;
static int sSurfaceType = 0; // 0: Asphalt, 1: Dirt, 2: Ice, 3: Heal, 4: Slip

void UI_Tab_Editor(struct nk_context *ctx) {
    UI_Header(ctx, "Track Construction Kit (GX Expansion)");
    nk_layout_row_dynamic(ctx, 400, 2);

    // Tools Palette
    if (nk_group_begin(ctx, "Tools", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Add Straight")) Track_Editor_AddSegment(SEG_STRAIGHT);
        if (nk_button_label(ctx, "Add Curve")) Track_Editor_AddSegment(SEG_CURVE);
        if (nk_button_label(ctx, "Clear Track")) Track_Editor_Clear();

        // Placeholders
        if (nk_button_label(ctx, "Loop")) sSelectedPiece = 2;
        if (nk_button_label(ctx, "Pipe")) sSelectedPiece = 3;
        if (nk_button_label(ctx, "Cylinder")) sSelectedPiece = 4;
        nk_group_end(ctx);
    }

    // Properties Panel
    if (nk_group_begin(ctx, "Properties", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "Segment Settings:", NK_TEXT_LEFT);

        UI_Property_Float(ctx, "Bank Angle", -90.0f, &sPieceBank, 90.0f, 5.0f, 1.0f, "Tilt of the track surface.");
        UI_Property_Float(ctx, "Track Width", 10.0f, &sPieceWidth, 200.0f, 5.0f, 1.0f, "Width of the driving area.");

        nk_label(ctx, "Surface Type:", NK_TEXT_LEFT);
        const char *surfaces[] = {"Asphalt", "Dirt", "Ice", "Heal Strip", "Slip Zone"};
        sSurfaceType = nk_combo(ctx, surfaces, 5, sSurfaceType, 25, nk_vec2(200, 200));

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Test Track")) { /* Launch Test */ }
        if (nk_button_label(ctx, "Save Course")) { /* Save */ }
        if (nk_button_label(ctx, "Load Course")) { /* Load */ }

        nk_group_end(ctx);
    }
}
