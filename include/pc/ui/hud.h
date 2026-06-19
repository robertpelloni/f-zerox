#ifndef HUD_H
#define HUD_H

#include "pc/physics.h"

struct nk_context;

void HUD_Render(struct nk_context* ctx, Vehicle* v);

#endif
