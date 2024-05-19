#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "shooter/structs.h"
#include "shooter/defs.h"
#include "shooter/init.h"
#include "shooter/asset_loader.h"

extern struct GameEngine g_gameEngine;

void initSDL(void) {
  const int windowFlags = SDL_WINDOW_SHOWN;
  const int rendererFlags = SDL_RENDERER_ACCELERATED;  
  const int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL could not initialize! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  g_gameEngine.window = SDL_CreateWindow("Shooter",
					 SDL_WINDOWPOS_UNDEFINED,
					 SDL_WINDOWPOS_UNDEFINED,
					 WINDOW_WIDTH,
					 WINDOW_HEIGHT,
					 windowFlags);

  if (!g_gameEngine.window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Window could not be created! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  g_gameEngine.renderer = SDL_CreateRenderer(g_gameEngine.window,
					     -1,
					     rendererFlags);

  if (!g_gameEngine.renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Renderer could not be created! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL_image could not initialize! SDL_image error: %s\n",
		 IMG_GetError());
    exit(1);
  }
}

void cleanup(void) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Cleaning up....\n");

  freeAllTextures();

  SDL_DestroyRenderer(g_gameEngine.renderer);
  SDL_DestroyWindow(g_gameEngine.window);

  IMG_Quit();
  SDL_Quit();
  
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Done!\n");
}
