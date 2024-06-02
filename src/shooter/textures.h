#ifndef SHOOTER_TEXTURES_H_
#define SHOOTER_TEXTURES_H_

#include <SDL2/SDL.h>

struct Entity;

typedef enum TextureType {
  TEXTURE_PLAYER,
  TEXTURE_PLAYER_BULLET,
  TEXTURE_ENEMY,
  TEXTURE_ENEMY_BULLET,
  TEXTURE_BACKGROUND,
  TEXTURE_COUNT
} TextureType;

typedef struct Textures {
  SDL_Texture* data[TEXTURE_COUNT];
  SDL_Renderer* renderer;
} Textures;

void Textures_Init(Textures* self, SDL_Renderer* renderer);
void Textures_Free(Textures* self);

void Textures_LoadTextureForEntity(Textures* self, struct Entity* entity);
void Textures_RenderEntity(Textures* self, struct Entity* entity);

void Textures_LoadAndStore(Textures* self, TextureType type);
void Textures_RenderFull(Textures* self, TextureType type, SDL_Rect* dst);
void Textures_RenderPortion(Textures* self, TextureType type, SDL_Rect* src,
			    SDL_Rect* dst);

#endif // SHOOTER_TEXTURES_H_
