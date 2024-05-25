#ifndef SHOOTER_TEXTURE_MANAGER_H_
#define SHOOTER_TEXTURE_MANAGER_H_

#include <SDL2/SDL.h>

enum TextureType {
  TEXTURE_PLAYER,
  TEXTURE_PLAYER_BULLET,
  TEXTURE_ENEMY,
  TEXTURE_COUNT
};

struct TextureManager {
  SDL_Texture* textures[TEXTURE_COUNT];
  SDL_Renderer* renderer;
};

void TextureManager_Init(struct TextureManager* self,
			 SDL_Renderer* renderer);

void TextureManager_Load(struct TextureManager* self,
			 enum TextureType type,
			 int* width, int* height);

void TextureManager_Render(struct TextureManager* self,
			   enum TextureType type,
			   float x, float y);

void TextureManager_Destroy(struct TextureManager* self);

#endif // SHOOTER_TEXTURE_MANAGER_H_
