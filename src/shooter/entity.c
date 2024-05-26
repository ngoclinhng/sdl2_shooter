#include "shooter/entity.h"

bool Entity_HasCollided(const struct Entity* entity1,
			const struct Entity* entity2) {
  return ENTITY_LEFT(entity1) < ENTITY_RIGHT(entity2) &&
    ENTITY_RIGHT(entity1) > ENTITY_LEFT(entity2) &&
    ENTITY_TOP(entity1) < ENTITY_BOTTOM(entity2) &&
    ENTITY_BOTTOM(entity1) > ENTITY_TOP(entity2);
}
