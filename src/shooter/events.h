#ifndef SHOOTER_EVENTS_H_
#define SHOOTER_EVENTS_H_

#include <stdbool.h>

typedef enum EventType {
  EVENT_UP,
  EVENT_DOWN,
  EVENT_LEFT,
  EVENT_RIGHT,
  EVENT_FIRE,
  EVENT_QUIT,
  EVENT_COUNT
} EventType;

typedef struct Events {
  bool data[EVENT_COUNT];
} Events;

void Events_Init(Events* events);
void Events_PollAndUpdate(Events* events);
bool Events_IsActive(const Events* events, EventType type);

#endif // SHOOTER_EVENTS_H_
