#ifndef SHOOTER_GAME_WORLD_H_
#define SHOOTER_GAME_WORLD_H_

#include "shooter/game_context.h"
#include "shooter/events.h"
#include "shooter/entity.h"

typedef struct GameWorld {
  Entity player;
} GameWorld;

void GameWorld_Init(GameWorld* game, GameContext* context);
void GameWorld_Free(GameWorld* game);

void GameWorld_Update(GameWorld* game, const Events* events);
void GameWorld_Draw(GameWorld* game);

#endif // SHOOTER_GAME_WORLD_H_
