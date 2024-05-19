#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "shooter/structs.h"
#include "shooter/draw.h"

extern struct Shooter g_shooter;

void prepareScene(void) {
  SDL_SetRenderDrawColor(g_shooter.renderer, 32, 32, 32, 255);
  SDL_RenderClear(g_shooter.renderer);
}

void presentScene(void) {
  SDL_RenderPresent(g_shooter.renderer);
}

SDL_Texture* loadTexture(const char* filename) {
  if (!filename) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "filename is NULL");
    return NULL;
  }

  SDL_Texture* newTexture = IMG_LoadTexture(g_shooter.renderer, filename);

  if (!newTexture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Unable to load texture from %s: %s\n",
		 filename, IMG_GetError());
  }

  return newTexture;
}

void blit(SDL_Texture* texture, int x, int y) {
  int width, height;
  SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  
  SDL_Rect destRect = {x, y, width, height};  
  SDL_RenderCopy(g_shooter.renderer, texture, NULL, &destRect);
}
