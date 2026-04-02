#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/ui/ui_manual.h"
#include "pc/ui/ui_tabs_content.h"
#include "pc/ui/ui_manual.h"
#include "pc/ui/ui_tabs_content.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void UI_Tab_General(struct nk_context *ctx) {
    UI_Header(ctx, "F-Zero X PC Port - Status Dashboard");

    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, "Frame Rate: 60 FPS (Locked)", NK_TEXT_LEFT);
    nk_label(ctx, "Game State: Menu", NK_TEXT_LEFT);
    nk_label(ctx, "Uptime: 00:01:23", NK_TEXT_LEFT);

    if (nk_button_label(ctx, "Exit Game")) {
        Config_Save("fzerox_pc.bin");
        exit(0);
    }

    if (nk_button_label(ctx, "Reset Config to Defaults")) {
        Config_SetDefaults();
    }
}

void UI_Tab_Video(struct nk_context *ctx) {
    UI_Header(ctx, "Display Settings");

    nk_layout_row_dynamic(ctx, 25, 2);
    UI_Property_Int(ctx, "Width", 640, &gConfig.width, 3840, 10, 1, "Horizontal Resolution.");
    UI_Property_Int(ctx, "Height", 480, &gConfig.height, 2160, 10, 1, "Vertical Resolution.");

    nk_layout_row_dynamic(ctx, 25, 1);
    int fullscreen = gConfig.fullscreen;
    int vsync = gConfig.vsync;
    int bloom = gConfig.bloom;

    UI_Checkbox(ctx, "Fullscreen", &fullscreen, "Toggle fullscreen mode.");
    UI_Checkbox(ctx, "V-Sync", &vsync, "Vertical Synchronization to prevent tearing.");

    gConfig.fullscreen = fullscreen;
    gConfig.vsync = vsync;

    UI_Header(ctx, "Advanced Graphics");

    nk_layout_row_dynamic(ctx, 25, 1);
    nk_label(ctx, "Texture Quality:", NK_TEXT_LEFT);
    const char *quality[] = {"Low (N64)", "Medium", "High (GX)", "Ultra (4K Pack)"};
    gConfig.texture_quality = nk_combo(ctx, quality, 4, gConfig.texture_quality, 25, nk_vec2(200, 200));

    UI_Checkbox(ctx, "Enable Bloom", &bloom, "Adds a glowing effect to bright areas.");
    gConfig.bloom = bloom;

    UI_Property_Int(ctx, "Shadow Quality", 0, &gConfig.shadow_quality, 3, 1, 1, "Controls the resolution of dynamic shadows.");
}

void UI_Tab_Audio(struct nk_context *ctx) {
    UI_Header(ctx, "Audio Settings");

    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Int(ctx, "Master Volume", 0, &gConfig.master_volume, 100, 1, 1, "Global volume level.");
    UI_Property_Int(ctx, "Music Volume", 0, &gConfig.music_volume, 100, 1, 1, "Music volume level.");
    UI_Property_Int(ctx, "SFX Volume", 0, &gConfig.sfx_volume, 100, 1, 1, "Sound effects volume level.");

    int audio_3d = gConfig.audio_3d;
    UI_Checkbox(ctx, "Enable 3D Audio (HRTF)", &audio_3d, "Enables binaural audio processing for headphones.");
    gConfig.audio_3d = audio_3d;
}

void UI_Tab_Cheats(struct nk_context *ctx) {
    UI_Header(ctx, "Cheats & Modifications");

    nk_layout_row_dynamic(ctx, 25, 1);
    int god = gConfig.god_mode;
    int unlock = gConfig.unlock_all;

    UI_Checkbox(ctx, "God Mode (Invincibility)", &god, "Prevents all damage and infinite boost.");
    UI_Checkbox(ctx, "Unlock All Content", &unlock, "Unlocks all machines, cups, and difficulties immediately.");

    gConfig.god_mode = god;
    gConfig.unlock_all = unlock;
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

        nk_layout_row_dynamic(ctx, 30, 7);
        if (nk_button_label(ctx, "Video")) current_tab = 6;
        if (nk_button_label(ctx, "Audio")) current_tab = 7;
        if (nk_button_label(ctx, "Input")) current_tab = 8;
        if (nk_button_label(ctx, "Cheats")) current_tab = 9;
        if (nk_button_label(ctx, "Debug")) current_tab = 10;
        if (nk_button_label(ctx, "Manual")) current_tab = 11;
        if (nk_button_label(ctx, "Arcade")) current_tab = 12;

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
            case 12: UI_Tab_Arcade(ctx); break;
        }
    }
    nk_end(ctx);
}
