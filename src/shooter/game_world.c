#include <string.h>
#include "shooter/defs.h"
#include "shooter/linked_list.h"
#include "shooter/star_field.h"
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
static LinkedList bullets;
static LinkedList enemies;
static StarField starField;

static int enemySpawnTimer;
static int gameWorldResetTimer;
static int backgroundX;

static void resetGameWorld(void);

static void updateBackground(void);
static void initPlayer(void);
static void updatePlayer(const Events* events);

static void firePlayerBullet(void);
static void updateBullet(void* bullet);
static void updateBullets(void);

static void spawnEnemy(void);
static void updateEnemy(void* enemy);
static void updateEnemies(void);
static void fireEnemyBullet(const Entity* enemy);

static void moveEntity(Entity* entity);
static void drawEntity(void* entity);
static void drawBackground(void);

static bool isBulletOutOfBoundsOrDead(const void* bullet);
static bool isEnemyOutOfBoundsOrDead(const void* enemy);
static void checkCollision(void* entity1, void* entity2);

void GameWorld_Init(GameContext* context) {
  Textures_Init(&textures, context->renderer);

  Textures_LoadAndStore(&textures, TEXTURE_PLAYER);
  Textures_LoadAndStore(&textures, TEXTURE_PLAYER_BULLET);
  Textures_LoadAndStore(&textures, TEXTURE_ENEMY);
  Textures_LoadAndStore(&textures, TEXTURE_ENEMY_BULLET);
  Textures_LoadAndStore(&textures, TEXTURE_BACKGROUND);
  
  LinkedList_Init(&bullets);
  LinkedList_Init(&enemies);
  StarField_Init(&starField, 400, windowBounds.w, windowBounds.h);
  
  resetGameWorld();
}

void GameWorld_Free() {
  LinkedList_Free(&bullets);
  LinkedList_Free(&enemies);
  StarField_Free(&starField);
  Textures_Free(&textures);
}

void GameWorld_Update(const Events* events) {
  if (Events_IsActive(events, EVENT_QUIT)) {
    exit(0);
  }

  updateBackground();
  StarField_Update(&starField);
  
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
  StarField_Draw(&starField, textures.renderer);
  
  drawEntity(&player);
  LinkedList_ForEach(&bullets, &drawEntity);
  LinkedList_ForEach(&enemies, &drawEntity);
}

// Helpers

static void resetGameWorld(void) {  
  initPlayer();

  LinkedList_Free(&bullets);
  LinkedList_Free(&enemies);

  StarField_Populate(&starField, 1, 5);

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

  bullet = LinkedList_Add(&bullets, sizeof(Entity));
  bullet->type = ENTITY_PLAYER_BULLET;

  Entity_SetTexture(bullet, &textures, TEXTURE_PLAYER_BULLET);
  Entity_PlaceAtCenter(bullet, &player);
  Entity_SetVelocity(bullet, SHOOTER_PLAYER_BULLET_SPEED, 0.0f);

  bullet->health = 1;
  player.reloadTime = 8;
}

static void updateBullets(void) {
  LinkedList_ForEachAndPrune(&bullets,
			     &updateBullet,
			     &isBulletOutOfBoundsOrDead);
}

static void updateBullet(void* b) {
  Entity* bullet = (Entity*) b;
  
  moveEntity(bullet);
  
  if (bullet->type == ENTITY_ENEMY_BULLET &&
      Entity_CheckCollision(bullet, &player)) {    
    player.health = 0;
    bullet->health = 0;
    return;
  }

  if (bullet->type == ENTITY_PLAYER_BULLET) {       
    LinkedList_ForEachWith(&enemies, bullet, &checkCollision);
  }
}

static void spawnEnemy(void) {
  if (--enemySpawnTimer <= 0) {
    Entity* enemy;
    enemy = LinkedList_Add(&enemies, sizeof(Entity));
    enemy->type = ENTITY_ENEMY;

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
  LinkedList_ForEachAndPrune(&enemies,
			     &updateEnemy,
			     &isEnemyOutOfBoundsOrDead);
}

static void updateEnemy(void* e) {
  Entity* enemy = (Entity*) e;
  
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

  bullet = LinkedList_Add(&bullets, sizeof(Entity));
  bullet->type = ENTITY_ENEMY_BULLET;

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

static void drawEntity(void* entity) {
  Entity* e = (Entity*) entity;
  Entity_Render(e, &textures);
}

static bool isBulletOutOfBoundsOrDead(const void* b) {
  const Entity* bullet = (Entity*) b;
  
  if (bullet->health == 0) {
    return true;
  }
  
  OutOfBoundsFlags flags = Entity_CheckOutOfBounds(bullet, &windowBounds);
  return flags != OUT_OF_BOUNDS_NONE;
}

static bool isEnemyOutOfBoundsOrDead(const void* e) {
  Entity* enemy = (Entity*) e;
  
  if (enemy->health == 0) {
    return true;
  }
  
  return Entity_IsToTheLeftOf(enemy, windowBounds.x);
}

static void checkCollision(void* entity1, void* entity2) {
  Entity* e1 = (Entity*) entity1;
  Entity* e2 = (Entity*) entity2;
  
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
