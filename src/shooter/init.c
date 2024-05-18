#include <stdlib.h>
#include <SDL2/SDL.h>
#include "shooter/structs.h"
#include "shooter/defs.h"
#include "shooter/init.h"

extern struct Shooter shooter;

void initSDL(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  shooter.window = SDL_CreateWindow("Shooter",
				    SDL_WINDOWPOS_UNDEFINED,
				    SDL_WINDOWPOS_UNDEFINED,
				    SHOOTER_WINDOW_WIDTH,
				    SHOOTER_WINDOW_HEIGHT,
				    SDL_WINDOW_SHOWN);

  if (!shooter.window) {
    SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  shooter.renderer = SDL_CreateRenderer(shooter.window,
					-1,
					SDL_RENDERER_ACCELERATED);
  if (!shooter.renderer) {
    SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }  
}

void cleanup(void) {
  SDL_DestroyRenderer(shooter.renderer);
  SDL_DestroyWindow(shooter.window);
  SDL_Quit();
}
