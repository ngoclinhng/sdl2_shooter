#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "shooter/defs.h"
#include "shooter/game_world.h"

static SDL_Window* window;
static SDL_Renderer* renderer;
static int enemySpawnTimer;

static void initSDL(void);
static void initPlayer(struct GameWorld* game);

static void updatePlayer(struct GameWorld* game);
static void updateBullets(struct GameWorld* game);
static void fireBullet(struct GameWorld* game);
static void spawnEnemy(struct GameWorld* game);
static void updateEnemies(struct GameWorld* game);

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
  struct Entity* entity = &game->player.entity;
  struct TextureManager* tm = &game->textureManager;

  entity->x = 100;
  entity->y = 100;
  entity->textureType = TEXTURE_PLAYER;
  TextureManager_GetSize(tm, entity->textureType, &entity->w, &entity->h);

  game->player.health = 1;
  game->player.reloadTime = 0;
}

static void updatePlayer(struct GameWorld* game) {
  struct Player* player = &game->player;
  const struct InputManager* inputManager = &game->inputManager;

  player->entity.dx = 0;
  player->entity.dy = 0;
  player->reloadTime--;

  if (inputManager->gameActions[GAME_ACTION_UP]) {
    player->entity.dy = -PLAYER_SPEED;
  }

  if (inputManager->gameActions[GAME_ACTION_DOWN]) {
    player->entity.dy = PLAYER_SPEED;
  }

  if (inputManager->gameActions[GAME_ACTION_LEFT]) {
    player->entity.dx = -PLAYER_SPEED;
  }

  if (inputManager->gameActions[GAME_ACTION_RIGHT]) {
    player->entity.dx = PLAYER_SPEED;
  }

  if (inputManager->gameActions[GAME_ACTION_FIRE] && player->reloadTime <= 0) {
    fireBullet(game);
  }

  player->entity.x += player->entity.dx;
  player->entity.y += player->entity.dy;  
}

static void fireBullet(struct GameWorld* game) {
  SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "fireBullet\n");

  struct Player* player = &game->player;
  struct Bullet* bullet;
  size_t bulletSize = sizeof(struct Bullet);
  
  bullet = malloc(bulletSize);
  memset(bullet, 0, bulletSize);

  bullet->entity.textureType = TEXTURE_PLAYER_BULLET;
  TextureManager_GetSize(&game->textureManager,
			 bullet->entity.textureType,
			 &bullet->entity.w, &bullet->entity.h);


  float offsetY = (player->entity.h / 2) - (bullet->entity.h / 2);
  
  bullet->entity.x = player->entity.x;
  bullet->entity.y = player->entity.y + offsetY;
  bullet->entity.dx = PLAYER_BULLET_SPEED;

  game->bulletTail->next = bullet;
  game->bulletTail = bullet;

  player->reloadTime = 8;
}

static void updateBullets(struct GameWorld* game) {
  struct Bullet *b, *prev;
  prev = &game->bulletHead;

  for (b = game->bulletHead.next; b != NULL; b = b->next) {
    b->entity.x += b->entity.dx;
    b->entity.y += b->entity.dy;

    if (b->entity.x > WINDOW_WIDTH) {
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

static void spawnEnemy(struct GameWorld* game) {
  struct Enemy* enemy;
  size_t enemySize = sizeof(struct Enemy);
  
  if (--enemySpawnTimer <= 0) {
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "spawnEnemy\n");

    enemy = malloc(enemySize);
    memset(enemy, 0, enemySize);

    enemy->entity.x = WINDOW_WIDTH;
    enemy->entity.y = rand() % WINDOW_HEIGHT;

    enemy->entity.textureType = TEXTURE_ENEMY;
    TextureManager_GetSize(&game->textureManager,
			   enemy->entity.textureType,
			   &enemy->entity.w,
			   &enemy->entity.h);

    enemy->entity.dx = -(2 + (rand() % 4));
    enemySpawnTimer = 30 + (rand() % 60);

    game->enemyTail->next = enemy;
    game->enemyTail = enemy;
  }
}

static void updateEnemies(struct GameWorld* game) {
  struct Enemy *e, *prev;
  prev = &game->enemyHead;

  for (e = game->enemyHead.next; e != NULL; e = e->next) {
    e->entity.x += e->entity.dx;
    e->entity.y += e->entity.dy;

    if (e->entity.x < 0) {
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
