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

static Entity player;
static EntityList bullets;
static EntityList enemies;

static int enemySpawnTimer;

static void resetGameWorld();

static void initPlayer();
static void updatePlayer(const Events* events);

static void firePlayerBullet();
static void updateBullets();

static void spawnEnemy();
static void updateEnemies();

static void moveEntity(Entity* entity);
static void drawEntity(Entity* entity);

static bool isBulletOutOfBounds(const Entity* bullet);
static bool isEnemyOutOfBounds(const Entity* enemy);

void GameWorld_Init(GameContext* context) {
  Textures_Init(&textures, context->renderer);

  EntityList_Init(&bullets);
  EntityList_Init(&enemies);
  
  resetGameWorld();
}

void GameWorld_Free() {
  EntityList_Free(&bullets);
  EntityList_Free(&enemies);
  Textures_Free(&textures);
}

void GameWorld_Update(const Events* events) {
  if (Events_IsActive(events, EVENT_QUIT)) {
    exit(0);
  }

  updatePlayer(events);
  updateEnemies();
  updateBullets();  
  spawnEnemy();
}

void GameWorld_Draw() {
  drawEntity(&player);
  EntityList_ForEach(&bullets, &drawEntity);
  EntityList_ForEach(&enemies, &drawEntity);
}

// Helpers

static void resetGameWorld() {  
  initPlayer();

  EntityList_Free(&bullets);
  EntityList_Free(&enemies);

  enemySpawnTimer = 0;
}

static void initPlayer() {
  memset(&player, 0, sizeof(Entity));

  player.type = ENTITY_PLAYER;
  player.textureType = TEXTURE_PLAYER;
  player.health = 1;
  player.reloadTime = 0;

  Textures_Load(&textures, &player);
  Entity_Place(&player, 100, 100);  
}

static void updatePlayer(const Events* events) {
  player.reloadTime--;
  Entity_SetVelocity(&player, 0.0f, 0.0f);

  if (Events_IsActive(events, EVENT_UP)) {
    Entity_SetVelocityY(&player, -SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_DOWN)) {
    Entity_SetVelocityY(&player, SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_LEFT)) {
    Entity_SetVelocityX(&player, -SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_RIGHT)) {
    Entity_SetVelocityX(&player, SHOOTER_PLAYER_SPEED);
  }

  if (Events_IsActive(events, EVENT_FIRE) && player.reloadTime <= 0) {
    firePlayerBullet();
  }

  moveEntity(&player);
  Entity_Clip(&player, &playerClipRect);
}

static void firePlayerBullet() {
  Entity* bullet;
  bullet = EntityList_Add(&bullets, ENTITY_PLAYER_BULLET);

  bullet->textureType = TEXTURE_PLAYER_BULLET;
  Textures_Load(&textures, bullet);
  
  Entity_PlaceAtCenter(bullet, &player);
  Entity_SetVelocity(bullet, SHOOTER_PLAYER_BULLET_SPEED, 0.0f);

  player.reloadTime = 8;
}

static void updateBullets() {
  EntityList_ForEachAndPrune(&bullets, &moveEntity, &isBulletOutOfBounds);
}

static void spawnEnemy() {
  if (--enemySpawnTimer <= 0) {
    Entity* enemy;
    enemy = EntityList_Add(&enemies, ENTITY_ENEMY);

    enemy->textureType = TEXTURE_ENEMY;
    Textures_Load(&textures, enemy);

    Entity_Place(enemy, windowBounds.w, rand() % windowBounds.h);
    Entity_SetVelocity(enemy, -(2 + (rand() % 4)), 0.0f);

    enemy->health = 1;
    enemy->reloadTime = SHOOTER_FPS * (1 + (rand() % 3));

    enemySpawnTimer = 30 + (rand() % SHOOTER_FPS);
  }
}

static void updateEnemies() {
  EntityList_ForEachAndPrune(&enemies, &moveEntity, &isEnemyOutOfBounds);
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
