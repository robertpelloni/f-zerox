#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <stdbool.h>
#include <stdint.h>
#include "pc/ultra64.h"

// MAX_BINDS: 2 keys per button
#define MAX_BINDS 2

typedef struct {
    uint32_t scancode; // SDL_Scancode
    uint16_t n64_button; // N64 Button Mask (e.g. CONT_A)
} KeyBind;

typedef struct {
    // Video
    int width;
    int height;
    bool fullscreen;
    bool vsync;
    int texture_quality; // 0-3
    int shadow_quality; // 0-3
    bool bloom;

    // Audio
    int master_volume; // 0-100
    int music_volume; // 0-100
    int sfx_volume; // 0-100
    bool audio_3d;

    // Input
    KeyBind binds[16][MAX_BINDS]; // Mapping for each N64 button
    float deadzone; // 0.0 - 1.0
    float sensitivity; // 0.1 - 2.0

    // Gameplay (GX/AX/Redout)
    int physics_mode; // 0: Classic, 1: GX, 2: AX
    float gravity_scale;
    float grip_loss_factor;
    float strafe_power;
    bool quick_turn;
    bool side_attack;
    bool spin_attack;
    bool shift_boost;
    bool shield_regen;
    float shield_regen_rate;

    // Visuals (Redout)
    float motion_blur;
    float chromatic_aberration;
    float lens_flare;
    float bloom_threshold;
    float dynamic_fov;
    float camera_shake;
    bool cockpit_view;
    int hud_style; // 0: Classic, 1: GX, 2: Redout

    // Garage
    float color_primary[3];
    float color_secondary[3];
    float color_tertiary[3];
    int emblem_layer;

    // Netplay
    char player_name[32];
    char server_ip[32];
    int rollback_frames;
    int input_delay;
    bool spectator_mode;

    // Cheats & Debug
    bool god_mode;
    bool unlock_all;
    bool debug_overlay;
} Config;

extern Config gConfig;

void Config_SetDefaults(void);
void Config_Save(const char* filename);
void Config_Load(const char* filename);

// Helper: Get binding for a specific N64 button index (0-15)
uint32_t Config_GetBind(int btn_idx, int bind_slot);
void Config_SetBind(int btn_idx, int bind_slot, uint32_t scancode);

#endif
