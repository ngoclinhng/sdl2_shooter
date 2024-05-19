#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "shooter/structs.h"
#include "shooter/init.h"
#include "shooter/input.h"
#include "shooter/draw.h"
#include "shooter/stage.h"

struct GameEngine g_gameEngine;
struct GameStage g_gameStage;

int main(void) {
  memset(&g_gameEngine, 0, sizeof(struct GameEngine));
  
  initSDL();  
  atexit(cleanup);

  initGameStage();

  while (1) {        
    prepareScene();    
    processEvents();

    g_gameEngine.delegate.handleLogic();
    g_gameEngine.delegate.handleDraw();
    
    presentScene();    
    SDL_Delay(16);
  }

  return 0;
}

