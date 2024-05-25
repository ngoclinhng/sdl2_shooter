#include "shooter/entity.h"

#define LEFT(entity) ((entity)->x)
#define RIGHT(entity) ((entity)->x + (entity)->w)
#define TOP(entity) ((entity)->y)
#define BOTTOM(entity) ((entity)->y + (entity)->h)

bool Entity_HasCollided(const struct Entity* entity1,
			const struct Entity* entity2) {
  return LEFT(entity1) < RIGHT(entity2) &&
    RIGHT(entity1) > LEFT(entity2) &&
    TOP(entity1) < BOTTOM(entity2) &&
    BOTTOM(entity1) > TOP(entity2);
}

#undef LEFT
#undef RIGHT
#undef TOP
#undef BOTTOM
