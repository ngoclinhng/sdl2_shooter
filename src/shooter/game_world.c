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
static int enemySpawnTimer;

static void resetGameWorld(GameWorld* game);

static void initPlayer(GameWorld* game);
static void updatePlayer(GameWorld* game, const Events* events);

static void firePlayerBullet(GameWorld* game);
static void updateBullets(GameWorld* game);

static void spawnEnemy(GameWorld* game);
static void updateEnemies(GameWorld* game);

static void moveEntity(Entity* entity);
static void drawEntity(Entity* entity);

static bool isBulletOutOfBounds(const Entity* bullet);
static bool isEnemyOutOfBounds(const Entity* enemy);

void GameWorld_Init(GameWorld* game, GameContext* context) {
  Textures_Init(&textures, context->renderer);
  memset(game, 0, sizeof(GameWorld));
  resetGameWorld(game);
}

void GameWorld_Free(GameWorld* game) {
  EntityList_Free(&game->bullets);
  EntityList_Free(&game->enemies);
  Textures_Free(&textures);
}

void GameWorld_Update(GameWorld* game, const Events* events) {
  if (Events_IsActive(events, EVENT_QUIT)) {
    exit(0);
  }

  updatePlayer(game, events);
  updateEnemies(game);
  updateBullets(game);  
  spawnEnemy(game);
}

void GameWorld_Draw(GameWorld* game) {
  drawEntity(&game->player);
  EntityList_ForEach(&game->bullets, &drawEntity);
  EntityList_ForEach(&game->enemies, &drawEntity);
}

// Helpers

static void resetGameWorld(GameWorld* game) {
  initPlayer(game);
  EntityList_Free(&game->bullets);
  EntityList_Free(&game->enemies);

  enemySpawnTimer = 0;
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

  bullet->textureType = TEXTURE_PLAYER_BULLET;
  Textures_Load(&textures, bullet);
  
  Entity_PlaceAtCenter(bullet, &game->player);
  Entity_SetVelocity(bullet, SHOOTER_PLAYER_BULLET_SPEED, 0.0f);

  game->player.reloadTime = 8;
}

static void updateBullets(GameWorld* game) {
  EntityList_ForEachAndPrune(&game->bullets,
			     &moveEntity,
			     &isBulletOutOfBounds);
}

static void spawnEnemy(GameWorld* game) {
  if (--enemySpawnTimer <= 0) {
    Entity* enemy;
    enemy = EntityList_Add(&game->enemies, ENTITY_ENEMY);

    enemy->textureType = TEXTURE_ENEMY;
    Textures_Load(&textures, enemy);

    Entity_Place(enemy, windowBounds.w, rand() % windowBounds.h);
    Entity_SetVelocity(enemy, -(2 + (rand() % 4)), 0.0f);

    enemy->health = 1;
    enemy->reloadTime = SHOOTER_FPS * (1 + (rand() % 3));

    enemySpawnTimer = 30 + (rand() % SHOOTER_FPS);
  }
}

static void updateEnemies(GameWorld* game) {
  EntityList_ForEachAndPrune(&game->enemies,
			     &moveEntity,
			     &isEnemyOutOfBounds);
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

static bool isEnemyOutOfBounds(const Entity* enemy) {
  return Entity_IsToTheLeftOf(enemy, windowBounds.x);
}
