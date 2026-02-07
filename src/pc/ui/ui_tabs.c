#include "pc/ui/ui_tabs.h"
#include "pc/ui/ui_manual.h"
#include "pc/hal.h"
#include "pc/nuklear.h"
#include "pc/ui/ui_tabs_content.h"
#include "pc/ui/ui_helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Mock Configuration State (Legacy)
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
static int sGodMode = 0;
static int sUnlockAll = 0;
static int sDebugMode = 0;

void UI_Tab_General(struct nk_context *ctx) {
    UI_Header(ctx, "F-Zero X PC Port - Status Dashboard");

    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, "Frame Rate: 60 FPS (Locked)", NK_TEXT_LEFT);
    nk_label(ctx, "Game State: Menu", NK_TEXT_LEFT);
    nk_label(ctx, "Uptime: 00:01:23", NK_TEXT_LEFT);

    if (nk_button_label(ctx, "Exit Game")) {
        exit(0);
    }
}

void UI_Tab_Video(struct nk_context *ctx) {
    UI_Header(ctx, "Display Settings");

    nk_layout_row_dynamic(ctx, 25, 2);
    UI_Property_Int(ctx, "Width", 640, &sResWidth, 3840, 10, 1, "Horizontal Resolution.");
    UI_Property_Int(ctx, "Height", 480, &sResHeight, 2160, 10, 1, "Vertical Resolution.");

    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Checkbox(ctx, "Fullscreen", &sFullscreen, "Toggle fullscreen mode.");
    UI_Checkbox(ctx, "V-Sync", &sVSync, "Vertical Synchronization to prevent tearing.");

    UI_Header(ctx, "Advanced Graphics");

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Texture Quality:", NK_TEXT_LEFT);
    const char *quality[] = {"Low (N64)", "Medium", "High (GX)", "Ultra (4K Pack)"};
    sTextureQuality = nk_combo(ctx, quality, 4, sTextureQuality, 25, nk_vec2(200, 200));

    UI_Checkbox(ctx, "Enable Bloom", &sBloom, "Adds a glowing effect to bright areas.");
    UI_Property_Int(ctx, "Shadow Quality", 0, &sShadowQuality, 3, 1, 1, "Controls the resolution of dynamic shadows.");
}

void UI_Tab_Audio(struct nk_context *ctx) {
    UI_Header(ctx, "Audio Settings");

    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Int(ctx, "Master Volume", 0, &sMasterVolume, 100, 1, 1, "Global volume level.");
    UI_Property_Int(ctx, "Music Volume", 0, &sMusicVolume, 100, 1, 1, "Music volume level.");
    UI_Property_Int(ctx, "SFX Volume", 0, &sSFXVolume, 100, 1, 1, "Sound effects volume level.");

    UI_Checkbox(ctx, "Enable 3D Audio (HRTF)", &s3DAudio, "Enables binaural audio processing for headphones.");
}

void UI_Tab_Input(struct nk_context *ctx) {
    UI_Header(ctx, "Input Configuration");

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

    UI_Header(ctx, "Sensitivity");
    static float deadzone = 0.1f;
    UI_Property_Float(ctx, "Deadzone", 0.0f, &deadzone, 1.0f, 0.05f, 0.01f, "Adjusts the analog stick deadzone to prevent drift.");
}

void UI_Tab_Cheats(struct nk_context *ctx) {
    UI_Header(ctx, "Cheats & Modifications");

    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Checkbox(ctx, "God Mode (Invincibility)", &sGodMode, "Prevents all damage and infinite boost.");
    UI_Checkbox(ctx, "Unlock All Content", &sUnlockAll, "Unlocks all machines, cups, and difficulties immediately.");
}

void UI_Tab_Debug(struct nk_context *ctx) {
    UI_Header(ctx, "Developer Tools");

    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Checkbox(ctx, "Enable Debug Overlay", &sDebugMode, "Show debug text.");

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
    UI_Header(ctx, "Help & Manual");

    nk_layout_row_dynamic(ctx, 400, 1);
    if (nk_group_begin(ctx, "ManualText", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 20, 1);
        nk_text_wrap(ctx, gManualText, strlen(gManualText));
        nk_group_end(ctx);
    }
}

void UI_DrawTabs(struct nk_context *ctx) {
    if (nk_begin(ctx, "F-Zero X Settings", nk_rect(50, 50, 900, 700),
        NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {

        static int current_tab = 0;
        nk_layout_row_dynamic(ctx, 30, 6);
        if (nk_button_label(ctx, "Dashboard")) current_tab = 0;
        if (nk_button_label(ctx, "Garage")) current_tab = 1;
        if (nk_button_label(ctx, "Gameplay")) current_tab = 2;
        if (nk_button_label(ctx, "Visuals")) current_tab = 3;
        if (nk_button_label(ctx, "Editor")) current_tab = 4;
        if (nk_button_label(ctx, "Netplay")) current_tab = 5;

        nk_layout_row_dynamic(ctx, 30, 6);
        if (nk_button_label(ctx, "Video")) current_tab = 6;
        if (nk_button_label(ctx, "Audio")) current_tab = 7;
        if (nk_button_label(ctx, "Input")) current_tab = 8;
        if (nk_button_label(ctx, "Cheats")) current_tab = 9;
        if (nk_button_label(ctx, "Debug")) current_tab = 10;
        if (nk_button_label(ctx, "Manual")) current_tab = 11;

        // Tab Content
        switch (current_tab) {
            case 0: UI_Tab_General(ctx); break;
            case 1: UI_Tab_Garage(ctx); break;
            case 2: UI_Tab_Gameplay(ctx); break;
            case 3: UI_Tab_Visuals(ctx); break;
            case 4: UI_Tab_Editor(ctx); break;
            case 5: UI_Tab_Netplay(ctx); break;
            case 6: UI_Tab_Video(ctx); break;
            case 7: UI_Tab_Audio(ctx); break;
            case 8: UI_Tab_Input(ctx); break;
            case 9: UI_Tab_Cheats(ctx); break;
            case 10: UI_Tab_Debug(ctx); break;
            case 11: UI_Tab_Help(ctx); break;
        }
    }
    nk_end(ctx);
}
