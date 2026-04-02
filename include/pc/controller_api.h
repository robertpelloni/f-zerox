#ifndef CONTROLLER_API_H
#define CONTROLLER_API_H

#include <stdbool.h>
#include <stdint.h>

// Returns true if the controller API is currently waiting for a key press
bool Controller_IsBinding(void);

// Starts the binding process for a specific button and slot
void Controller_StartBinding(int btn_idx, int bind_slot);

// Cancels any active binding process
void Controller_CancelBinding(void);

// Processes an SDL Scancode if we are binding. Returns true if a bind occurred.
bool Controller_HandleBind(uint32_t scancode);

const char* Controller_GetButtonName(int btn_idx);
const char* Controller_GetKeyName(uint32_t scancode);

#endif
