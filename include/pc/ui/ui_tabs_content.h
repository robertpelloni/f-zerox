#ifndef UI_TABS_CONTENT_H
#define UI_TABS_CONTENT_H

#include "pc/nuklear.h"

void UI_Tab_General(struct nk_context *ctx);
void UI_Tab_Video(struct nk_context *ctx);
void UI_Tab_Audio(struct nk_context *ctx);
void UI_Tab_Input(struct nk_context *ctx);
void UI_Tab_Cheats(struct nk_context *ctx);
void UI_Tab_Debug(struct nk_context *ctx);
void UI_Tab_Help(struct nk_context *ctx);

// New Tabs
void UI_Tab_Gameplay(struct nk_context *ctx);
void UI_Tab_Visuals(struct nk_context *ctx);
void UI_Tab_Garage(struct nk_context *ctx);
void UI_Tab_Netplay(struct nk_context *ctx);
void UI_Tab_Editor(struct nk_context *ctx);

#endif
