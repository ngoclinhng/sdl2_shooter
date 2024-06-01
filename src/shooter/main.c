#include <stdlib.h>
#include "shooter/defs.h"
#include "shooter/frame_rate.h"
#include "shooter/game_context.h"
#include "shooter/events.h"

GameContext g_gameContext;
Events g_events;
FrameRate g_frameRate;

static void cleanup(void);

int main(void) {
  GameContext_Init(&g_gameContext, "Shooter", SHOOTER_WINDOW_WIDTH,
		   SHOOTER_WINDOW_HEIGHT);
  Events_Init(&g_events);  
  FrameRate_Init(&g_frameRate, SHOOTER_FPS);  
  atexit(cleanup);

  while (1) {
    FrameRate_BeginFrame(&g_frameRate);    
    GameContext_PrepareScene(&g_gameContext);

    Events_PollAndUpdate(&g_events);

    if (Events_IsActive(&g_events, EVENT_QUIT)) {
      exit(0);
    }

    GameContext_PresentScene(&g_gameContext);        
    FrameRate_EndFrame(&g_frameRate);
  }

  return 0;
}

static void cleanup(void) {
  GameContext_Free(&g_gameContext);
}
