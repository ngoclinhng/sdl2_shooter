#include <SDL2/SDL.h>
#include "shooter/structs.h"
#include "shooter/draw.h"

extern struct Shooter shooter;

void prepareScene(void) {
  SDL_SetRenderDrawColor(shooter.renderer, 96, 128, 255, 255);
  SDL_RenderClear(shooter.renderer);
}

void presentScene(void) {
  SDL_RenderPresent(shooter.renderer);
}
