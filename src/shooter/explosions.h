#ifndef SHOOTER_EXPLOSIONS_H_
#define SHOOTER_EXPLOSIONS_H_

#include <SDL2/SDL.h>

typedef enum ExplosionType {
  EXPLOSION_RED,
  EXPLOSION_BLUE
} ExplosionType;

void Explosions_Init(void);
void Explosions_Free(void);

void Explosions_Add(ExplosionType type, int x, int y);
void Explosions_Update(void);
void Explosions_Draw(SDL_Renderer* renderer, SDL_Texture* texture);

#endif // SHOOTER_EXPLOSIONS_H_
