#ifndef SHOOTER_GAME_WORLD_H_
#define SHOOTER_GAME_WORLD_H_

#include "shooter/game_context.h"
#include "shooter/events.h"
#include "shooter/entity.h"

void GameWorld_Init(GameContext* context);
void GameWorld_Free();

void GameWorld_Update(const Events* events);
void GameWorld_Draw();

#endif // SHOOTER_GAME_WORLD_H_
