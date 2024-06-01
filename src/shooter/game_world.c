#include <string.h>
#include "shooter/defs.h"
#include "shooter/textures.h"
#include "shooter/game_world.h"

static const SDL_Rect playerClipRect = {
  0, 0,
  SHOOTER_WINDOW_WIDTH / 2, SHOOTER_WINDOW_HEIGHT
};

static const SDL_Rect windowBounds = {
  0, 0,
  SHOOTER_WINDOW_WIDTH, SHOOTER_WINDOW_HEIGHT
};

static Textures textures;

static void resetGameWorld(GameWorld* game);
static void initPlayer(GameWorld* game);
static void updatePlayer(GameWorld* game, const Events* events);
static void firePlayerBullet(GameWorld* game);
static void updateBullets(GameWorld* game);

static void moveEntity(Entity* entity);
static void drawEntity(Entity* entity);
static bool isBulletOutOfBounds(const Entity* bullet);

void GameWorld_Init(GameWorld* game, GameContext* context) {
  Textures_Init(&textures, context->renderer);
  memset(game, 0, sizeof(GameWorld));
  resetGameWorld(game);
}

void GameWorld_Free(GameWorld* game) {
  EntityList_Free(&game->bullets);
  Textures_Free(&textures);
}

void GameWorld_Update(GameWorld* game, const Events* events) {
  if (Events_IsActive(events, EVENT_QUIT)) {
    exit(0);
  }

  updatePlayer(game, events);
  updateBullets(game);
}

void GameWorld_Draw(GameWorld* game) {
  drawEntity(&game->player);
  EntityList_ForEach(&game->bullets, &drawEntity);
}

// Helpers

static void resetGameWorld(GameWorld* game) {
  initPlayer(game);
  EntityList_Free(&game->bullets);
}

static void initPlayer(GameWorld* game) {
  Entity* entity = &game->player;

  entity->type = ENTITY_PLAYER;
  entity->textureType = TEXTURE_PLAYER;
  entity->health = 1;
  entity->reloadTime = 0;

  Textures_Load(&textures, entity);
  Entity_Place(entity, 100, 100);  
}

static void updatePlayer(GameWorld* game, const Events* events) {
  Entity* entity = &game->player;

  entity->reloadTime--;
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

  if (Events_IsActive(events, EVENT_FIRE) && entity->reloadTime <= 0) {
    firePlayerBullet(game);
  }

  moveEntity(entity);
  Entity_Clip(entity, &playerClipRect);
}

static void firePlayerBullet(GameWorld* game) {
  Entity* bullet;
  bullet = EntityList_Add(&game->bullets, ENTITY_PLAYER_BULLET);

  Entity_PlaceAtCenter(bullet, &game->player);
  Entity_SetVelocity(bullet, SHOOTER_PLAYER_BULLET_SPEED, 0.0f);

  bullet->textureType = TEXTURE_PLAYER_BULLET;
  Textures_Load(&textures, bullet);

  game->player.reloadTime = 8;
}

static void updateBullets(GameWorld* game) {
  EntityList_ForEachAndPrune(&game->bullets,
			     &moveEntity,
			     &isBulletOutOfBounds);
}

static void moveEntity(Entity* entity) {
  Entity_Move(entity, 1.0f);
}

static void drawEntity(Entity* entity) {
  Textures_Render(&textures, entity);
}

static bool isBulletOutOfBounds(const Entity* bullet) {
  OutOfBoundsFlags flags = Entity_CheckOutOfBounds(bullet, &windowBounds);
  return flags != OUT_OF_BOUNDS_NONE;
}
