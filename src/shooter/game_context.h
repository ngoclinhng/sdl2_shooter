#ifndef SHOOTER_GAME_CONTEXT_H_
#define SHOOTER_GAME_CONTEXT_H_

#include <SDL2/SDL.h>

typedef struct GameContext {
  SDL_Window* window;
  SDL_Renderer* renderer;
} GameContext;

void GameContext_Init(GameContext* ctx, const char* title, int w, int h);
void GameContext_PrepareScene(GameContext* ctx);
void GameContext_PresentScene(GameContext* ctx);
void GameContext_Free(GameContext* ctx);

#endif
