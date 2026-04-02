#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/track_data.h"
#include "pc/fs/file_system.h"
#include "pc/game_loop.h" // For MAX_MACHINES (via gMachines logic or similar)

// Track Editor State
static float sPieceBank = 0.0f;
static float sPieceWidth = 200.0f;
static int sSurfaceType = 0; // 0: Asphalt, 1: Dirt, 2: Ice, 3: Heal, 4: Slip

void UI_Tab_Editor(struct nk_context *ctx) {
    UI_Header(ctx, "Track Construction Kit (GX Expansion)");
    nk_layout_row_dynamic(ctx, 500, 2);

    // Tools Palette
    if (nk_group_begin(ctx, "Tools", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 30, 1);

        if (nk_button_label(ctx, "Add Straight")) {
            Track_Editor_AddSegment(SEG_STRAIGHT);
            if (gTrackTail) {
                gTrackTail->width = sPieceWidth;
                gTrackTail->bank = sPieceBank;
            }
            Track_Editor_Build();
        }

        if (nk_button_label(ctx, "Add Curve (45 deg)")) {
            Track_Editor_AddSegment(SEG_CURVE);
            if (gTrackTail) {
                gTrackTail->width = sPieceWidth;
                gTrackTail->bank = sPieceBank;
            }
            Track_Editor_Build();
        }

        if (nk_button_label(ctx, "Add Loop (Vertical)")) {
            Track_Editor_AddSegment(SEG_LOOP);
            if (gTrackTail) {
                gTrackTail->width = sPieceWidth;
                gTrackTail->bank = sPieceBank;
            }
            Track_Editor_Build();
        }

        if (nk_button_label(ctx, "Add Pipe/Tube")) {
            // SEG_PIPE currently maps to default mesh generation without full tube wall logic.
            // Placeholder for future tube walls.
            Track_Editor_AddSegment(SEG_PIPE);
            if (gTrackTail) {
                gTrackTail->width = sPieceWidth;
                gTrackTail->bank = sPieceBank;
            }
            Track_Editor_Build();
        }

        if (nk_button_label(ctx, "Clear Track")) {
            Track_Editor_Clear();
        }

        nk_group_end(ctx);
    }

    // Properties Panel
    if (nk_group_begin(ctx, "Properties", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 25, 1);
        nk_label(ctx, "Next Segment Defaults:", NK_TEXT_LEFT);

        UI_Property_Float(ctx, "Bank Angle (Deg)", -90.0f, &sPieceBank, 90.0f, 5.0f, 1.0f, "Tilt of the track surface for curves (Positive = Right).");
        UI_Property_Float(ctx, "Track Width", 50.0f, &sPieceWidth, 500.0f, 10.0f, 5.0f, "Width of the driving area from center to edge.");

        nk_label(ctx, "Surface Material:", NK_TEXT_LEFT);
        const char *surfaces[] = {"Asphalt", "Dirt", "Ice", "Heal Strip", "Slip Zone"};
        sSurfaceType = nk_combo(ctx, surfaces, 5, sSurfaceType, 25, nk_vec2(200, 200));

        nk_layout_row_dynamic(ctx, 15, 1);
        nk_label(ctx, "---", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Rebuild Mesh")) {
            Track_Editor_Build();
        }
        if (nk_button_label(ctx, "Save Course (custom_track.dat)")) {
            FS_SaveTrack("custom_track.dat");
        }
        if (nk_button_label(ctx, "Load Course (custom_track.dat)")) {
            FS_LoadTrack("custom_track.dat");
        }

        nk_group_end(ctx);
    }
}
