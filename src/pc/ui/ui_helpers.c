#include "pc/ui/ui_helpers.h"

void UI_Property_Int(struct nk_context* ctx, const char* label, int min, int* val, int max, int step, float inc_per_pixel, const char* tooltip) {
    nk_property_int(ctx, label, min, val, max, step, inc_per_pixel);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, tooltip);
}

void UI_Property_Float(struct nk_context* ctx, const char* label, float min, float* val, float max, float step, float inc_per_pixel, const char* tooltip) {
    nk_property_float(ctx, label, min, val, max, step, inc_per_pixel);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, tooltip);
}

void UI_Checkbox(struct nk_context* ctx, const char* label, int* active, const char* tooltip) {
    nk_checkbox_label(ctx, label, active);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, tooltip);
}

void UI_Header(struct nk_context* ctx, const char* title) {
    nk_layout_row_dynamic(ctx, 30, 1);
    nk_label(ctx, title, NK_TEXT_LEFT);
}

void UI_Label(struct nk_context* ctx, const char* label, nk_flags align, const char* tooltip) {
    nk_label(ctx, label, align);
    if (nk_widget_is_hovered(ctx)) nk_tooltip(ctx, tooltip);
}
