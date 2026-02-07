#include "pc/ui/ui_helpers.h"

// Netplay State
static char sPlayerName[32] = "Pilot 01";
static char sServerIP[32] = "127.0.0.1";
static int sRollbackFrames = 3;
static int sInputDelay = 2;
static int sSpectatorMode = 0;
static int sRegion = 0; // 0: NA, 1: EU, 2: JP

void UI_Tab_Netplay(struct nk_context *ctx) {
    (void)sRegion;
    UI_Header(ctx, "Online Multiplayer (30-Player Death Race)");
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Pilot Name:", NK_TEXT_LEFT);
    nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, sPlayerName, sizeof(sPlayerName), nk_filter_default);

    UI_Header(ctx, "Lobby Browser");
    nk_layout_row_dynamic(ctx, 150, 1);
    if (nk_group_begin(ctx, "ServerList", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 25, 3);
        nk_label(ctx, "Mute City Open", NK_TEXT_LEFT);
        nk_label(ctx, "24/30 Players", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "Join")) { /* Connect */ }

        nk_label(ctx, "Big Blue Time Attack", NK_TEXT_LEFT);
        nk_label(ctx, "5/30 Players", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "Join")) { /* Connect */ }

        nk_group_end(ctx);
    }

    UI_Header(ctx, "Direct Connection");
    nk_layout_row_dynamic(ctx, 30, 2);
    nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, sServerIP, sizeof(sServerIP), nk_filter_default);
    if (nk_button_label(ctx, "Connect IP")) { /* Connect */ }

    UI_Header(ctx, "Rollback Netcode Settings");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Int(ctx, "Rollback Frames", 0, &sRollbackFrames, 10, 1, 1, "Max frames to simulate backwards. Higher = smoother visuals but more CPU.");
    UI_Property_Int(ctx, "Input Delay (Frames)", 0, &sInputDelay, 5, 1, 1, "Fixed delay to hide lag. 0 = pure rollback (may teleport).");
    UI_Checkbox(ctx, "Spectator Mode Only", &sSpectatorMode, "Join as a camera drone to watch races.");
}
