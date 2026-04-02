#include "pc/controller_api.h"
#include "pc/configfile.h"
#include <SDL2/SDL.h>

static int sBindingButton = -1;
static int sBindingSlot = -1;

const char* sButtonNames[16] = {
    "A Button", "B Button", "Z Trigger", "Start",
    "D-Pad Up", "D-Pad Down", "D-Pad Left", "D-Pad Right",
    "L Trigger", "R Trigger",
    "C Up", "C Down", "C Left", "C Right",
    "Unknown", "Unknown"
};

bool Controller_IsBinding(void) {
    return sBindingButton != -1;
}

void Controller_StartBinding(int btn_idx, int bind_slot) {
    sBindingButton = btn_idx;
    sBindingSlot = bind_slot;
}

void Controller_CancelBinding(void) {
    sBindingButton = -1;
    sBindingSlot = -1;
}

bool Controller_HandleBind(uint32_t scancode) {
    if (!Controller_IsBinding()) return false;

    // Ignore Escape (Cancel)
    if (scancode == SDL_SCANCODE_ESCAPE) {
        Controller_CancelBinding();
        return true;
    }

    Config_SetBind(sBindingButton, sBindingSlot, scancode);
    Controller_CancelBinding();
    return true;
}

const char* Controller_GetButtonName(int btn_idx) {
    if (btn_idx < 0 || btn_idx >= 16) return "Unknown";
    return sButtonNames[btn_idx];
}

const char* Controller_GetKeyName(uint32_t scancode) {
    if (scancode == 0) return "None";
    return SDL_GetScancodeName((SDL_Scancode)scancode);
}
