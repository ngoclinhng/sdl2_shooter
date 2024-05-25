#include <stdlib.h>
#include <SDL2/SDL.h>
#include "shooter/game_world.h"

struct GameWorld g_gameWorld;

static void cleanup(void);

int main(void) {
  GameWorld_Init(&g_gameWorld);
  atexit(cleanup);

  while (1) {
    GameWorld_PrepareScene();    

    GameWorld_Update(&g_gameWorld);
    GameWorld_Draw(&g_gameWorld);    

    GameWorld_PresentScene();
    SDL_Delay(16);
  }

  return 0;
}

static void cleanup(void) {
  GameWorld_Destroy(&g_gameWorld);
}
