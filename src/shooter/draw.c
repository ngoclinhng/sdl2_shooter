#include <SDL2/SDL.h>
#include "shooter/structs.h"
#include "shooter/draw.h"

extern struct Shooter g_shooter;

void prepareScene(void) {
  SDL_SetRenderDrawColor(g_shooter.renderer, 96, 128, 255, 255);
  SDL_RenderClear(g_shooter.renderer);
}

void presentScene(void) {
  SDL_RenderPresent(g_shooter.renderer);
}
