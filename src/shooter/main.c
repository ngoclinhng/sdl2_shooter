#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "shooter/structs.h"
#include "shooter/init.h"
#include "shooter/input.h"
#include "shooter/draw.h"

struct Shooter g_shooter;
struct Entity player;

int main(void) {
  memset(&g_shooter, 0, sizeof(struct Shooter));
  memset(&player, 0, sizeof(struct Entity));
  
  initSDL();  
  atexit(cleanup);

  player.x = 100;
  player.y = 100;
  player.texture = loadTexture("gfx/player.png");

  while (1) {        
    prepareScene();
    
    doInput();

    if (g_shooter.up) {
      player.y -= 4;
    }

    if (g_shooter.down) {
      player.y += 4;
    }

    if (g_shooter.left) {
      player.x -= 4;
    }

    if (g_shooter.right) {
      player.x += 4;
    }

    blit(player.texture, player.x, player.y);
    
    presentScene();
    
    SDL_Delay(16);
  }

  return 0;
}

