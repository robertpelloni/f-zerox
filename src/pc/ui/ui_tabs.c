#include "pc/ui/ui_tabs.h"
#include "pc/ui/ui_manual.h"
#include "pc/hal.h"
#include "pc/nuklear.h"
#include <stdio.h>

// Mock Configuration State (Placeholder for actual game config)
static int sResWidth = 1280;
static int sResHeight = 720;
static int sVSync = 1;
static int sFullscreen = 0;
static int sTextureQuality = 2; // 0: Low, 1: Med, 2: High, 3: Ultra
static int sShadowQuality = 1;
static int sBloom = 1;
static int sMasterVolume = 80;
static int sMusicVolume = 70;
static int sSFXVolume = 100;
static int s3DAudio = 0;
static int sPhysicsMode = 0; // 0: Classic, 1: GX, 2: AX
static int sGodMode = 0;
static int sUnlockAll = 0;
static int sDebugMode = 0;

void UI_Tab_General(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "F-Zero X PC Port - Status Dashboard", NK_TEXT_CENTERED);

    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, "Frame Rate: 60 FPS (Locked)", NK_TEXT_LEFT);
    nk_label(ctx, "Game State: Menu", NK_TEXT_LEFT);
    nk_label(ctx, "Uptime: 00:01:23", NK_TEXT_LEFT);

    if (nk_button_label(ctx, "Exit Game")) {
        exit(0);
    }
}

void UI_Tab_Video(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Display Settings", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 2);
    nk_property_int(ctx, "Width", 640, &sResWidth, 3840, 10, 1);
    nk_property_int(ctx, "Height", 480, &sResHeight, 2160, 10, 1);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_checkbox_label(ctx, "Fullscreen", &sFullscreen);
    nk_checkbox_label(ctx, "V-Sync", &sVSync);

    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Advanced Graphics", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Texture Quality:", NK_TEXT_LEFT);
    const char *quality[] = {"Low (N64)", "Medium", "High (GX)", "Ultra (4K Pack)"};
    sTextureQuality = nk_combo(ctx, quality, 4, sTextureQuality, 25, nk_vec2(200, 200));
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Selects the resolution of textures. Ultra requires the 4K Texture Pack.");

    nk_checkbox_label(ctx, "Enable Bloom", &sBloom);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Adds a glowing effect to bright areas (Planned).");

    nk_property_int(ctx, "Shadow Quality", 0, &sShadowQuality, 3, 1, 1);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Controls the resolution of dynamic shadows.");
}

void UI_Tab_Audio(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Audio Settings", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_property_int(ctx, "Master Volume", 0, &sMasterVolume, 100, 1, 1);
    nk_property_int(ctx, "Music Volume", 0, &sMusicVolume, 100, 1, 1);
    nk_property_int(ctx, "SFX Volume", 0, &sSFXVolume, 100, 1, 1);

    nk_checkbox_label(ctx, "Enable 3D Audio (HRTF)", &s3DAudio);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Enables binaural audio processing for headphones (Planned).");
}

void UI_Tab_Input(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Input Configuration", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Current Device: Keyboard", NK_TEXT_LEFT);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Button Mapping", NK_MAXIMIZED)) {
        nk_layout_row_dynamic(ctx, 25, 2);
        nk_label(ctx, "Accelerate (A):", NK_TEXT_LEFT); nk_button_label(ctx, "Z Key");
        nk_label(ctx, "Boost (B):", NK_TEXT_LEFT);      nk_button_label(ctx, "X Key");
        nk_label(ctx, "Drift Left (L):", NK_TEXT_LEFT);  nk_button_label(ctx, "A Key");
        nk_label(ctx, "Drift Right (R):", NK_TEXT_LEFT); nk_button_label(ctx, "S Key");
        nk_label(ctx, "Attack (Z):", NK_TEXT_LEFT);      nk_button_label(ctx, "D Key");
        nk_tree_pop(ctx);
    }

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Deadzone", NK_TEXT_LEFT);
    static float deadzone = 0.1f;
    nk_slider_float(ctx, 0.0f, &deadzone, 1.0f, 0.05f);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Adjusts the analog stick deadzone to prevent drift.");
}

void UI_Tab_Gameplay(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Gameplay Modifiers", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Physics Engine:", NK_TEXT_LEFT);
    const char *physics[] = {"Classic (N64)", "Modern (GX)", "Arcade (AX)"};
    sPhysicsMode = nk_combo(ctx, physics, 3, sPhysicsMode, 25, nk_vec2(200, 200));
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Changes the underlying physics engine. 'Modern' enables MTS (Momentum Turbo Slide).");

    nk_label(ctx, "Game Mode:", NK_TEXT_LEFT);
    if (nk_button_label(ctx, "Start Story Mode (Planned)")) {
        // Placeholder action
    }
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Launches the GX-inspired Story Mode campaign.");
}

void UI_Tab_Cheats(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Cheats & Modifications", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_checkbox_label(ctx, "God Mode (Invincibility)", &sGodMode);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Prevents all damage and infinite boost.");

    nk_checkbox_label(ctx, "Unlock All Content", &sUnlockAll);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, "Unlocks all machines, cups, and difficulties immediately.");
}

void UI_Tab_Debug(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Developer Tools", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_checkbox_label(ctx, "Enable Debug Overlay", &sDebugMode);

    if (nk_tree_push(ctx, NK_TREE_TAB, "Variable Watcher", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_label(ctx, "Player 1 X: 0.00", NK_TEXT_LEFT);
        nk_label(ctx, "Player 1 Y: 0.00", NK_TEXT_LEFT);
        nk_label(ctx, "Player 1 Z: 0.00", NK_TEXT_LEFT);
        nk_label(ctx, "Speed: 0 km/h", NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }
}

void UI_Tab_Help(struct nk_context *ctx) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Help & Manual", NK_TEXT_LEFT);

    nk_layout_row_dynamic(ctx, 400, 1);
    nk_edit_string_zero_terminated(ctx, NK_EDIT_BOX | NK_EDIT_MULTILINE | NK_EDIT_READ_ONLY, (char*)gManualText, 999999, nk_filter_default);
}

void UI_DrawTabs(struct nk_context *ctx) {
    if (nk_begin(ctx, "F-Zero X Settings", nk_rect(50, 50, 800, 600),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

        static int current_tab = 0;
        nk_layout_row_dynamic(ctx, 30, 8);

        // Tab Headers
        // Using buttons as tabs for simplicity
        if (nk_button_label(ctx, "General")) current_tab = 0;
        if (nk_button_label(ctx, "Video")) current_tab = 1;
        if (nk_button_label(ctx, "Audio")) current_tab = 2;
        if (nk_button_label(ctx, "Input")) current_tab = 3;
        if (nk_button_label(ctx, "Game")) current_tab = 4;
        if (nk_button_label(ctx, "Cheats")) current_tab = 5;
        if (nk_button_label(ctx, "Debug")) current_tab = 6;
        if (nk_button_label(ctx, "Help")) current_tab = 7;

        // Tab Content
        switch (current_tab) {
            case 0: UI_Tab_General(ctx); break;
            case 1: UI_Tab_Video(ctx); break;
            case 2: UI_Tab_Audio(ctx); break;
            case 3: UI_Tab_Input(ctx); break;
            case 4: UI_Tab_Gameplay(ctx); break;
            case 5: UI_Tab_Cheats(ctx); break;
            case 6: UI_Tab_Debug(ctx); break;
            case 7: UI_Tab_Help(ctx); break;
        }
    }
    nk_end(ctx);
}
