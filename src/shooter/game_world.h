#ifndef SHOOTER_GAME_WORLD_H_
#define SHOOTER_GAME_WORLD_H_

#include "shooter/input_manager.h"
#include "shooter/texture_manager.h"
#include "shooter/entity.h"

enum BulletOrigin {
  BULLET_FROM_PLAYER,
  BULLET_FROM_ENEMY
};

struct Player {
  struct Entity entity;
  int health;
  int reloadTime;
};

struct Enemy {
  struct Entity entity;
  int health;
  int reloadTime;
  struct Enemy* next;
};

struct Bullet {
  struct Entity entity;
  enum BulletOrigin origin;
  struct Bullet* next;
};

struct GameWorld {
  struct Player player;
  struct Enemy enemyHead, *enemyTail;
  struct Bullet bulletHead, *bulletTail;

  struct InputManager inputManager;
  struct TextureManager textureManager;
};

#define GAME_WORLD_ADD_BULLET(game, bullet)	\
  do {						\
    (game)->bulletTail->next = (bullet);	\
    (game)->bulletTail = (bullet);		\
  } while (0)

#define GAME_WORLD_ADD_ENEMY(game, enemy)	\
  do {						\
    (game)->enemyTail->next = (enemy);		\
    (game)->enemyTail = (enemy);		\
  } while (0)

void GameWorld_Init(struct GameWorld* game);
void GameWorld_PrepareScene(void);
void GameWorld_Update(struct GameWorld* game);
void GameWorld_Draw(struct GameWorld* game);
void GameWorld_PresentScene(void);
void GameWorld_Destroy(struct GameWorld* game);

#endif // SHOOTER_GAME_WORLD_H_
