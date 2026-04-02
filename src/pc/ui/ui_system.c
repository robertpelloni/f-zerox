#include "pc/ui/ui.h"
#include "pc/hal.h"
#include "pc/ui/ui_tabs.h"
#include "pc/ui/hud.h"
#include "pc/game_loop.h" // For gPlayerVehicle

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL2_IMPLEMENTATION
#include "pc/nuklear.h"
#include "pc/nuklear_sdl_gl2.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

// Fallback if not compiled with Makefile macro
#ifndef PROJECT_VERSION
#define PROJECT_VERSION "Unknown Version"
#endif

static struct nk_context *ctx;

bool UI_Init(void) {
    SDL_Window* win = (SDL_Window*)HAL_Video_GetWindow();
    if (!win) {
        printf("UI Error: No SDL Window available.\n");
        return false;
    }

    ctx = nk_sdl_init(win);

    // Load default font
    {
        struct nk_font_atlas *atlas;
        nk_sdl_font_stash_begin(&atlas);
        // Load default font
        nk_sdl_font_stash_end();
    }

    // Set custom style to match F-Zero X aesthetics (Dark Blue/Cyberpunk)
    struct nk_color table[NK_COLOR_COUNT];
    table[NK_COLOR_TEXT] = nk_rgba(255, 255, 255, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(20, 20, 40, 240);
    table[NK_COLOR_HEADER] = nk_rgba(40, 40, 80, 255);
    table[NK_COLOR_BORDER] = nk_rgba(100, 100, 200, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(50, 50, 100, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(70, 70, 140, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(90, 90, 180, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(50, 50, 100, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(70, 70, 140, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(100, 100, 200, 255);
    table[NK_COLOR_SELECT] = nk_rgba(50, 50, 100, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(90, 90, 180, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(50, 50, 100, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(100, 100, 200, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(120, 120, 220, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(140, 140, 240, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(50, 50, 100, 255);
    table[NK_COLOR_EDIT] = nk_rgba(40, 40, 80, 255);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(200, 200, 200, 255);
    table[NK_COLOR_COMBO] = nk_rgba(40, 40, 80, 255);
    table[NK_COLOR_CHART] = nk_rgba(50, 50, 100, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(100, 100, 200, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba(255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 30, 60, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(60, 60, 120, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(80, 80, 160, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(100, 100, 200, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(40, 40, 80, 255);

    nk_style_from_table(ctx, table);

    return true;
}

void UI_Shutdown(void) {
    nk_sdl_shutdown();
}

extern Vehicle gPlayerVehicle;

void UI_Render(void) {
    if (!ctx) return;

    HUD_Render(ctx, &gPlayerVehicle);

    // Render Version Number (Watermark)
    struct nk_style_window original_style = ctx->style.window;
    ctx->style.window.background = nk_rgba(0, 0, 0, 0);
    ctx->style.window.fixed_background = nk_style_item_color(nk_rgba(0,0,0,0));
    ctx->style.window.border = 0.0f;

    if (nk_begin(ctx, "VersionWatermark", nk_rect(10, 10, 200, 40), NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_NO_INPUT)) {
        nk_layout_row_dynamic(ctx, 30, 1);
        char verStr[64];
        snprintf(verStr, 64, "F-Zero X PC Port v%s", PROJECT_VERSION);
        nk_label_colored(ctx, verStr, NK_TEXT_LEFT, nk_rgb(150, 150, 150));
    }
    nk_end(ctx);
    ctx->style.window = original_style;

    UI_DrawTabs(ctx);
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void UI_HandleEvent(void* event) {
    if (!ctx) return;
    nk_sdl_handle_event((SDL_Event*)event);
}

bool UI_IsCapturingInput(void) {
    if (!ctx) return false;
    return nk_window_is_any_hovered(ctx) || nk_item_is_any_active(ctx);
}
