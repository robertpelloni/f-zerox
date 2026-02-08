#include "pc/configfile.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Config gConfig;

// Default bindings (SDL ScanCodes)
// 0: A, 1: B, 2: Z, 3: Start, 4: U, 5: D, 6: L, 7: R
// 8: L_Trig, 9: R_Trig, 10: C-U, 11: C-D, 12: C-L, 13: C-R
static const int sDefaultBinds[16] = {
    29, // Z (A)
    27, // X (B)
    6,  // C (Z)
    40, // Enter (Start)
    82, // Up
    81, // Down
    80, // Left
    79, // Right
    4,  // A (L Trig)
    22, // S (R Trig)
    20, // Q (C-U)
    26, // W (C-D)
    8,  // E (C-L)
    21, // R (C-R)
    0, 0
};

void Config_SetDefaults(void) {
    gConfig.width = 1280;
    gConfig.height = 720;
    gConfig.fullscreen = false;
    gConfig.vsync = true;
    gConfig.texture_quality = 2; // High
    gConfig.shadow_quality = 1; // Medium
    gConfig.bloom = true;

    gConfig.master_volume = 80;
    gConfig.music_volume = 70;
    gConfig.sfx_volume = 100;
    gConfig.audio_3d = false;

    for (int i = 0; i < 16; i++) {
        gConfig.binds[i][0].scancode = sDefaultBinds[i];
        gConfig.binds[i][1].scancode = 0; // Secondary unbinded
    }
    gConfig.deadzone = 0.1f;
    gConfig.sensitivity = 1.0f;

    gConfig.physics_mode = 0; // Classic
    gConfig.gravity_scale = 1.0f;
    gConfig.grip_loss_factor = 1.0f;
    gConfig.strafe_power = 0.0f;
    gConfig.quick_turn = true;
    gConfig.side_attack = true;
    gConfig.spin_attack = false;
    gConfig.shift_boost = false;
    gConfig.shield_regen = false;
    gConfig.shield_regen_rate = 0.5f;

    gConfig.motion_blur = 0.5f;
    gConfig.chromatic_aberration = 0.0f;
    gConfig.lens_flare = 0.8f;
    gConfig.bloom_threshold = 0.7f;
    gConfig.dynamic_fov = 1.0f;
    gConfig.camera_shake = 0.5f;
    gConfig.cockpit_view = false;
    gConfig.hud_style = 0; // Classic

    gConfig.color_primary[0] = 0.8f; gConfig.color_primary[1] = 0.1f; gConfig.color_primary[2] = 0.1f;
    gConfig.color_secondary[0] = 0.1f; gConfig.color_secondary[1] = 0.1f; gConfig.color_secondary[2] = 0.8f;
    gConfig.color_tertiary[0] = 0.1f; gConfig.color_tertiary[1] = 0.1f; gConfig.color_tertiary[2] = 0.1f;
    gConfig.emblem_layer = 0;

    strncpy(gConfig.player_name, "Pilot 01", 31);
    strncpy(gConfig.server_ip, "127.0.0.1", 31);
    gConfig.rollback_frames = 3;
    gConfig.input_delay = 2;
    gConfig.spectator_mode = false;

    gConfig.god_mode = false;
    gConfig.unlock_all = false;
    gConfig.debug_overlay = false;
}

void Config_Save(const char* filename) {
    FILE* file = fopen(filename, "wb");
    if (file) {
        fwrite(&gConfig, sizeof(Config), 1, file);
        fclose(file);
        printf("Config saved to %s\n", filename);
    } else {
        printf("Failed to save config to %s\n", filename);
    }
}

void Config_Load(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file) {
        fread(&gConfig, sizeof(Config), 1, file);
        fclose(file);
        printf("Config loaded from %s\n", filename);
    } else {
        printf("Config file not found, creating defaults.\n");
        Config_SetDefaults();
        Config_Save(filename);
    }
}

uint32_t Config_GetBind(int btn_idx, int bind_slot) {
    if (btn_idx < 0 || btn_idx >= 16 || bind_slot < 0 || bind_slot >= MAX_BINDS) return 0;
    return gConfig.binds[btn_idx][bind_slot].scancode;
}

void Config_SetBind(int btn_idx, int bind_slot, uint32_t scancode) {
    if (btn_idx < 0 || btn_idx >= 16 || bind_slot < 0 || bind_slot >= MAX_BINDS) return;
    gConfig.binds[btn_idx][bind_slot].scancode = scancode;
}
