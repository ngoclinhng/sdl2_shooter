#ifndef SHOOTER_ASSET_LOADER_H_
#define SHOOTER_ASSET_LOADER_H_

#include <SDL2/SDL.h>

enum TextureType {
  TEXTURE_PLAYER,
  TEXTURE_PLAYER_BULLET,
  TEXTURE_COUNT
};

SDL_Texture* loadTexture(enum TextureType type);
void freeAllTextures(void);

#endif // SHOOTER_ASSET_LOADER_H_
