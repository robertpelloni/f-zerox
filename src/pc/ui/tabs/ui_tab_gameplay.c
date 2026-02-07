#include "pc/ui/ui_helpers.h"

// Physics State
static float sGravityScale = 1.0f;
static float sGripLossFactor = 1.0f;
static float sStrafePower = 0.0f; // Redout Strafe

// Mechanics State
static int sQuickTurn = 1;
static int sSideAttack = 1;
static int sSpinAttack = 0; // GX/AX
static int sShiftBoost = 0; // Redout/Snaking

// Health State
static int sShieldRegen = 0; // Redout
static float sShieldRegenRate = 0.5f;

void UI_Tab_Gameplay(struct nk_context *ctx) {
    UI_Header(ctx, "Physics Engine Modifications");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Property_Float(ctx, "Gravity Scale", 0.1f, &sGravityScale, 5.0f, 0.1f, 0.01f, "Multiplies global gravity. Lower values for floaty jumps (AX), higher for heavy feel (GX).");
    UI_Property_Float(ctx, "Grip Loss Factor", 0.0f, &sGripLossFactor, 2.0f, 0.1f, 0.01f, "Controls how easily machines lose traction. 0.0 = On Rails, 2.0 = Ice.");
    UI_Property_Float(ctx, "Strafe Power", 0.0f, &sStrafePower, 100.0f, 1.0f, 0.1f, "Lateral thrust power (Redout style). Allows strafing without turning.");

    UI_Header(ctx, "Advanced Mechanics");
    nk_layout_row_dynamic(ctx, 25, 2);
    UI_Checkbox(ctx, "Enable Quick Turn", &sQuickTurn, "Classic F-Zero X quick turning (L+R).");
    UI_Checkbox(ctx, "Enable Side Attack", &sSideAttack, "Standard side attack functionality.");
    UI_Checkbox(ctx, "Enable Spin Attack", &sSpinAttack, "GX/AX Spin Attack. Damages enemies in a radius.");
    UI_Checkbox(ctx, "Enable Shift Boost", &sShiftBoost, "Gain speed when shifting weight during drifts (Snaking/Redout).");

    UI_Header(ctx, "Health & Shield System");
    nk_layout_row_dynamic(ctx, 25, 1);
    UI_Checkbox(ctx, "Enable Shield Regen", &sShieldRegen, "Shields regenerate over time (Redout style).");
    if (sShieldRegen) {
        UI_Property_Float(ctx, "Regen Rate", 0.1f, &sShieldRegenRate, 10.0f, 0.1f, 0.01f, "Speed of shield regeneration.");
    }
}
