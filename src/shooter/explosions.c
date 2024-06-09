#include <stdbool.h>
#include "shooter/defs.h"
#include "shooter/linked_list.h"
#include "shooter/explosions.h"
#include "shooter/utils.h"

typedef struct Explosion {
  int x, y;
  float dx, dy;
  SDL_Color color;
} Explosion;

static LinkedList explosions;
static SDL_Renderer* renderer;
static SDL_Texture* texture;

static void updateExplosion(void* explosion);
static bool isDead(const void* explosion);
static void drawExplosion(void* explosion);

void Explosions_Init(void) {
  LinkedList_Init(&explosions);
}

void Explosions_Free(void) {
  LinkedList_Free(&explosions);
}

void Explosions_Add(int x, int y) {
  Explosion* e;

  for (int i = 0; i < 32; ++i) {
    e = LinkedList_Add(&explosions, sizeof(Explosion));

    e->x = randomInt(x - 32, x + 32);
    e->y = randomInt(y - 32, y + 32);

    e->dx = randomInt(-9, 9);
    e->dy = randomInt(-9, 9);

    e->dx /= 10.0f;
    e->dy /= 10.0f;

    switch(randomInt(0, 3)) {
    case 0:
      e->color.r = 255;
      break;

    case 1:
      e->color.r = 255;
      e->color.g = 128;
      break;

    case 2:
      e->color.r = 255;
      e->color.g = 255;
      break;

    default:
      e->color.r = 255;
      e->color.g = 255;
      e->color.b = 255;
      break;      
    }

    e->color.a = randomInt(0, SHOOTER_FPS * 3);
  }
}

void Explosions_Update(void) {
  LinkedList_ForEachAndPrune(&explosions, &updateExplosion, &isDead);
}

void Explosions_Draw(SDL_Renderer* inRenderer, SDL_Texture* inTexture) {
  renderer = inRenderer;
  texture = inTexture;

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
  SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);  

  LinkedList_ForEach(&explosions, &drawExplosion);

  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

static void updateExplosion(void* explosion) {
  Explosion* e = (Explosion*) explosion;

  e->x += e->dx;
  e->y += e->dy;
  e->color.a -= 1;
}

static bool isDead(const void* explosion) {
  const Explosion* e = (const Explosion*) explosion;
  return e->color.a <= 0;
}

static void drawExplosion(void* explosion) {
  Explosion* e = (Explosion*) explosion;

  SDL_SetTextureColorMod(texture, e->color.r, e->color.g, e->color.b);
  SDL_SetTextureAlphaMod(texture, e->color.a);

  SDL_Rect dst = {.x = e->x, .y = e->y, .w = 0, .h = 0};
  SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
  SDL_RenderCopy(renderer, texture, NULL, &dst);
}
