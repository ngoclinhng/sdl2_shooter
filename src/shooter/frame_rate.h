#ifndef SHOOTER_FRAME_RATE_H_
#define SHOOTER_FRAME_RATE_H_

#include <SDL2/SDL.h>

struct FrameRate {
  Uint32 frameStart;
  int frameDelay;
  int frameTime;
};

void FrameRate_Init(struct FrameRate* rate, int fps);
void FrameRate_BeginFrame(struct FrameRate* rate);
void FrameRate_EndFrame(struct FrameRate* rate);

#endif // SHOOTER_FRAME_RATE_H_
