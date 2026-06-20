#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "pc/network/network.h"
#include <stdio.h>

void UI_Tab_Netplay(struct nk_context *ctx) {
    UI_Header(ctx, "Online Multiplayer (30-Player Death Race)");
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, "Pilot Name:", NK_TEXT_LEFT);
    nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, gConfig.player_name, sizeof(gConfig.player_name), nk_filter_default);

    UI_Header(ctx, "Lobby Participants (LAN Broadcast)");
    nk_layout_row_dynamic(ctx, 150, 1);
    if (nk_group_begin(ctx, "ParticipantList", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 25, 2);
        int connected_count = 0;
        for (int i = 0; i < 30; i++) {
            if (Net_IsMachineActive(i)) {
                char label[64];
                snprintf(label, sizeof(label), "Machine %d", i);
                nk_label(ctx, label, NK_TEXT_LEFT);
                nk_label(ctx, "Connected", NK_TEXT_RIGHT);
                connected_count++;
            }
        }

        if (connected_count == 0) {
            nk_label(ctx, "Waiting for players to join...", NK_TEXT_LEFT);
            nk_label(ctx, "", NK_TEXT_LEFT); // Empty spacer
        }
        nk_group_end(ctx);
    }

    nk_layout_row_dynamic(ctx, 30, 1);
    if (nk_button_label(ctx, "Broadcast Lobby Handshake")) {
        Net_ConnectLobby();
    }

    UI_Header(ctx, "Lobby Browser (Simulated)");
    nk_layout_row_dynamic(ctx, 100, 1);
    if (nk_group_begin(ctx, "ServerList", NK_WINDOW_BORDER)) {
        nk_layout_row_dynamic(ctx, 25, 3);
        nk_label(ctx, "Mute City Open", NK_TEXT_LEFT);
        nk_label(ctx, "24/30 Players", NK_TEXT_LEFT);
        if (nk_button_label(ctx, "Join")) { /* Connect */ }

        nk_group_end(ctx);
    }

    UI_Header(ctx, "Direct Connection");
    nk_layout_row_dynamic(ctx, 30, 2);
    nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, gConfig.server_ip, sizeof(gConfig.server_ip), nk_filter_default);
    if (nk_button_label(ctx, "Connect IP")) { /* Connect */ }

    UI_Header(ctx, "Rollback Netcode Settings");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Int(ctx, "Rollback Frames", 0, &gConfig.rollback_frames, 10, 1, 1, "Max frames to simulate backwards. Higher = smoother visuals but more CPU.");
    UI_Property_Int(ctx, "Input Delay (Frames)", 0, &gConfig.input_delay, 5, 1, 1, "Fixed delay to hide lag. 0 = pure rollback (may teleport).");

    int spectator = gConfig.spectator_mode;
    UI_Checkbox(ctx, "Spectator Mode Only", &spectator, "Join as a camera drone to watch races.");
    gConfig.spectator_mode = spectator;
}
