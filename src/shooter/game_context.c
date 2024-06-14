#include <string.h>
#include <assert.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "shooter/game_context.h"

void GameContext_Init(GameContext* ctx, const char* title, int width,
		      int height) {
  memset(ctx, 0, sizeof(GameContext));

  const int windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI;
  const int rendererFlags = SDL_RENDERER_ACCELERATED;  
  const int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL could not initialize! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  ctx->window = SDL_CreateWindow(title,
				 SDL_WINDOWPOS_UNDEFINED,
				 SDL_WINDOWPOS_UNDEFINED,
				 width, height, windowFlags);

  if (ctx->window == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Window could not be created! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }


  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  ctx->renderer = SDL_CreateRenderer(ctx->window, -1, rendererFlags);

  if (ctx->renderer == NULL) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Renderer could not be created! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }
 
  // Get true drawable area.
  // https://wiki.libsdl.org/SDL2/SDL_GetRendererOutputSize
  if (SDL_GetRendererOutputSize(ctx->renderer,
				&ctx->windowWidth,
				&ctx->windowHeight) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL_GetRendererOutputSize: %s",
		 SDL_GetError());
    ctx->windowWidth = width;
    ctx->windowHeight = height;
  }

  if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL_image could not initialize! SDL_image error: %s\n",
		 IMG_GetError());
    exit(1);
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Mix_OpenAudio: %s",
		 Mix_GetError());
    exit(1);
  }

  Mix_AllocateChannels(8);
}

void GameContext_PrepareScene(GameContext* ctx) {
  SDL_SetRenderDrawColor(ctx->renderer, 32, 32, 32, 255);
  SDL_RenderClear(ctx->renderer);
}

void GameContext_PresentScene(GameContext* ctx) {
    SDL_RenderPresent(ctx->renderer);
}

void GameContext_Free(GameContext* ctx) {
  if (ctx != NULL) {
    if (ctx->renderer != NULL) {
      SDL_DestroyRenderer(ctx->renderer);
    }

    if(ctx->window != NULL) {
      SDL_DestroyWindow(ctx->window);
    }
  }

  Mix_CloseAudio();
  IMG_Quit();
  SDL_Quit();
}
