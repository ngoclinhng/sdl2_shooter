#include <string.h>
#include <assert.h>
#include "shooter/audio_service.h"

#define PLAYER_SOUND_CHANNEL 0
#define ENEMY_SOUND_CHANNEL 1

void AudioService_Init(AudioService* service) {
  memset(service, 0, sizeof(AudioService));

  service->backgroundMusic = Mix_LoadMUS("sounds/background.ogg");
  assert(service->backgroundMusic != NULL);

  service->playerBulletSound = Mix_LoadWAV("sounds/player_bullet.wav");
  assert(service->playerBulletSound != NULL);

  service->enemyExplosionSound = Mix_LoadWAV("sounds/enemy_explosion.wav");
  assert(service->enemyExplosionSound != NULL);
}

void AudioService_Play(AudioService* service, SoundType sound) {
  switch (sound) {
  case BACKGROUND_MUSIC:
    Mix_PlayMusic(service->backgroundMusic, -1);
    return;

  case PLAYER_BULLET_SOUND:
    Mix_PlayChannel(PLAYER_SOUND_CHANNEL, service->playerBulletSound, 0);
    return;

  case ENEMY_EXPLOSION_SOUND:
    Mix_PlayChannel(ENEMY_SOUND_CHANNEL, service->enemyExplosionSound, 0);
    return;
    
  default:
    return;
  }
}

void AudioService_Free(AudioService* service) {
  Mix_FreeChunk(service->enemyExplosionSound);
  Mix_FreeChunk(service->playerBulletSound);
  Mix_FreeMusic(service->backgroundMusic);
}
