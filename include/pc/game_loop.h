#ifndef GAME_LOOP_H
#define GAME_LOOP_H

// The core update loop, simulates one N64 frame (16.6ms)
void Game_RunFrame(void);

// Initialize game state (physics, world, etc.)
void Game_Init(void);

#endif
