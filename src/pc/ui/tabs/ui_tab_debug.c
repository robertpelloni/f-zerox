#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"
#include "variables.h"
#include "pc/assets/dynamic_asset_loader.h"
#include <stdio.h>

// Mock Game State for Debugging (since we don't have full decomp yet)
static float sMockPlayerX = 100.0f;
static float sMockPlayerY = 50.0f;
static float sMockPlayerZ = -200.0f;
static float sMockSpeed = 1200.0f;

extern void System_Init(void);
extern void GameState_RotateCycle(void);

void UI_Tab_Debug(struct nk_context *ctx) {
    UI_Header(ctx, "Developer Tools");

    nk_layout_row_dynamic(ctx, 25, 1);
    int overlay = gConfig.debug_overlay;
    UI_Checkbox(ctx, "Enable Debug Overlay", &overlay, "Show debug text.");
    gConfig.debug_overlay = overlay;

    if (nk_tree_push(ctx, NK_TREE_TAB, "Decompilation & Shiftability Monitor", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 20, 1);

        char initBuf[96];
        snprintf(initBuf, sizeof(initBuf), "System Init: 0x%08X (Ready: %s)",
                 gSystemInitialized,
                 gSystemFullyInitialized ? "YES" : "NO");
        UI_Label(ctx, initBuf, NK_TEXT_LEFT, "Status of authentic N64 System_Init boot state (magic 0x20DE1529).");

        char stateBuf[64];
        snprintf(stateBuf, sizeof(stateBuf), "State Index: %d | Active: %d",
                 gCurrentStateIndex, gStateMachineActive);
        UI_Label(ctx, stateBuf, NK_TEXT_LEFT, "Current N64 state machine index and active execution flag.");

        char cycleBuf[96];
        snprintf(cycleBuf, sizeof(cycleBuf), "Cycle A:%d B:%d C:%d",
                 gGameStateCycleA, gGameStateCycleB, gGameStateCycleC);
        UI_Label(ctx, cycleBuf, NK_TEXT_LEFT, "Shifted 3-register cycle (formerly D_800DCD04-0C) in game_1AE0.c.");

        char audioStateBuf[96];
        snprintf(audioStateBuf, sizeof(audioStateBuf), "Audio State Act:%d Play:%d Reset:%d",
                 gAudioStateActiveFlag, gAudioStatePlayTrigger, gAudioStateResetFlag);
        UI_Label(ctx, audioStateBuf, NK_TEXT_LEFT, "Shifted audio state flags (formerly D_800D1A48, D_800D19E0, D_800D1A0C).");

        char assetBuf[64];
        snprintf(assetBuf, sizeof(assetBuf), "Mapped Assets: %d", Asset_GetMappingCount());
        UI_Label(ctx, assetBuf, NK_TEXT_LEFT, "Active count of dynamically relocated ROM address asset mappings.");

        nk_layout_row_dynamic(ctx, 25, 4);
        if (nk_button_label(ctx, "Re-trigger System_Init")) {
            System_Init();
        }
        if (nk_button_label(ctx, "Step Rotate Cycle")) {
            GameState_RotateCycle();
        }
        if (nk_button_label(ctx, "Toggle Audio Trigger")) {
            extern void AudioState_TriggerPlay(void);
            AudioState_TriggerPlay();
        }
        if (nk_button_label(ctx, "Clear Asset Mappings")) {
            Asset_ClearMappings();
        }

        nk_tree_pop(ctx);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "Variable Watcher (Live)", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 20, 1);

        char buf[64];
        snprintf(buf, 64, "Player 1 X: %.2f", sMockPlayerX);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        snprintf(buf, 64, "Player 1 Y: %.2f", sMockPlayerY);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        snprintf(buf, 64, "Player 1 Z: %.2f", sMockPlayerZ);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        snprintf(buf, 64, "Speed: %.0f km/h", sMockSpeed);
        nk_label(ctx, buf, NK_TEXT_LEFT);

        // Mock Updates
        sMockPlayerZ += sMockSpeed * 0.016f;
        if (sMockPlayerZ > 1000.0f) sMockPlayerZ = -1000.0f;

        nk_tree_pop(ctx);
    }

    if (nk_tree_push(ctx, NK_TREE_TAB, "Memory Editor (Mock)", NK_MINIMIZED)) {
        nk_layout_row_dynamic(ctx, 25, 2);
        nk_label(ctx, "Address: 0x80001000", NK_TEXT_LEFT);
        nk_label(ctx, "Value: 0xDEADBEEF", NK_TEXT_LEFT);
        nk_tree_pop(ctx);
    }
}
