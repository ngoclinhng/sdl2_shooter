#include "shooter/frame_rate.h"

void FrameRate_Init(FrameRate* rate, int fps) {
  rate->frameStart = 0;
  rate->frameDelay = 1000 / fps;
  rate->frameTime = 0;
}

void FrameRate_BeginFrame(FrameRate* rate) {
  rate->frameStart = SDL_GetTicks();
}

void FrameRate_EndFrame(FrameRate* rate) {
  rate->frameTime = SDL_GetTicks() - rate->frameStart;

  if (rate->frameDelay > rate->frameTime) {
    SDL_Delay(rate->frameDelay - rate->frameTime);
  }
}
