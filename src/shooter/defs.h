#ifndef SHOOTER_DEFS_H_
#define SHOOTER_DEFS_H_

#define SHOOTER_FPS 60

// Uncomment this for other machines
// #define SHOOTER_WINDOW_WIDTH 1280
// #define SHOOTER_WINDOW_HEIGHT 720

// Resonably good resolution for MacOS Pro 13 inch.
#define SHOOTER_WINDOW_WIDTH 960
#define SHOOTER_WINDOW_HEIGHT 540

#define SHOOTER_PLAYER_SPEED 4
#define SHOOTER_PLAYER_BULLET_SPEED 16
#define SHOOTER_ENEMY_BULLET_SPEED 8

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

#endif // SHOOTER_DEFS_H_
