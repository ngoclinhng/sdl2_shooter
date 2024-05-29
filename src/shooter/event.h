#ifndef SHOOTER_EVENT_HANDLER_H_
#define SHOOTER_EVENT_HANDLER_H_

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

typedef struct Event {
  bool data[EVENT_COUNT];
} Event;

void Event_Init(Event* event);
void Event_PollAndUpdate(Event* event);
bool Event_IsActive(const Event* event, EventType type);

#endif // SHOOTER_EVENT_HANDLER_H_
