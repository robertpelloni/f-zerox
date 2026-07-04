#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"

void UI_Tab_Visuals(struct nk_context *ctx) {
    UI_Header(ctx, "Post-Processing Effects (Modern)");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Motion Blur Intensity", 0.0f, &gConfig.motion_blur, 1.0f, 0.05f, 0.01f, "Simulates camera shutter speed at high velocities (Redout feel).");
    UI_Property_Float(ctx, "Chromatic Aberration", 0.0f, &gConfig.chromatic_aberration, 1.0f, 0.05f, 0.01f, "Color fringing at screen edges. Increases with boost.");
    UI_Property_Float(ctx, "Lens Flare Strength", 0.0f, &gConfig.lens_flare, 1.0f, 0.05f, 0.01f, "Anamorphic lens flares from engines and sun.");
    UI_Property_Float(ctx, "Bloom Threshold", 0.0f, &gConfig.bloom_threshold, 1.0f, 0.05f, 0.01f, "Brightness level at which glow begins.");

    UI_Header(ctx, "Camera & Immersion");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Dynamic FOV Scale", 1.0f, &gConfig.dynamic_fov, 2.0f, 0.1f, 0.01f, "How much the camera zooms out at top speed.");
    UI_Property_Float(ctx, "Camera Shake", 0.0f, &gConfig.camera_shake, 1.0f, 0.05f, 0.01f, "Vibration intensity on boost and impact.");

    int cockpit = gConfig.cockpit_view;
    UI_Checkbox(ctx, "Enable Cockpit View (VR Ready)", &cockpit, "First-person perspective from inside the machine.");
    gConfig.cockpit_view = cockpit;


    UI_Header(ctx, "Directional Lighting (GX)");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Light Dir X", -1.0f, &gConfig.light_dir[0], 1.0f, 0.05f, 0.01f, "Direction of primary light source (X axis).");
    UI_Property_Float(ctx, "Light Dir Y", -1.0f, &gConfig.light_dir[1], 1.0f, 0.05f, 0.01f, "Direction of primary light source (Y axis).");
    UI_Property_Float(ctx, "Light Dir Z", -1.0f, &gConfig.light_dir[2], 1.0f, 0.05f, 0.01f, "Direction of primary light source (Z axis).");

    nk_label(ctx, "Light Color:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 120, 1);
    struct nk_colorf color = {gConfig.light_color[0], gConfig.light_color[1], gConfig.light_color[2], 1.0f};
    if (nk_color_pick(ctx, &color, NK_RGB)) {
        gConfig.light_color[0] = color.r;
        gConfig.light_color[1] = color.g;
        gConfig.light_color[2] = color.b;
    }

    UI_Header(ctx, "Heads-Up Display (HUD)");
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "HUD Style:", NK_TEXT_LEFT);
    const char *styles[] = {"Classic (N64)", "Modern (GX)", "Minimalist (Redout)"};
    gConfig.hud_style = nk_combo(ctx, styles, 3, gConfig.hud_style, 25, nk_vec2(200, 200));
}
