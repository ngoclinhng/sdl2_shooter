#ifndef SHOOTER_AUDIO_SERVICE_H_
#define SHOOTER_AUDIO_SERVICE_H_

#include <SDL2/SDL_mixer.h>

typedef enum SoundType {
  BACKGROUND_MUSIC,
  PLAYER_BULLET_SOUND,
  ENEMY_EXPLOSION_SOUND
} SoundType;

typedef struct AudioService {
  Mix_Music* backgroundMusic;
  Mix_Chunk* playerBulletSound;
  Mix_Chunk* enemyExplosionSound;
} AudioService;

void AudioService_Init(AudioService* service);
void AudioService_Play(AudioService* service, SoundType sound);
void AudioService_Free(AudioService* service);

#endif // SHOOTER_AUDIO_SERVICE_H_
