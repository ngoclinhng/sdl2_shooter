#include "shooter/frame_rate.h"

void FrameRate_Init(struct FrameRate* rate, int fps) {
  rate->frameStart = 0;
  rate->frameDelay = 1000 / fps;
  rate->frameTime = 0;
}

void FrameRate_BeginFrame(struct FrameRate* rate) {
  rate->frameStart = SDL_GetTicks();
}

void FrameRate_EndFrame(struct FrameRate* rate) {
  rate->frameTime = SDL_GetTicks() - rate->frameStart;

  if (rate->frameDelay > rate->frameTime) {
    SDL_Delay(rate->frameDelay - rate->frameTime);
  }
}
