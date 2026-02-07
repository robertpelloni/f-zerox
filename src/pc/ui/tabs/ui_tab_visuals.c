#include "pc/ui/ui_helpers.h"

// Post-Processing
static float sMotionBlur = 0.5f;
static float sChromaticAberration = 0.0f;
static float sLensFlare = 0.8f;
static float sBloomThreshold = 0.7f;

// Camera
static float sDynamicFOV = 1.0f; // Scale factor
static float sCameraShake = 0.5f;
static int sCockpitView = 0;

// HUD
static int sHUDStyle = 1; // 0: Classic, 1: GX, 2: Redout

void UI_Tab_Visuals(struct nk_context *ctx) {
    UI_Header(ctx, "Post-Processing Effects (Modern)");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Motion Blur Intensity", 0.0f, &sMotionBlur, 1.0f, 0.05f, 0.01f, "Simulates camera shutter speed at high velocities (Redout feel).");
    UI_Property_Float(ctx, "Chromatic Aberration", 0.0f, &sChromaticAberration, 1.0f, 0.05f, 0.01f, "Color fringing at screen edges. Increases with boost.");
    UI_Property_Float(ctx, "Lens Flare Strength", 0.0f, &sLensFlare, 1.0f, 0.05f, 0.01f, "Anamorphic lens flares from engines and sun.");
    UI_Property_Float(ctx, "Bloom Threshold", 0.0f, &sBloomThreshold, 1.0f, 0.05f, 0.01f, "Brightness level at which glow begins.");

    UI_Header(ctx, "Camera & Immersion");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Dynamic FOV Scale", 1.0f, &sDynamicFOV, 2.0f, 0.1f, 0.01f, "How much the camera zooms out at top speed.");
    UI_Property_Float(ctx, "Camera Shake", 0.0f, &sCameraShake, 1.0f, 0.05f, 0.01f, "Vibration intensity on boost and impact.");
    UI_Checkbox(ctx, "Enable Cockpit View (VR Ready)", &sCockpitView, "First-person perspective from inside the machine.");

    UI_Header(ctx, "Heads-Up Display (HUD)");
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "HUD Style:", NK_TEXT_LEFT);
    const char *styles[] = {"Classic (N64)", "Modern (GX)", "Minimalist (Redout)"};
    sHUDStyle = nk_combo(ctx, styles, 3, sHUDStyle, 25, nk_vec2(200, 200));
}
