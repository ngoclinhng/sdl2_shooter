#include <string.h>
#include <SDL2/SDL.h>
#include "shooter/event.h"

static void updateEventData(Event* event, SDL_Keycode key, bool isPressed);

void Event_Init(Event* event) {
  memset(event, 0, sizeof(Event));
}

void Event_PollAndUpdate(Event* event) {
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      event->data[EVENT_QUIT] = true;
    } else if (e.type == SDL_KEYDOWN && !e.key.repeat) {
      updateEventData(event, e.key.keysym.sym, true);
    } else if (e.type == SDL_KEYUP) {
      updateEventData(event, e.key.keysym.sym, false);
    }
  }
}

bool Event_IsActive(const Event* event, EventType type) {
  if (type < 0 || type >= EVENT_COUNT) {
    return false;
  }

  return event->data[type];
}


static void updateEventData(Event* event, SDL_Keycode key, bool isPressed) {
  switch (key) {
  case SDLK_UP:
    event->data[EVENT_UP] = isPressed;
    break;

  case SDLK_DOWN:
    event->data[EVENT_DOWN] = isPressed;
    break;
    
  case SDLK_LEFT:
    event->data[EVENT_LEFT] = isPressed;
    break;
    
  case SDLK_RIGHT:
    event->data[EVENT_RIGHT] = isPressed;
    break;

  case SDLK_LCTRL:
  case SDLK_SPACE:
    event->data[EVENT_FIRE] = isPressed;
    break;
    
  default: break;
  }
}
