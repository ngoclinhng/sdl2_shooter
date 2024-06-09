#ifndef SHOOTER_TEXTURES_H_
#define SHOOTER_TEXTURES_H_

#include <SDL2/SDL.h>

typedef enum TextureType {
  TEXTURE_PLAYER,
  TEXTURE_PLAYER_BULLET,
  TEXTURE_ENEMY,
  TEXTURE_ENEMY_BULLET,
  TEXTURE_EXPLOSION,
  TEXTURE_BACKGROUND,
  TEXTURE_COUNT
} TextureType;

typedef struct Textures {
  SDL_Texture* data[TEXTURE_COUNT];
  SDL_Renderer* renderer;
} Textures;

void Textures_Init(Textures* self, SDL_Renderer* renderer);
void Textures_Free(Textures* self);

void Textures_LoadAndStore(Textures* self, TextureType type);
void Textures_GetSize(Textures* self, TextureType type, int* w, int* h);
SDL_Texture* Textures_Get(Textures* self, TextureType type);

void Textures_RenderFull(Textures* self, TextureType type,
			 const SDL_Rect* dst);

void Textures_RenderPart(Textures* self, TextureType type,
			 const SDL_Rect* src,
			 const SDL_Rect* dst);

#endif // SHOOTER_TEXTURES_H_
