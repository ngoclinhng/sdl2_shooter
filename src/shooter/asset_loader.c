#include <SDL2/SDL_image.h>
#include "shooter/structs.h"
#include "shooter/asset_loader.h"

extern struct GameEngine g_gameEngine;

static const char* textureFilenames[TEXTURE_COUNT] = {
  "gfx/player.png",
  "gfx/playerBullet.png"
};

static SDL_Texture* textureRegistry[TEXTURE_COUNT] = {NULL};

SDL_Texture* loadTexture(enum TextureType type) {
  if (type < 0 || type >= TEXTURE_COUNT) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid texture type\n");
    return NULL;
  }

  if (textureRegistry[type] != NULL) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Using preloaded texture...");
    return textureRegistry[type];
  }

  const char* filename = textureFilenames[type];
  SDL_Texture* newTexture = IMG_LoadTexture(g_gameEngine.renderer, filename);

  if (!newTexture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Unable to load texture from %s: %s\n",
		 filename, IMG_GetError());
    return NULL;
  }

  textureRegistry[type] = newTexture;
  return newTexture;
}

void freeAllTextures(void) {
  for (int i = 0; i < TEXTURE_COUNT; ++i) {
    if (textureRegistry[i] != NULL) {
      SDL_DestroyTexture(textureRegistry[i]);
      textureRegistry[i] = NULL;
    }
  }
}
