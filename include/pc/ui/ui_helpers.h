#ifndef UI_HELPERS_H
#define UI_HELPERS_H

#include "pc/nuklear.h"

// Helper for tooltipped properties
void UI_Property_Int(struct nk_context* ctx, const char* label, int min, int* val, int max, int step, float inc_per_pixel, const char* tooltip);
void UI_Property_Float(struct nk_context* ctx, const char* label, float min, float* val, float max, float step, float inc_per_pixel, const char* tooltip);
void UI_Checkbox(struct nk_context* ctx, const char* label, int* active, const char* tooltip);
void UI_Header(struct nk_context* ctx, const char* title);
void UI_Label(struct nk_context* ctx, const char* label, nk_flags align, const char* tooltip);

#endif
