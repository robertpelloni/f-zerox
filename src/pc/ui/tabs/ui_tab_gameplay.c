#include "pc/ui/ui_helpers.h"
#include "pc/configfile.h"

void UI_Tab_Gameplay(struct nk_context *ctx) {
    UI_Header(ctx, "Physics Engine Modifications");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Gravity Scale", 0.1f, &gConfig.gravity_scale, 5.0f, 0.1f, 0.01f, "Multiplies global gravity. Lower values for floaty jumps (AX), higher for heavy feel (GX).");
    UI_Property_Float(ctx, "Grip Loss Factor", 0.0f, &gConfig.grip_loss_factor, 2.0f, 0.1f, 0.01f, "Controls how easily machines lose traction. 0.0 = On Rails, 2.0 = Ice.");
    UI_Property_Float(ctx, "Strafe Power", 0.0f, &gConfig.strafe_power, 100.0f, 1.0f, 0.1f, "Lateral thrust power (Redout style). Allows strafing without turning.");

    UI_Header(ctx, "Advanced Mechanics");
    nk_layout_row_dynamic(ctx, 25, 2);
    int quick_turn = gConfig.quick_turn;
    int side_attack = gConfig.side_attack;
    int spin_attack = gConfig.spin_attack;
    int shift_boost = gConfig.shift_boost;

    UI_Checkbox(ctx, "Enable Quick Turn", &quick_turn, "Classic F-Zero X quick turning (L+R).");
    UI_Checkbox(ctx, "Enable Side Attack", &side_attack, "Standard side attack functionality.");
    UI_Checkbox(ctx, "Enable Spin Attack", &spin_attack, "GX/AX Spin Attack. Damages enemies in a radius.");
    UI_Checkbox(ctx, "Enable Shift Boost", &shift_boost, "Gain speed when shifting weight during drifts (Snaking/Redout).");

    gConfig.quick_turn = quick_turn;
    gConfig.side_attack = side_attack;
    gConfig.spin_attack = spin_attack;
    gConfig.shift_boost = shift_boost;

    UI_Header(ctx, "Health & Shield System");
    nk_layout_row_dynamic(ctx, 25, 1);
    int shield_regen = gConfig.shield_regen;
    UI_Checkbox(ctx, "Enable Shield Regen", &shield_regen, "Shields regenerate over time (Redout style).");
    gConfig.shield_regen = shield_regen;

    if (gConfig.shield_regen) {
        UI_Property_Float(ctx, "Regen Rate", 0.1f, &gConfig.shield_regen_rate, 10.0f, 0.1f, 0.01f, "Speed of shield regeneration.");
    }
}
