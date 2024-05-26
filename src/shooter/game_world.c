#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "shooter/defs.h"
#include "shooter/game_world.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static int enemySpawnTimer;

#define LOAD_TEXTURE_FOR_ENTITY(game, entity, type)	\
  do {							\
    TextureManager_Load(&((game)->textureManager),	\
		        (type),				\
			&((entity)->w),			\
			&(entity)->h);			\
    (entity)->textureType = (type);			\
  } while (0)

#define IS_ENEMY_OFF_SCREEN(E) ((E)->entity.x < -(E)->entity.w)
#define IS_ENEMY_DEAD(E) ((E)->health == 0)
#define IS_BULLET_OFF_SCREEN(B) ((B)->entity.x > WINDOW_WIDTH)

static void initSDL(void);
static void initPlayer(struct GameWorld* game);

static void updatePlayer(struct GameWorld* game);
static void updateBullets(struct GameWorld* game);
static void fireBullet(struct GameWorld* game);
static void spawnEnemy(struct GameWorld* game);
static void updateEnemies(struct GameWorld* game);

static bool checkBulletHit(struct GameWorld* game,
			   const struct Bullet* bullet);

static bool checkBulletHitPlayer(struct GameWorld* game,
				 const struct Bullet* bullet);

static bool checkBulletHitEnemy(struct GameWorld* game,
				const struct Bullet* bullet);

static void drawPlayer(struct GameWorld* game);
static void drawBullets(struct GameWorld* game);
static void drawEnemies(struct GameWorld* game);

static void destroyBullets(struct GameWorld* game);
static void destroyEnemies(struct GameWorld* game);

void GameWorld_Init(struct GameWorld* game) {
  initSDL();

  memset(game, 0, sizeof(struct GameWorld));
  InputManager_Init(&game->inputManager);
  TextureManager_Init(&game->textureManager, renderer);

  initPlayer(game);

  game->enemyTail = &game->enemyHead;
  game->bulletTail = &game->bulletHead;
  enemySpawnTimer = 0;
}

void GameWorld_PrepareScene(void) {
  SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
  SDL_RenderClear(renderer);
}

void GameWorld_Update(struct GameWorld* game) {
  struct InputManager* inputManager = &game->inputManager;
  InputManager_PollEventAndUpdate(inputManager);

  if (inputManager->gameActions[GAME_ACTION_QUIT]) {
    exit(0);
  }
  
  updatePlayer(game);
  updateBullets(game);

  spawnEnemy(game);
  updateEnemies(game);
}

void GameWorld_Draw(struct GameWorld* game) {
  drawPlayer(game);
  drawBullets(game);
  drawEnemies(game);
}

void GameWorld_PresentScene(void) {
  SDL_RenderPresent(renderer);
}

void GameWorld_Destroy(struct GameWorld* game) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Cleaning up....\n");
  
  destroyBullets(game);
  destroyEnemies(game);
  TextureManager_Destroy(&game->textureManager);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  IMG_Quit();
  SDL_Quit();
  
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Done!\n");
}

// Helpers

static void initPlayer(struct GameWorld* game) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "initPlayer\n");

  struct Player* player = &game->player;

  LOAD_TEXTURE_FOR_ENTITY(game, &player->entity, TEXTURE_PLAYER);
  ENTITY_SET_POSITION(&player->entity, 100.0f, 100.0f);
  
  player->health = 1;
  player->reloadTime = 0;
}

static void updatePlayer(struct GameWorld* game) {
  const struct InputManager* im = &game->inputManager;
  struct Player* player = &game->player;

  ENTITY_SET_VELOCITY(&player->entity, 0.0f, 0.0f);  
  player->reloadTime--;

  if (im->gameActions[GAME_ACTION_UP]) {
    ENTITY_SET_VELOCITY_Y(&player->entity, -PLAYER_SPEED);
  }

  if (im->gameActions[GAME_ACTION_DOWN]) {
    ENTITY_SET_VELOCITY_Y(&player->entity, PLAYER_SPEED);
  }

  if (im->gameActions[GAME_ACTION_LEFT]) {
    ENTITY_SET_VELOCITY_X(&player->entity, -PLAYER_SPEED);
  }

  if (im->gameActions[GAME_ACTION_RIGHT]) {
    ENTITY_SET_VELOCITY_X(&player->entity, PLAYER_SPEED);
  }

  if (im->gameActions[GAME_ACTION_FIRE] && player->reloadTime <= 0) {
    fireBullet(game);
  }

  ENTITY_MOVE(&player->entity);
}

static void fireBullet(struct GameWorld* game) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "fireBullet\n");

  struct Player* player = &game->player;
  struct Bullet* bullet;
  size_t bulletSize = sizeof(struct Bullet);
  
  bullet = malloc(bulletSize);
  memset(bullet, 0, bulletSize);

  LOAD_TEXTURE_FOR_ENTITY(game, &bullet->entity, TEXTURE_PLAYER_BULLET);  
  ENTITY_SET_POSITION_RELATIVE(&bullet->entity, &player->entity);
  ENTITY_SET_VELOCITY(&bullet->entity, PLAYER_BULLET_SPEED, 0.0f);  

  bullet->origin = BULLET_FROM_PLAYER;
  GAME_WORLD_ADD_BULLET(game, bullet);
  player->reloadTime = 8;
}

static void updateBullets(struct GameWorld* game) {
  struct Bullet *b, *prev;
  prev = &game->bulletHead;

  for (b = game->bulletHead.next; b != NULL; b = b->next) {
    ENTITY_MOVE(&b->entity);

    if (IS_BULLET_OFF_SCREEN(b) || checkBulletHit(game, b)) {
      if (b == game->bulletTail) {
	game->bulletTail = prev;
      }

      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Free a bullet\n");
      
      prev->next = b->next;
      free(b);
      b = prev;
    }

    prev = b;
  }
}

static bool checkBulletHit(struct GameWorld* game,
			   const struct Bullet* bullet) {
  if (bullet->origin == BULLET_FROM_ENEMY) {
    return checkBulletHitPlayer(game, bullet);
  } else {
    return checkBulletHitEnemy(game, bullet);
  }
}

static bool checkBulletHitPlayer(struct GameWorld* game,
				 const struct Bullet* bullet) {
  if (Entity_HasCollided(&bullet->entity, &game->player.entity)) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Bullet hit Player!\n");
    return false;
  }

  return false;
}

static bool checkBulletHitEnemy(struct GameWorld* game,
				const struct Bullet* bullet) {
  struct Enemy* e;

  for (e = game->enemyHead.next; e != NULL; e = e->next) {
    if (Entity_HasCollided(&bullet->entity, &e->entity)) {
      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Bullet hit Enemy!\n");
      e->health = 0;
      return true;
    }
  }

  return false;
}

static void spawnEnemy(struct GameWorld* game) {
  struct Enemy* enemy;
  size_t enemySize = sizeof(struct Enemy);
  
  if (--enemySpawnTimer <= 0) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "spawnEnemy\n");

    enemy = malloc(enemySize);
    memset(enemy, 0, enemySize);

    LOAD_TEXTURE_FOR_ENTITY(game, &enemy->entity, TEXTURE_ENEMY);
    ENTITY_SET_POSITION(&enemy->entity, WINDOW_WIDTH, rand() % WINDOW_HEIGHT);
    ENTITY_SET_VELOCITY(&enemy->entity, -(2 + (rand() % 4)), 0.0f);

    enemy->health = 1;
    GAME_WORLD_ADD_ENEMY(game, enemy);
    enemySpawnTimer = 30 + (rand() % 60);
  }
}

static void updateEnemies(struct GameWorld* game) {
  struct Enemy *e, *prev;
  prev = &game->enemyHead;

  for (e = game->enemyHead.next; e != NULL; e = e->next) {
    ENTITY_MOVE(&e->entity);

    if (IS_ENEMY_OFF_SCREEN(e) || IS_ENEMY_DEAD(e)) {
      if (e == game->enemyTail) {
	game->enemyTail = prev;
      }

      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Free an enemy\n");
      
      prev->next = e->next;
      free(e);
      e = prev;
    }

    prev = e;
  }
}

static void drawPlayer(struct GameWorld* game) {
  TextureManager_Render(&game->textureManager,
			game->player.entity.textureType,
			game->player.entity.x,
			game->player.entity.y);
}

static void drawBullets(struct GameWorld* game) {
  struct Bullet* b;

  for (b = game->bulletHead.next; b != NULL; b = b->next) {
    TextureManager_Render(&game->textureManager,
			  b->entity.textureType,
			  b->entity.x,
			  b->entity.y);
  }
}

static void drawEnemies(struct GameWorld* game) {
  struct Enemy* e;

  for (e = game->enemyHead.next; e != NULL; e = e->next) {
    TextureManager_Render(&game->textureManager,
			  e->entity.textureType,
			  e->entity.x,
			  e->entity.y);
  }
}

static void destroyBullets(struct GameWorld* game) {
  struct Bullet* b;

  for (b = game->bulletHead.next; b != NULL; b = b->next) {
    free(b);
  }

  memset(&game->bulletHead, 0, sizeof(struct Bullet));
  game->bulletTail = &game->bulletHead;
}

static void destroyEnemies(struct GameWorld* game) {
  struct Enemy* e;

  for (e = game->enemyHead.next; e != NULL; e = e->next) {
    free(e);
  }

  memset(&game->enemyHead, 0, sizeof(struct Enemy));
  game->enemyTail = &game->enemyHead;
}

void initSDL(void) {
  const int windowFlags = 0;
  const int rendererFlags = SDL_RENDERER_ACCELERATED;  
  const int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL could not initialize! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  window = SDL_CreateWindow("Shooter",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    WINDOW_WIDTH,
			    WINDOW_HEIGHT,
			    windowFlags);

  if (!window) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Window could not be created! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  renderer = SDL_CreateRenderer(window, -1, rendererFlags);

  if (!renderer) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "Renderer could not be created! SDL_Error: %s\n",
		 SDL_GetError());
    exit(1);
  }

  if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
		 "SDL_image could not initialize! SDL_image error: %s\n",
		 IMG_GetError());
    exit(1);
  }
}
