#include <stdlib.h>
#include <SDL2/SDL.h>

#include "shooter/structs.h"
#include "shooter/input.h"

extern struct GameEngine g_gameEngine;

static void updateActionStates(SDL_Keycode key, int state);

void processEvents(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      exit(0);
    } else if (event.type == SDL_KEYDOWN && !event.key.repeat) {
      updateActionStates(event.key.keysym.sym, 1);
    } else if (event.type == SDL_KEYUP) {
      updateActionStates(event.key.keysym.sym, 0);
    }
  }
}

static void updateActionStates(SDL_Keycode key, int state) {
  switch (key) {
  case SDLK_UP:
    g_gameEngine.actionStates[ACTION_UP] = state;
    break;

  case SDLK_DOWN:
    g_gameEngine.actionStates[ACTION_DOWN] = state;
    break;
    
  case SDLK_LEFT:
    g_gameEngine.actionStates[ACTION_LEFT] = state;
    break;
    
  case SDLK_RIGHT:
    g_gameEngine.actionStates[ACTION_RIGHT] = state;
    break;

  case SDLK_LCTRL:
    g_gameEngine.actionStates[ACTION_FIRE] = state;
    break;
    
  default: break;
  }
}
