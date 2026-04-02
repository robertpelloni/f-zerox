#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/arcade/arcade_io.h"

// Arcade Calibration State (Local)
static float sMotionScale = 1.0f;
static float sTestPitch = 0.0f;
static float sTestRoll = 0.0f;

void UI_Tab_Arcade(struct nk_context *ctx) {
    UI_Header(ctx, "AX Arcade Cabinet Output");

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Motion Seat Telemetry:", NK_TEXT_LEFT);

    UI_Property_Float(ctx, "Motion Intensity", 0.1f, &sMotionScale, 2.0f, 0.1f, 0.01f, "Global scaling for pitch/roll output.");

    UI_Header(ctx, "Manual Calibration");
    nk_layout_row_dynamic(ctx, 25, 2);
    UI_Property_Float(ctx, "Test Pitch", -1.0f, &sTestPitch, 1.0f, 0.1f, 0.01f, "Override Pitch");
    UI_Property_Float(ctx, "Test Roll", -1.0f, &sTestRoll, 1.0f, 0.1f, 0.01f, "Override Roll");

    if (nk_button_label(ctx, "Send Test Signal")) {
        Arcade_SendMotion(sTestPitch, sTestRoll, 0.0f);
    }
}
