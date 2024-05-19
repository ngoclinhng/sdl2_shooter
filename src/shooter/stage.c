#include <stdlib.h>
#include <string.h>
#include "shooter/defs.h"
#include "shooter/structs.h"
#include "shooter/stage.h"
#include "shooter/asset_loader.h"
#include "shooter/draw.h"

extern struct GameEngine g_gameEngine;
extern struct GameStage g_gameStage;

static struct GameEntity player;

static void handlePlayerLogic(void);
static void fireBullet(void);
static void handleBulletsLogic(void);
static void handleLogic(void);

static void drawPlayer(void);
static void drawBullets(void);
static void handleDraw(void);

static void initPlayer(void);

void initGameStage(void) {
  g_gameEngine.delegate.handleLogic = &handleLogic;
  g_gameEngine.delegate.handleDraw = &handleDraw;
  
  memset(&g_gameStage, 0, sizeof(struct GameStage));  
  g_gameStage.fightersTail = &g_gameStage.fightersHead;
  g_gameStage.bulletsTail = &g_gameStage.bulletsHead;

  initPlayer();
}

static void initPlayer() {
  memset(&player, 0, sizeof(struct GameEntity));

  player.x = 100;
  player.y = 100;
  player.texture = loadTexture(TEXTURE_PLAYER);
  SDL_QueryTexture(player.texture, NULL, NULL, &player.w, &player.h);

  // Add player to game stage
  g_gameStage.fightersTail->next = &player;
  g_gameStage.fightersTail = &player;
}

static void handleLogic(void) {
  handlePlayerLogic();
  handleBulletsLogic();
}

static void handlePlayerLogic(void) {
  player.dx = 0;
  player.dy = 0;

  if (player.reload > 0) {
    player.reload--;
  }

  if (g_gameEngine.actionStates[ACTION_UP]) {
    player.dy = -PLAYER_SPEED;
  }

  if (g_gameEngine.actionStates[ACTION_DOWN]) {
    player.dy = PLAYER_SPEED;
  }

  if (g_gameEngine.actionStates[ACTION_LEFT]) {
    player.dx = PLAYER_SPEED;
  }

  if (g_gameEngine.actionStates[ACTION_RIGHT]) {
    player.dx = -PLAYER_SPEED;
  }

  if (g_gameEngine.actionStates[ACTION_FIRE] && player.reload == 0) {
    fireBullet();
  }

  player.x += player.dx;
  player.y += player.dy;
}

static void fireBullet(void) {
  struct GameEntity* bullet;
  size_t bulletSize = sizeof(struct GameEntity);

  bullet = malloc(bulletSize);
  memset(bullet, 0, bulletSize);
  
  bullet->texture = loadTexture(TEXTURE_PLAYER_BULLET);
  SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

  bullet->x = player.x;
  bullet->y = player.y + (player.h / 2) - (bullet->h / 2);
  bullet->dx = PLAYER_BULLET_SPEED;
  bullet->health = 1;

  // Add new bullet to game stage
  g_gameStage.bulletsTail->next = bullet;
  g_gameStage.bulletsTail = bullet;  
  
  player.reload = 8;
}

static void handleBulletsLogic(void) {
  struct GameEntity *b, *prev;
  prev = &g_gameStage.bulletsHead;

  for (b = g_gameStage.bulletsHead.next; b != NULL; b = b->next) {
    b->x += b->dx;
    b->y += b->dy;

    if (b->x > WINDOW_WIDTH) {
      if (b == g_gameStage.bulletsTail) {
	g_gameStage.bulletsTail = prev;
      }

      SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Free a bullet.");
      prev->next = b->next;
      free(b);
      b = prev;
    }

    prev = b;
  }
}

static void handleDraw(void) {
  drawPlayer();
  drawBullets();
}

static void drawPlayer(void) {
  blit(player.texture, player.x, player.y);
}

static void drawBullets(void) {
  struct GameEntity *b;

  for (b = g_gameStage.bulletsHead.next; b != NULL; b = b->next) {
    blit(b->texture, b->x, b->y);
  }
}


