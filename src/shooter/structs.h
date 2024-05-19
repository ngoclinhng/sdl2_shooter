#ifndef SHOOTER_STRUCTS_H_
#define SHOOTER_STRUCTS_H_

#include <SDL2/SDL.h>

struct Shooter {
  SDL_Renderer *renderer;
  SDL_Window *window;

  int up;
  int down;
  int left;
  int right;

  int fire;
};

struct Entity {
  int x;
  int y;
  int dx;
  int dy;
  int health;
  SDL_Texture* texture;  
};

#endif // SHOOTER_STRUCTS_H_
