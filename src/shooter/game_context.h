#ifndef SHOOTER_GAME_CONTEXT_H_
#define SHOOTER_GAME_CONTEXT_H_

#include <SDL2/SDL.h>

typedef struct GameContext {
  SDL_Window* window;
  SDL_Renderer* renderer;
} GameContext;

GameContext* GameContext_Init(const char* title, int width, int height);
void GameContext_Free(GameContext* ctx);

#endif
