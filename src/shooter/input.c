#include <stdlib.h>
#include <SDL2/SDL.h>

#include "shooter/structs.h"
#include "shooter/input.h"

extern struct Shooter g_shooter;

void updateShooterMovement(SDL_Keycode key, int state) {
  switch (key) {
  case SDLK_UP:
    g_shooter.up = state;
    break;

  case SDLK_DOWN:
    g_shooter.down = state;
    break;
    
  case SDLK_LEFT:
    g_shooter.left = state;
    break;
    
  case SDLK_RIGHT:
    g_shooter.right = state;
    break;

  case SDLK_LCTRL:
    g_shooter.fire = state;
    break;
    
  default: break;
  }
}

void doInput(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      exit(0);
    } else if (event.type == SDL_KEYDOWN && !event.key.repeat) {
      updateShooterMovement(event.key.keysym.sym, 1);
    } else if (event.type == SDL_KEYUP) {
      updateShooterMovement(event.key.keysym.sym, 0);
    }
  }
}
