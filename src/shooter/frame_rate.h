#ifndef SHOOTER_FRAME_RATE_H_
#define SHOOTER_FRAME_RATE_H_

#include <SDL2/SDL.h>

typedef struct FrameRate {
  Uint32 frameStart;
  int frameDelay;
  int frameTime;
} FrameRate;

void FrameRate_Init(FrameRate* rate, int fps);
void FrameRate_BeginFrame(FrameRate* rate);
void FrameRate_EndFrame(FrameRate* rate);

#endif // SHOOTER_FRAME_RATE_H_
