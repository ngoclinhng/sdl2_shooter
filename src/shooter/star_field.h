#ifndef SHOOTER_START_FIELD_H_
#define SHOOTER_START_FIELD_H_

#include <SDL2/SDL.h>

typedef struct Star {
  int x, y, speed;
  SDL_Color color;
} Star;

typedef struct StarField {
  Star* stars;
  int count;
  int windowWidth, windowHeight;
} StarField;

void StarField_Init(StarField* starField, int count, int windowWidth,
		    int windowHeight);
void StarField_Free(StarField* starField);

void StarField_Populate(StarField* starField, int minSpeed, int maxSpeed);
void StarField_Update(StarField* starField);
void StarField_Draw(StarField* starField, SDL_Renderer* renderer);

#endif // SHOOTER_START_FIELD_H_
