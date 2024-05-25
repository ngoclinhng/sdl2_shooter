#include <string.h>
#include <SDL2/SDL.h>
#include "shooter/input_manager.h"

static void updateGameActions(struct InputManager* im,
			      SDL_Keycode key,
			      bool isPressed);

void InputManager_Init(struct InputManager* im) {
  memset(im, 0, sizeof(struct InputManager));
}

void InputManager_PollEventAndUpdate(struct InputManager* im) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      im->gameActions[GAME_ACTION_QUIT] = true;
    } else if (event.type == SDL_KEYDOWN && !event.key.repeat) {
      updateGameActions(im, event.key.keysym.sym, true);
    } else if (event.type == SDL_KEYUP) {
      updateGameActions(im, event.key.keysym.sym, false);
    }
  }
}

static void updateGameActions(struct InputManager* im,
			      SDL_Keycode key,
			      bool isPressed) {
  switch (key) {
  case SDLK_UP:
    im->gameActions[GAME_ACTION_UP] = isPressed;
    break;

  case SDLK_DOWN:
    im->gameActions[GAME_ACTION_DOWN] = isPressed;
    break;
    
  case SDLK_LEFT:
    im->gameActions[GAME_ACTION_LEFT] = isPressed;
    break;
    
  case SDLK_RIGHT:
    im->gameActions[GAME_ACTION_RIGHT] = isPressed;
    break;

  case SDLK_LCTRL:
    im->gameActions[GAME_ACTION_FIRE] = isPressed;
    break;
    
  default: break;
  }
}
