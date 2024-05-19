#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "shooter/defs.h"
#include "shooter/structs.h"
#include "shooter/init.h"
#include "shooter/input.h"
#include "shooter/draw.h"

struct Shooter g_shooter;
struct Entity player;
struct Entity bullet;

int main(void) {
  memset(&g_shooter, 0, sizeof(struct Shooter));
  memset(&player, 0, sizeof(struct Entity));
  memset(&bullet, 0, sizeof(struct Entity));
  
  initSDL();  
  atexit(cleanup);

  player.x = 100;
  player.y = 100;
  player.texture = loadTexture("gfx/player.png");

  bullet.texture = loadTexture("gfx/playerBullet.png");

  while (1) {        
    prepareScene();
    
    doInput();

    player.x += player.dx;
    player.y += player.dy;

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

    if (g_shooter.fire && bullet.health == 0) {
      bullet.x = player.x;
      bullet.y = player.y;
      bullet.dx = 16;
      bullet.dy = 0;
      bullet.health = 1;
    }

    bullet.x += bullet.dx;
    bullet.y += bullet.dy;

    if (bullet.x > SHOOTER_WINDOW_WIDTH) {
      bullet.health = 0;
    }

    blit(player.texture, player.x, player.y);

    if (bullet.health > 0) {
      blit(bullet.texture, bullet.x, bullet.y);
    }
    
    presentScene();
    
    SDL_Delay(16);
  }

  return 0;
}

