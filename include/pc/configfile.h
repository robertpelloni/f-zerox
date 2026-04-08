#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    // Window
    int width;
    int height;
    bool fullscreen;

    // Controls
    uint32_t binds[16];
    float deadzone;

    // Physics / Mechanics
    float gravity_scale;
    float grip_loss_factor;
    float strafe_power;
    bool quick_turn;
    bool side_attack;
    bool spin_attack;
    bool shift_boost;
    bool shield_regen;
    float shield_regen_rate;

    // Visuals / Camera
    float dynamic_fov;
    float camera_shake;
    bool cockpit_view;
    int hud_style;
    bool bilinear_filtering;
    float motion_blur;
    float chromatic_aberration;
    float lens_flare;
    float bloom_threshold;

    // Customization
    float color_primary[3];
    int emblem_layer;

    // Netplay & Game Settings
    char player_name[32];
    char server_ip[32];
    int rollback_frames;
    int input_delay;
    bool spectator_mode;

    // Game Rules
    int num_ai_machines; // 0 to 28 (Slot 29 is ghost)

} Config;

extern Config gConfig;

void Config_Load(void);
void Config_Save(void);
uint32_t Config_GetBind(int btn_idx, int slot);
void Config_SetBind(int btn_idx, int slot, uint32_t scancode);

#endif
