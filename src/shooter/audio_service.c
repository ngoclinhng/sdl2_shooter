#include <string.h>
#include <assert.h>
#include "shooter/audio_service.h"

void AudioService_Init(AudioService* service) {
  memset(service, 0, sizeof(AudioService));

  service->backgroundMusic = Mix_LoadMUS("sounds/background.ogg");
  assert(service->backgroundMusic != NULL);
}

void AudioService_Play(AudioService* service, SoundType sound) {
  switch (sound) {
  case BACKGROUND_MUSIC:
    Mix_PlayMusic(service->backgroundMusic, -1);
    return;
    
  default:
    return;
  }
}

void AudioService_Free(AudioService* service) {
  Mix_FreeMusic(service->backgroundMusic);
}
