#ifndef PC_UI_H
#define PC_UI_H

#include <stdbool.h>

// Basic UI Interface
bool UI_Init(void);
void UI_Shutdown(void);
void UI_Render(void);

// Input forwarding (pass SDL_Event* as void*)
void UI_HandleEvent(void* event);

// Check if UI is capturing input (mouse or keyboard)
bool UI_IsCapturingInput(void);

#endif
