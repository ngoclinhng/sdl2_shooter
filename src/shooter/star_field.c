#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "shooter/utils.h"
#include "shooter/star_field.h"

void StarField_Init(StarField* starField, int count, int windowWidth,
		    int windowHeight) {
  memset(starField, 0, sizeof(StarField));

  Star* stars = malloc(sizeof(Star) * count);
  assert(stars != NULL);

  starField->stars = stars;
  starField->count = count;

  starField->windowWidth = windowWidth;
  starField->windowHeight = windowHeight;
}

void StarField_Free(StarField* starField) {
  free(starField->stars);
  starField->stars = NULL;
  starField->count = 0;
}

void StarField_Populate(StarField* starField, int minSpeed, int maxSpeed) {
  for (int i = 0; i < starField->count; ++i) {
    starField->stars[i].x = randomInt(0, starField->windowWidth);
    starField->stars[i].y = randomInt(0, starField->windowHeight);
    starField->stars[i].speed = randomInt(minSpeed, maxSpeed);

    int c = starField->stars[i].speed * 32;
    starField->stars[i].color = (SDL_Color){c, c, c, 255};
  }
}

void StarField_Update(StarField* starField) {
  Star* stars = starField->stars;

  for (int i= 0; i < starField->count; ++i) {
    stars[i].x -= stars[i].speed;

    if (stars[i].x < 0) {
      stars[i].x = starField->windowWidth + stars[i].x;
    }
  }
}

void StarField_Draw(StarField* starField, SDL_Renderer* renderer) {
  Star* stars = starField->stars;

  for (int i = 0; i < starField->count; ++i) {
    SDL_SetRenderDrawColor(renderer,
			   stars[i].color.r,
			   stars[i].color.g,
			   stars[i].color.b,
			   stars[i].color.a);    
    SDL_RenderDrawLine(renderer, stars[i].x, stars[i].y,
		       stars[i].x + stars[i].speed, stars[i].y);
  }
}

