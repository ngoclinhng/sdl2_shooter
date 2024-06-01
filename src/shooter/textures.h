#ifndef SHOOTER_TEXTURES_H_
#define SHOOTER_TEXTURES_H_

#include <SDL2/SDL.h>
#include "shooter/entity.h"

typedef struct Textures {
  SDL_Texture* data[TEXTURE_COUNT];
  SDL_Renderer* renderer;
} Textures;

void Textures_Init(Textures* self, SDL_Renderer* renderer);
void Textures_Load(Textures* self, Entity* entity);
void Textures_Render(Textures* self, Entity* entity);
void Textures_Free(Textures* self);

#endif // SHOOTER_TEXTURES_H_
