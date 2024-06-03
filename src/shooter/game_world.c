#include <string.h>
#include "shooter/defs.h"
#include "shooter/game_world.h"
#include "shooter/utils.h"

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
static int gameWorldResetTimer;
static int backgroundX;

static void resetGameWorld(void);

static void updateBackground(void);
static void initPlayer(void);
static void updatePlayer(const Events* events);

static void firePlayerBullet(void);
static void updateBullet(Entity* bullet);
static void updateBullets(void);

static void spawnEnemy(void);
static void updateEnemy(Entity* enemy);
static void updateEnemies(void);
static void fireEnemyBullet(const Entity* enemy);

static void moveEntity(Entity* entity);
static void drawEntity(Entity* entity);
static void drawBackground(void);

static bool isBulletOutOfBoundsOrDead(const Entity* bullet);
static bool isEnemyOutOfBoundsOrDead(const Entity* enemy);
static void checkCollision(Entity* e1, Entity* e2);

void GameWorld_Init(GameContext* context) {
  Textures_Init(&textures, context->renderer);

  Textures_LoadAndStore(&textures, TEXTURE_PLAYER);
  Textures_LoadAndStore(&textures, TEXTURE_PLAYER_BULLET);
  Textures_LoadAndStore(&textures, TEXTURE_ENEMY);
  Textures_LoadAndStore(&textures, TEXTURE_ENEMY_BULLET);
  Textures_LoadAndStore(&textures, TEXTURE_BACKGROUND);

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

  updateBackground();
  updatePlayer(events);
  updateEnemies();
  updateBullets();  
  spawnEnemy();

  if (player.health <= 0 && --gameWorldResetTimer <= 0) {
    resetGameWorld();
  }
}

void GameWorld_Draw() {
  drawBackground();
  drawEntity(&player);
  EntityList_ForEach(&bullets, &drawEntity);
  EntityList_ForEach(&enemies, &drawEntity);
}

// Helpers

static void resetGameWorld(void) {  
  initPlayer();

  EntityList_Free(&bullets);
  EntityList_Free(&enemies);

  enemySpawnTimer = 0;
  gameWorldResetTimer = SHOOTER_FPS * 2;
  backgroundX = 0;
}

static void initPlayer(void) {
  memset(&player, 0, sizeof(Entity));

  player.type = ENTITY_PLAYER;
  player.health = 1;
  player.reloadTime = 0;
  
  Entity_SetTexture(&player, &textures, TEXTURE_PLAYER);
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

static void firePlayerBullet(void) {
  Entity* bullet;
  bullet = EntityList_Add(&bullets, ENTITY_PLAYER_BULLET);

  Entity_SetTexture(bullet, &textures, TEXTURE_PLAYER_BULLET);
  Entity_PlaceAtCenter(bullet, &player);
  Entity_SetVelocity(bullet, SHOOTER_PLAYER_BULLET_SPEED, 0.0f);

  bullet->health = 1;
  player.reloadTime = 8;
}

static void updateBullets(void) {
  EntityList_ForEachAndPrune(&bullets,
			     &updateBullet,
			     &isBulletOutOfBoundsOrDead);
}

static void updateBullet(Entity* bullet) {
  moveEntity(bullet);
  
  if (bullet->type == ENTITY_ENEMY_BULLET &&
      Entity_CheckCollision(bullet, &player)) {    
    player.health = 0;
    bullet->health = 0;
    return;
  }

  if (bullet->type == ENTITY_PLAYER_BULLET) {       
    EntityList_ForEachWith(&enemies, bullet, &checkCollision);
  }
}

static void spawnEnemy(void) {
  if (--enemySpawnTimer <= 0) {
    Entity* enemy;
    enemy = EntityList_Add(&enemies, ENTITY_ENEMY);

    Entity_SetTexture(enemy, &textures, TEXTURE_ENEMY);

    int xPos = windowBounds.w;
    int minY = 0;
    int maxY = windowBounds.h - ENTITY_H(enemy);

    Entity_Place(enemy, xPos, randomInt(minY, maxY));
    Entity_SetVelocity(enemy, randomInt(-5, -2), 0.0f);

    enemy->health = 1;

    int reloadTimeMin = -(ENTITY_W(enemy) / enemy->dx);
    enemy->reloadTime = randomInt(reloadTimeMin, reloadTimeMin + 8);

    enemySpawnTimer = randomInt(SHOOTER_FPS / 2, 2 * SHOOTER_FPS);
  }
}

static void updateEnemies(void) {
  EntityList_ForEachAndPrune(&enemies,
			     &updateEnemy,
			     &isEnemyOutOfBoundsOrDead);
}

static void updateEnemy(Entity* enemy) {
  moveEntity(enemy);
  --enemy->reloadTime;

  checkCollision(enemy, &player);

  if (enemy->health > 0 && enemy->reloadTime <= 0) {
    fireEnemyBullet(enemy);
    enemy->reloadTime = randomInt(0, SHOOTER_FPS * 2);
  }
}

static void fireEnemyBullet(const Entity* enemy) {
  Entity* bullet;
  bullet = EntityList_Add(&bullets, ENTITY_ENEMY_BULLET);

  Entity_SetTexture(bullet, &textures, TEXTURE_ENEMY_BULLET);

  bullet->type = ENTITY_ENEMY_BULLET;
  bullet->health = 1;

  Entity_PlaceAtCenter(bullet, enemy);

  float dx, dy;
  Entity_GetDirectionalVector(bullet, &player, &dx, &dy);

  dx *= SHOOTER_ENEMY_BULLET_SPEED;
  dy *= SHOOTER_ENEMY_BULLET_SPEED;
  Entity_SetVelocity(bullet, dx, dy);
}

static void moveEntity(Entity* entity) {
  Entity_Move(entity, 1.0f);
}

static void drawEntity(Entity* entity) {
  Entity_Render(entity, &textures);
}

static bool isBulletOutOfBoundsOrDead(const Entity* bullet) {
  if (bullet->health == 0) {
    return true;
  }
  
  OutOfBoundsFlags flags = Entity_CheckOutOfBounds(bullet, &windowBounds);
  return flags != OUT_OF_BOUNDS_NONE;
}

static bool isEnemyOutOfBoundsOrDead(const Entity* enemy) {
  if (enemy->health == 0) {
    return true;
  }
  
  return Entity_IsToTheLeftOf(enemy, windowBounds.x);
}

static void checkCollision(Entity* e1, Entity* e2) {
  if (Entity_CheckCollision(e1, e2)) {
    e1->health = 0;
    e2->health = 0;
  }
}

static void updateBackground(void) {
  if (--backgroundX < -windowBounds.w) {
    backgroundX = 0;
  }
}

static void drawBackground(void) {
  SDL_Rect dst;
  int x;

  for (x = 0; x < windowBounds.w; x += windowBounds.w) {
      dst.x = x;
      dst.y = 0;
      dst.w = windowBounds.w;
      dst.h = windowBounds.h;

      Textures_RenderFull(&textures, TEXTURE_BACKGROUND, &dst);
  }
}
