#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include "pc/physics.h" // For Vehicle

extern Vehicle* gPlayerVehicle;

void Game_Init(void);
void Game_RunFrame(void); // Legacy wrapper

// Split loop
void Game_Update(void);
void Game_Render(void);

#endif
