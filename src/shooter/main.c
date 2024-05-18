#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "shooter/structs.h"
#include "shooter/init.h"
#include "shooter/input.h"
#include "shooter/draw.h"

struct Shooter shooter;

int main(void) {
  memset(&shooter, 0, sizeof(shooter));
  initSDL();
  atexit(cleanup);

  while (1) {        
    prepareScene();
    doInput();
    presentScene();
    SDL_Delay(16);
  }

  return 0;
}

