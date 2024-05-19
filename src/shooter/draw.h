#ifndef SHOOTER_DRAW_H_
#define SHOOTER_DRAW_H_

#include <SDL2/SDL.h>

void prepareScene(void);
void presentScene(void);
void blit(SDL_Texture* texture, int x, int y);

#endif // SHOOTER_DRAW_H_
