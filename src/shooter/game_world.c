#include <string.h>
#include "shooter/defs.h"
#include "shooter/textures.h"
#include "shooter/game_world.h"

static Textures textures;

static void resetGameWorld(GameWorld* game);
static void initPlayer(GameWorld* game);
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

  // TODO: update game world
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
  Entity* player = &game->player;
  int width, height;  

  Textures_Load(&textures, TEXTURE_PLAYER, &width, &height);
  Entity_SetPositionAndSize(player, 100, 100, width, height);
  
  player->type = ENTITY_PLAYER;
  player->health = 1;

  Entity_SetVelocity(player, SHOOTER_PLAYER_SPEED, 0.0f);
}

static void drawPlayer(GameWorld* game) {
  int x = game->player.hitbox.x;
  int y = game->player.hitbox.y;  
  Textures_Render(&textures, TEXTURE_PLAYER, x, y);
}
