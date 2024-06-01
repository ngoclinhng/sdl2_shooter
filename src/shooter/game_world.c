#include <string.h>
#include "shooter/defs.h"
#include "shooter/textures.h"
#include "shooter/game_world.h"

static const SDL_Rect playerClipRect = {
  0, 0,
  SHOOTER_WINDOW_WIDTH / 2, SHOOTER_WINDOW_HEIGHT
};

static Textures textures;

static void resetGameWorld(GameWorld* game);
static void initPlayer(GameWorld* game);
static void updatePlayer(GameWorld* game, const Events* events);
static void drawPlayer(GameWorld* game);

void GameWorld_Init(GameWorld* game, GameContext* context) {
  Textures_Init(&textures, context->renderer);
  resetGameWorld(game);
}

void GameWorld_Free(GameWorld* game) {
  Textures_Free(&textures);
}

void GameWorld_Update(GameWorld* game, const Events* events) {
  if (Events_IsActive(events, EVENT_QUIT)) {
    exit(0);
  }

  updatePlayer(game, events);
}

void GameWorld_Draw(GameWorld* game) {
  drawPlayer(game);
}

// Helpers

static void resetGameWorld(GameWorld* game) {
  memset(game, 0, sizeof(GameWorld));
  initPlayer(game);
}

static void initPlayer(GameWorld* game) {
  Entity* entity = &game->player;

  entity->type = ENTITY_PLAYER;
  entity->textureType = TEXTURE_PLAYER;
  entity->health = 1;

  Textures_Load(&textures, entity);
  Entity_Place(entity, 100, 100);  
}

static void updatePlayer(GameWorld* game, const Events* events) {
  Entity* entity = &game->player;

  Entity_SetVelocity(entity, 0.0f, 0.0f);

  if (Events_IsActive(events, EVENT_UP)) {
    Entity_SetVelocityY(entity, -SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_DOWN)) {
    Entity_SetVelocityY(entity, SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_LEFT)) {
    Entity_SetVelocityX(entity, -SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_RIGHT)) {
    Entity_SetVelocityX(entity, SHOOTER_PLAYER_SPEED);
  }

  Entity_Move(entity, 1.0f);
  Entity_Clip(entity, &playerClipRect);
}

static void drawPlayer(GameWorld* game) {
  Textures_Render(&textures, &game->player);
}
