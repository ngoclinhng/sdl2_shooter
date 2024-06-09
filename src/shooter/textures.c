#include <string.h>
#include <assert.h>
#include <SDL2/SDL_image.h>
#include "shooter/textures.h"

typedef struct TextureMap {
  TextureType type;
  char* filename;
} TextureMap;

static const TextureMap textureMaps[] = {
  {TEXTURE_PLAYER, "gfx/player.png"},
  {TEXTURE_PLAYER_BULLET, "gfx/playerBullet.png"},
  {TEXTURE_ENEMY, "gfx/enemy.png"},
  {TEXTURE_ENEMY_BULLET, "gfx/enemyBullet.png"},
  {TEXTURE_EXPLOSION, "gfx/explosion.png"},
  {TEXTURE_BACKGROUND, "gfx/background.png"}
};

static char* getTextureFileName(TextureType type);
static SDL_Texture* getTexture(Textures* self, TextureType type);

void Textures_Init(Textures* self, SDL_Renderer* renderer) {
  memset(self, 0, sizeof(Textures));
  self->renderer = renderer;
}

void Textures_LoadAndStore(Textures* self, TextureType type) {
  const char* filename = getTextureFileName(type);
  
  if (filename == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "No filename found for texture type %d",
		 type);
    return;
  }

  SDL_Texture* newTexture = IMG_LoadTexture(self->renderer, filename);

  if (!newTexture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Unable to load texture from %s: %s",
		 filename, IMG_GetError());
    return;
  }

  self->data[type] = newTexture;
}

void Textures_GetSize(Textures* self, TextureType type, int* w, int* h) {
  SDL_Texture* texture = getTexture(self, type);
  assert(texture != NULL);
  SDL_QueryTexture(texture, NULL, NULL, w, h);
}

SDL_Texture* Textures_Get(Textures* self, TextureType type) {
  SDL_Texture* texture = getTexture(self, type);
  assert(texture != NULL);
  return texture;
}

void Textures_RenderFull(Textures* self, TextureType type,
			 const SDL_Rect* dst) {
  SDL_Texture* texture = getTexture(self, type);
  assert(texture != NULL);  
  SDL_RenderCopy(self->renderer, texture, NULL, dst);
}

void Textures_RenderPart(Textures* self, TextureType type,
			 const SDL_Rect* src,
			 const SDL_Rect* dst) {
  SDL_Texture* texture = getTexture(self, type);
  assert(texture != NULL);
  SDL_RenderCopy(self->renderer, texture, src, dst);
}

void Textures_Free(Textures* self) {
  for (int i = 0; i < TEXTURE_COUNT; ++i) {
    SDL_Texture* texture = self->data[i];

    if (texture != NULL) {
      SDL_DestroyTexture(texture);
      texture = NULL;
    }
  }
}

static char* getTextureFileName(TextureType type) {
  size_t textureMapsSize = sizeof(textureMaps) / sizeof(TextureMap);

  for (size_t i = 0; i < textureMapsSize; ++i) {
    if (textureMaps[i].type == type) {
      return textureMaps[i].filename;
    }
  }

  return NULL;
}

static SDL_Texture* getTexture(Textures* self, TextureType type) {
  if (type < 0 || type >= TEXTURE_COUNT) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Invalid texture type %d",
		 type);
    return NULL;
  }

  return self->data[type];
}
