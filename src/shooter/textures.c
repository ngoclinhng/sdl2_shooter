#include <string.h>
#include <SDL2/SDL_image.h>
#include "shooter/textures.h"
#include "shooter/entity.h"

static const char* const textureFileNames[TEXTURE_COUNT] = {
  "gfx/player.png",
  "gfx/playerBullet.png",
  "gfx/enemy.png",
  "gfx/enemyBullet.png",
  "gfx/background.png"
};

static SDL_Texture* loadTexture(Textures* self, TextureType type);

void Textures_Init(Textures* self, SDL_Renderer* renderer) {
  memset(self, 0, sizeof(Textures));
  self->renderer = renderer;
}

void Textures_LoadAndStore(Textures* self, TextureType type) {
  loadTexture(self, type);  
}

void Textures_RenderFull(Textures* self, TextureType type, SDL_Rect* dst) {
  SDL_Texture* texture = loadTexture(self, type);

  if (texture == NULL) {
    return;
  }

  SDL_RenderCopy(self->renderer, texture, NULL, dst);
}

void Textures_RenderPortion(Textures* self, TextureType type, SDL_Rect* src,
			    SDL_Rect* dst) {
  SDL_Texture* texture = loadTexture(self, type);

  if (texture == NULL) {
    return;
  }

  SDL_RenderCopy(self->renderer, texture, src, dst);
}

void Textures_LoadTextureForEntity(Textures* self, Entity* entity) {
  TextureType type = entity->textureType;  
  SDL_Texture* texture = loadTexture(self, type);

  if (!texture)  {
    return;
  }

  int* w = &entity->hitbox.w;
  int* h = &entity->hitbox.h;
  SDL_QueryTexture(texture, NULL, NULL, w, h);
}

void Textures_RenderEntity(Textures* self, Entity* entity) {
  TextureType type = entity->textureType;
  SDL_Texture* texture = loadTexture(self, type);

  if (!texture) {
    return;
  }
  
  SDL_Rect destRect = entity->hitbox;
  SDL_RenderCopy(self->renderer, texture, NULL, &destRect);
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

static SDL_Texture* loadTexture(Textures* self, TextureType type) {
  if (type < 0 || type >= TEXTURE_COUNT) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid texture type\n");
    return NULL;
  }

  if (self->data[type] != NULL) {
    return self->data[type];
  }

  const char* filename = textureFileNames[type];
  SDL_Texture* newTexture = IMG_LoadTexture(self->renderer, filename);

  if (!newTexture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Unable to load texture from %s: %s\n",
		 filename, IMG_GetError());
    return NULL;
  }

  self->data[type] = newTexture;
  return newTexture;
}
