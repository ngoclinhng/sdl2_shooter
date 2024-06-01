#ifndef SHOOTER_TEXTURES_H_
#define SHOOTER_TEXTURES_H_

#include <SDL2/SDL.h>

typedef enum TextureType {
  TEXTURE_PLAYER,
  TEXTURE_PLAYER_BULLET,
  TEXTURE_ENEMY,
  TEXTURE_ENEMY_BULLET,
  TEXTURE_COUNT
} TextureType;

typedef struct Textures {
  SDL_Texture* data[TEXTURE_COUNT];
  SDL_Renderer* renderer;
} Textures;

void Textures_Init(Textures* self, SDL_Renderer* renderer);
void Textures_Load(Textures* self, TextureType type, int* w, int* h);
void Textures_Render(Textures* self, TextureType type, float x, float y);
void Textures_Free(Textures* self);

#endif // SHOOTER_TEXTURES_H_
