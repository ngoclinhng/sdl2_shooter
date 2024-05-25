#ifndef SHOOTER_GAME_WORLD_H_
#define SHOOTER_GAME_WORLD_H_

#include "shooter/input_manager.h"
#include "shooter/texture_manager.h"

struct Entity {
  float x, y;
  float dx, dy;
  int w, h;
  enum TextureType textureType;
};

struct Player {
  struct Entity entity;
  int health;
  int reloadTime;
};

struct Enemy {
  struct Entity entity;
  int health;
  struct Enemy* next;
};

struct Bullet {
  struct Entity entity;
  struct Bullet* next;
};

struct GameWorld {
  struct Player player;
  struct Enemy enemyHead, *enemyTail;
  struct Bullet bulletHead, *bulletTail;

  struct InputManager inputManager;
  struct TextureManager textureManager;
};

void GameWorld_Init(struct GameWorld* game);
void GameWorld_PrepareScene(void);
void GameWorld_Update(struct GameWorld* game);
void GameWorld_Draw(struct GameWorld* game);
void GameWorld_PresentScene(void);
void GameWorld_Destroy(struct GameWorld* game);

#endif // SHOOTER_GAME_WORLD_H_
