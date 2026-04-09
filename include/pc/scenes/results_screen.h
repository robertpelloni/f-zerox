#ifndef RESULTS_SCREEN_H
#define RESULTS_SCREEN_H

void Scene_Result_Update(void);
void Scene_Result_Render(void);

// Provide Nuklear overlay hook for the results screen
struct nk_context;
void Scene_Result_UI(struct nk_context* ctx);

#endif
