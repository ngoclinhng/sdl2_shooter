#ifndef SHOOTER_ENTITY_H_
#define SHOOTER_ENTITY_H_

#include <stdbool.h>
#include "shooter/texture_manager.h"

struct Entity {
  float x, y;
  float dx, dy;
  int w, h;
  enum TextureType textureType;
};

#define ENTITY_SET_POSITION(entity, X, Y)	\
  do {						\
    (entity)->x = (X);				\
    (entity)->y = (Y);				\
  } while (0)

#define ENTITY_SET_POSITION_RELATIVE(e1, e2)		\
  do {							\
    (e1)->x = (e2)->x;					\
    (e1)->y = (e2)->y + ((e2)->h / 2) - ((e1)->h / 2);	\
  } while (0)

#define ENTITY_SET_VELOCITY(entity, DX, DY)	\
  do {						\
    (entity)->dx = (DX);			\
    (entity)->dy = (DY);			\
  } while (0)

#define ENTITY_SET_VELOCITY_X(entity, DX)	\
  do {						\
    (entity)->dx = (DX);			\
  } while (0)

#define ENTITY_SET_VELOCITY_Y(entity, DY)	\
  do {						\
    (entity)->dy = (DY);			\
  } while (0)

#define ENTITY_MOVE(entity)			\
  do {						\
    (entity)->x += (entity)->dx;		\
    (entity)->y += (entity)->dy;		\
  } while (0)

bool Entity_HasCollided(const struct Entity* e1,
			const struct Entity* e2);

#endif // SHOOTER_ENTITY_H_
