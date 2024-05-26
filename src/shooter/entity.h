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

#define ENTITY_LEFT(E) ((E)->x)
#define ENTITY_RIGHT(E) ((E)->x + (E)->w)
#define ENTITY_TOP(E) ((E)->y)
#define ENTITY_BOTTOM(E) ((E)->y + (E)->h)

#define ENTITY_WIDTH(E) ((E)->w)
#define ENTITY_HEIGHT(E) ((E)->h)

#define ENTITY_SET_POSITION(E, X, Y)		\
  do {						\
    (E)->x = (X);				\
    (E)->y = (Y);				\
  } while (0)

#define ENTITY_SET_HORIZONTAL_POSITION(E, X)	\
  do {						\
    (E)->x = (X);				\
  } while (0)

#define ENTITY_SET_VERTICAL_POSITION(E, Y)	\
  do {						\
    (E)->y = (Y);				\
  } while (0)

#define ENTITY_SET_POSITION_RELATIVE(E1, E2)		\
  do {							\
    (E1)->x = (E2)->x;					\
    (E1)->y = (E2)->y + ((E2)->h / 2) - ((E1)->h / 2);	\
  } while (0)

#define ENTITY_SET_VELOCITY(E, DX, DY)		\
  do {						\
    (E)->dx = (DX);				\
    (E)->dy = (DY);				\
  } while (0)

#define ENTITY_SET_HORIZONTAL_VELOCITY(E, DX)	\
  do {						\
    (E)->dx = (DX);				\
  } while (0)

#define ENTITY_SET_VERTICAL_VELOCITY(E, DY)	\
  do {						\
    (E)->dy = (DY);				\
  } while (0)

#define ENTITY_MOVE(E)				\
  do {						\
    (E)->x += (E)->dx;				\
    (E)->y += (E)->dy;				\
  } while (0)

bool Entity_HasCollided(const struct Entity* e1,
			const struct Entity* e2);

#endif // SHOOTER_ENTITY_H_
