#ifndef SHOOTER_GEOMETRY_H_
#define SHOOTER_GEOMETRY_H_

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "shooter/textures.h"

typedef enum EntityType {
  ENTITY_PLAYER,
  ENTITY_ENEMY,
  ENTITY_PLAYER_BULLET,
  ENTITY_ENEMY_BULLET
} EntityType;

typedef enum OutOfBoundsFlags {
  OUT_OF_BOUNDS_NONE = 0,
  OUT_OF_BOUNDS_LEFT = 1 << 0,
  OUT_OF_BOUNDS_RIGHT = 1 << 1,
  OUT_OF_BOUNDS_TOP = 1 << 2,
  OUT_OF_BOUNDS_BOTTOM = 1 << 3
} OutOfBoundsFlags;

typedef struct Entity {
  EntityType type;
  TextureType textureType;
  SDL_Rect hitbox;
  float dx, dy;
  int health;
  int reloadTime;
} Entity;

typedef struct EntityNode {
  Entity entity;
  struct EntityNode* next;
} EntityNode;

typedef struct EntityList {
  EntityNode head;
  EntityNode *tail;
} EntityList;

#define ENTITY_W(E) ((E)->hitbox.w)
#define ENTITY_H(E) ((E)->hitbox.h)

void Entity_SetRect(Entity* entity, SDL_Rect rect);
void Entity_SetPositionAndSize(Entity* entity, int x, int y, int w, int h);

void Entity_Place(Entity* entity, int x, int y);
void Entity_PlaceAtCenter(Entity* entity, const Entity* target);

void Entity_SetTexture(Entity* entity, Textures* textures,
		       TextureType textureType);

void Entity_Render(const Entity* entity, Textures* textures);

void Entity_SetVelocity(Entity* entity, float dx, float dy);
void Entity_SetVelocityX(Entity* entity, float dx);
void Entity_SetVelocityY(Entity* entity, float dy);
void Entity_Move(Entity* entity, float dt);

void Entity_GetDirectionalVector(const Entity* e1, const Entity* e2,
				 float* dx, float* dy);

bool Entity_CheckCollision(const Entity* entity1, const Entity* entity2);

OutOfBoundsFlags
Entity_CheckOutOfBounds(const Entity* entity, const SDL_Rect* bounds);

void Entity_Clip(Entity* entity, const SDL_Rect* bounds);

bool Entity_IsToTheLeftOf(const Entity* entity, int x);
bool Entity_IsToTheRightOf(const Entity* entity, int x);

bool Entity_IsAbove(const Entity* entity, int y);
bool Entity_IsBelow(const Entity* entity, int y);


void EntityList_Init(EntityList* list);
Entity* EntityList_Add(EntityList* list, EntityType type);
void EntityList_Free(EntityList* list);

void EntityList_ForEach(EntityList* list, void (*processFunc)(Entity*));

void EntityList_ForEachWith(EntityList* list, Entity* other,
			    void (*processFunc)(Entity*, Entity*));

void EntityList_ForEachAndPrune(EntityList* list,
				void (*processFunc)(Entity*),
				bool (*shouldRemove)(const Entity*));

#endif // SHOOTER_GEOMETRY_H_
