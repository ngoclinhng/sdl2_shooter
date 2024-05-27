#include "shooter/entity.h"

void Entity_Place(Entity* entity, int x, int y) {
  entity->hitbox.x = x;
  entity->hitbox.y = y;
}

void Entity_PlaceAtCenter(Entity* entity, const Entity* target) {
  int centerX = target->hitbox.x + target->hitbox.w / 2;
  int centerY = target->hitbox.y + target->hitbox.h / 2;

  entity->hitbox.x = centerX - entity->hitbox.w / 2;
  entity->hitbox.y = centerY - entity->hitbox.h / 2;
}

void Entity_SetVelocity(Entity* entity, float dx, float dy) {
  entity->dx = dx;
  entity->dy = dy;
}

void Entity_Move(Entity* entity, float dt) {
  entity->hitbox.x += (int)(entity->dx * dt);
  entity->hitbox.y += (int)(entity->dy * dt);
}

bool Entity_CheckCollision(const Entity* a, const Entity* b) {
  OutOfBoundsFlags flags = Entity_CheckOutOfBounds(a, &b->hitbox);

  if (flags & ~OUT_OF_BOUNDS_NONE) {
    return false;
  }

  return true;
}

OutOfBoundsFlags
Entity_CheckOutOfBounds(const Entity* entity, const SDL_Rect* bounds) {
  OutOfBoundsFlags flags = OUT_OF_BOUNDS_NONE;

  if (entity->hitbox.x + entity->hitbox.w <= bounds->x) {
    flags |= OUT_OF_BOUNDS_LEFT;
  }

  if (entity->hitbox.x >= bounds->x + bounds->w) {
    flags |= OUT_OF_BOUNDS_RIGHT;
  }

  if (entity->hitbox.y + entity->hitbox.h <= bounds->y) {
    flags |= OUT_OF_BOUNDS_TOP;
  }

  if (entity->hitbox.y >= bounds->y + bounds->h) {
    flags |= OUT_OF_BOUNDS_BOTTOM;
  }

  return flags;
}

bool Entity_IsLeftOfLine(const Entity* entity, int x) {
  return entity->hitbox.x + entity->hitbox.w <= x;
}

bool Entity_IsRightOfLine(const Entity* entity, int x) {
  return entity->hitbox.x >= x;
}

bool Entity_IsAboveLine(const Entity* entity, int y) {
  return entity->hitbox.y + entity->hitbox.h <= y;
}

bool Entity_IsBelowLine(const Entity* entity, int y) {
  return entity->hitbox.y >= y;
}
