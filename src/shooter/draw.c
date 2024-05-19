#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "shooter/structs.h"
#include "shooter/draw.h"

extern struct GameEngine g_gameEngine;

void prepareScene(void) {
  SDL_SetRenderDrawColor(g_gameEngine.renderer, 32, 32, 32, 255);
  SDL_RenderClear(g_gameEngine.renderer);
}

void presentScene(void) {
  SDL_RenderPresent(g_gameEngine.renderer);
}

void blit(SDL_Texture* texture, int x, int y) {
  int width, height;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  
  SDL_Rect destRect = {x, y, width, height};  
  SDL_RenderCopy(g_gameEngine.renderer, texture, NULL, &destRect);
}
