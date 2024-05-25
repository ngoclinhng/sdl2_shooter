#ifndef SHOOTER_INPUT_MANAGER_H_
#define SHOOTER_INPUT_MANAGER_H_

#include <stdbool.h>

enum GameAction {
  GAME_ACTION_UP,
  GAME_ACTION_DOWN,
  GAME_ACTION_LEFT,
  GAME_ACTION_RIGHT,
  GAME_ACTION_FIRE,
  GAME_ACTION_QUIT,
  GAME_ACTION_COUNT
};

struct InputManager {
  bool gameActions[GAME_ACTION_COUNT];
};

void InputManager_Init(struct InputManager* im);
void InputManager_PollEventAndUpdate(struct InputManager* im);

#endif // SHOOTER_INPUT_MANAGER_H_
