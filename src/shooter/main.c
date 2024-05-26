#include <stdlib.h>
#include <SDL2/SDL.h>

#include "shooter/defs.h"
#include "shooter/frame_rate.h"
#include "shooter/game_world.h"

struct FrameRate g_frameRate;
struct GameWorld g_gameWorld;

static void cleanup(void);

int main(void) {
  GameWorld_Init(&g_gameWorld);
  FrameRate_Init(&g_frameRate, SHOOTER_FPS);
  
  atexit(cleanup);

  while (1) {
    FrameRate_BeginFrame(&g_frameRate);    
    GameWorld_PrepareScene();    

    GameWorld_Update(&g_gameWorld);
    GameWorld_Draw(&g_gameWorld);    

    GameWorld_PresentScene();
    FrameRate_EndFrame(&g_frameRate);
  }

  return 0;
}

static void cleanup(void) {
  GameWorld_Destroy(&g_gameWorld);
}
