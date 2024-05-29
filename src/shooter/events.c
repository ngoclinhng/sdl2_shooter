#include <string.h>
#include <SDL2/SDL.h>
#include "shooter/events.h"

static void updateEventsData(Events* events, SDL_Keycode key,
			     bool isPressed);

void Events_Init(Events* events) {
  memset(events, 0, sizeof(Events));
}

void Events_PollAndUpdate(Events* events) {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      events->data[EVENT_QUIT] = true;
    } else if (e.type == SDL_KEYDOWN && !e.key.repeat) {
      updateEventsData(events, e.key.keysym.sym, true);
    } else if (e.type == SDL_KEYUP) {
      updateEventsData(events, e.key.keysym.sym, false);
    }
  }
}

bool Events_IsActive(const Events* events, EventType type) {
  if (type < 0 || type >= EVENT_COUNT) {
    return false;
  }

  return events->data[type];
}


static void updateEventsData(Events* events, SDL_Keycode key,
			     bool isPressed) {
  switch (key) {
  case SDLK_UP:
    events->data[EVENT_UP] = isPressed;
    break;

  case SDLK_DOWN:
    events->data[EVENT_DOWN] = isPressed;
    break;
    
  case SDLK_LEFT:
    events->data[EVENT_LEFT] = isPressed;
    break;
    
  case SDLK_RIGHT:
    events->data[EVENT_RIGHT] = isPressed;
    break;

  case SDLK_LCTRL:
  case SDLK_SPACE:
    events->data[EVENT_FIRE] = isPressed;
    break;
    
  default: break;
  }
}
