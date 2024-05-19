#ifndef SHOOTER_STRUCTS_H_
#define SHOOTER_STRUCTS_H_

#include <SDL2/SDL.h>

enum GameAction {
  ACTION_UP,
  ACTION_DOWN,
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_FIRE,
  ACTION_COUNT
};

struct GameDelegate {
  void (*handleLogic)(void);
  void (*handleDraw)(void);
};

struct GameEngine {
  SDL_Renderer *renderer;
  SDL_Window *window;
  
  int actionStates[ACTION_COUNT];
  struct GameDelegate delegate;
};

struct GameEntity {
  float x, y;
  int w, h;
  float dx, dy;  

  int health;
  int reload;
  
  SDL_Texture* texture;
  struct GameEntity* next;
};

struct GameStage {
  struct GameEntity fightersHead;
  struct GameEntity* fightersTail;
  
  struct GameEntity bulletsHead;
  struct GameEntity* bulletsTail;
};

#endif // SHOOTER_STRUCTS_H_
