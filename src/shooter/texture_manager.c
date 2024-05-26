#include <string.h>
#include <SDL2/SDL_image.h>
#include "shooter/texture_manager.h"

static const char* const textureFileNames[TEXTURE_COUNT] = {
  "gfx/player.png",
  "gfx/playerBullet.png",
  "gfx/enemy.png",
  "gfx/enemyBullet.png"
};

static SDL_Texture* loadTexture(struct TextureManager* self,
				enum TextureType type);

void TextureManager_Init(struct TextureManager* self,
			 SDL_Renderer* renderer) {
  memset(self, 0, sizeof(struct TextureManager));
  self->renderer = renderer;
}

void TextureManager_Load(struct TextureManager* self,
			 enum TextureType type,
			 int* width, int* height) {
  SDL_Texture* texture = loadTexture(self, type);

  if (!texture)  {
    return;
  }

  SDL_QueryTexture(texture, NULL, NULL, width, height);
}

void TextureManager_Render(struct TextureManager* self,
			   enum TextureType textureType,
			   float x, float y) {
  SDL_Texture* texture = loadTexture(self, textureType);

  if (!texture) {
    return;
  }
  
  SDL_Rect destRect = {x, y, 0, 0};
  SDL_QueryTexture(texture, NULL, NULL, &destRect.w, &destRect.h);
  SDL_RenderCopy(self->renderer, texture, NULL, &destRect);
}

void TextureManager_Destroy(struct TextureManager* self) {
  for (int i = 0; i < TEXTURE_COUNT; ++i) {
    if (self->textures[i]) {
      SDL_DestroyTexture(self->textures[i]);
      self->textures[i] = NULL;
    }
  }
}

static SDL_Texture* loadTexture(struct TextureManager* self,
				enum TextureType type) {
  if (type < 0 || type >= TEXTURE_COUNT) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Invalid texture type\n");
    return NULL;
  }

  if (self->textures[type]) {
    return self->textures[type];
  }

  const char* filename = textureFileNames[type];
  SDL_Texture* newTexture = IMG_LoadTexture(self->renderer, filename);

  if (!newTexture) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Unable to load texture from %s: %s\n",
		 filename, IMG_GetError());
    return NULL;
  }

  self->textures[type] = newTexture;
  return newTexture;
}
