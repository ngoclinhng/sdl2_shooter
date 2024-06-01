#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "shooter/entity.h"

#define TOP(R) ((R)->y)
#define RIGHT(R) ((R)->x + (R)->w)
#define BOTTOM(R) ((R)->y + (R)->h)
#define LEFT(R) ((R)->x)
#define CENTER_X(R) ((R)->x + (R)->w / 2)
#define CENTER_Y(R) ((R)->y + (R)->h / 2)

void Entity_SetRect(Entity* entity, SDL_Rect rect) {
  entity->hitbox = rect;
}

void Entity_SetPositionAndSize(Entity* entity, int x, int y, int w, int h) {
  entity->hitbox.x = x;
  entity->hitbox.y = y;
  entity->hitbox.w = w;
  entity->hitbox.h = h;
}

void Entity_Place(Entity* entity, int x, int y) {
  entity->hitbox.x = x;
  entity->hitbox.y = y;
}

void Entity_PlaceAtCenter(Entity* entity, const Entity* target) {
  entity->hitbox.x = CENTER_X(&target->hitbox) - entity->hitbox.w / 2;
  entity->hitbox.y = CENTER_Y(&target->hitbox) - entity->hitbox.h / 2;
}

void Entity_SetVelocity(Entity* entity, float dx, float dy) {
  entity->dx = dx;
  entity->dy = dy;
}

void Entity_SetVelocityX(Entity* entity, float dx) {
  entity->dx = dx;
}

void Entity_SetVelocityY(Entity* entity, float dy) {
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
  const SDL_Rect* hitbox = &entity->hitbox;

  if (RIGHT(hitbox) <= LEFT(bounds)) {
    flags |= OUT_OF_BOUNDS_LEFT;
  }

  if (LEFT(hitbox) >= RIGHT(bounds)) {
    flags |= OUT_OF_BOUNDS_RIGHT;
  }

  if (BOTTOM(hitbox) <= TOP(bounds)) {
    flags |= OUT_OF_BOUNDS_TOP;
  }

  if (TOP(hitbox) >= BOTTOM(bounds)) {
    flags |= OUT_OF_BOUNDS_BOTTOM;
  }

  return flags;
}

void Entity_Clip(Entity* entity, const SDL_Rect* bounds) {
  SDL_Rect* hitbox = &entity->hitbox;

  if (TOP(hitbox) < TOP(bounds)) {
    hitbox->y = TOP(bounds);
  }

  if (BOTTOM(hitbox) > BOTTOM(bounds)) {
    hitbox->y = BOTTOM(bounds) - hitbox->h;
  }

  if (LEFT(hitbox) < LEFT(bounds)) {
    hitbox->x = LEFT(bounds);
  }

  if (RIGHT(hitbox) > RIGHT(bounds)) {
    hitbox->x = RIGHT(bounds) - hitbox->w;
  }
}

void EntityList_Init(EntityList* list) {
  memset(list, 0, sizeof(EntityList));
  list->tail = &list->head;
}

Entity* EntityList_Add(EntityList* list, EntityType type) {
  EntityNode* newNode;

  newNode = malloc(sizeof(EntityNode));
  assert(newNode != NULL);

  memset(newNode, 0, sizeof(EntityNode));
  newNode->entity.type = type;
  
  list->tail->next = newNode;
  list->tail = newNode;

  return &newNode->entity;
}

void EntityList_Free(EntityList* list) {
  EntityNode* current = list->head.next;

  while (current != NULL) {
    EntityNode* next = current->next;
    free(current);
    current = next;
  }

  list->head.next = NULL;
  list->tail = &list->head;
}

void EntityList_ForEach(EntityList* list, void (*processFunc)(Entity*)) {
  EntityNode* node;

  for (node = list->head.next; node != NULL; node = node->next) {
    processFunc(&node->entity);
  }
}

void EntityList_ForEachAndPrune(EntityList* list,
				void (*processFunc)(Entity*),
				bool (*shouldRemove)(const Entity*)) {
  EntityNode* prev = &list->head;
  EntityNode* node = list->head.next;

  while (node != NULL) {
    if (shouldRemove(&node->entity)) {      
      if (node == list->tail) {
	list->tail = prev;
      }

      prev->next = node->next;
      free(node);
      node = prev->next;
    } else {
      processFunc(&node->entity);
      prev = node;
      node = node->next;
    }
  }
}
