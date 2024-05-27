#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "entity.h"

static void moveEntity(Entity* e) {
  Entity_Move(e, 1.0f);
}

static void test_Entity_SetRect(void** state) {
  (void) state;

  Entity e;
  SDL_Rect rect = {10, 20, 30, 40};

  Entity_SetRect(&e, rect);

  assert_int_equal(e.hitbox.x, 10);
  assert_int_equal(e.hitbox.y, 20);
  assert_int_equal(e.hitbox.w, 30);
  assert_int_equal(e.hitbox.h, 40);
}

static void test_Entity_SetPositionAndSize(void** state) {
  (void) state;

  Entity e;

  Entity_SetPositionAndSize(&e, 10, 20, 30, 40);

  assert_int_equal(e.hitbox.x, 10);
  assert_int_equal(e.hitbox.y, 20);
  assert_int_equal(e.hitbox.w, 30);
  assert_int_equal(e.hitbox.h, 40);
}

static void test_Entity_Place(void** state) {
  (void) state;

  Entity e = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};
  Entity_Place(&e, 25, 50);

  assert_int_equal(e.hitbox.x, 25);
  assert_int_equal(e.hitbox.y, 50);
  assert_int_equal(e.hitbox.w, 50);
  assert_int_equal(e.hitbox.h, 50);
}

static void test_Entity_PlaceAtCenter(void** state) {
  (void) state;

  Entity target = {.hitbox = {.x = 0, .y = 0, .w = 100, .h = 100}};
  Entity entity = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};

  Entity_PlaceAtCenter(&entity, &target);

  assert_int_equal(entity.hitbox.x, 25);
  assert_int_equal(entity.hitbox.y, 25);
  assert_int_equal(entity.hitbox.w, 50);
  assert_int_equal(entity.hitbox.h, 50);
}

static void test_Entity_SetVelocity(void** state) {
  (void) state;

  Entity e;
  Entity_SetVelocity(&e, 1.0f, 2.0f);

  assert_float_equal(e.dx, 1.0f, 0.001f);
  assert_float_equal(e.dy, 2.0f, 0.001f);
}

static void test_Entity_Move(void** state) {
  (void) state;

  Entity e = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};

  Entity_SetVelocity(&e, 5.0f, 10.0f);
  Entity_Move(&e, 1.0f);

  assert_int_equal(e.hitbox.x, 5);
  assert_int_equal(e.hitbox.y, 10);
  assert_int_equal(e.hitbox.w, 50);
  assert_int_equal(e.hitbox.h, 50);

  Entity_Move(&e, 1.0f);

  assert_int_equal(e.hitbox.x, 10);
  assert_int_equal(e.hitbox.y, 20);
  assert_int_equal(e.hitbox.w, 50);
  assert_int_equal(e.hitbox.h, 50);
}

static void test_Entity_CheckCollision(void** state) {
  (void) state;

  // Two rectangles that overlap
  Entity a = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};
  Entity b = {.hitbox = {.x = 25, .y = 25, .w = 50, .h = 50}};
  assert_true(Entity_CheckCollision(&a, &b));

  // Two rectangles that do not overlap
  Entity c = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};
  Entity d = {.hitbox = {.x = 100, .y = 100, .w = 50, .h = 50}};
  assert_false(Entity_CheckCollision(&c, &d));

  // Two rectangles that touch at a corner (not overlapping)
  Entity e = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};
  Entity f = {.hitbox = {.x = 50, .y = 50, .w = 50, .h = 50}};
  assert_false(Entity_CheckCollision(&e, &f));

  // Two rectangles where one is entirely within the other
  Entity g = {.hitbox = {.x = 0, .y = 0, .w = 100, .h = 100}};
  Entity h = {.hitbox = {.x = 25, .y = 25, .w = 50, .h = 50}};
  assert_true(Entity_CheckCollision(&g, &h));

  // Two rectangles that touch along an edge (not overlapping)
  Entity i = {.hitbox = {.x = 0, .y = 0, .w = 50, .h = 50}};
  Entity j = {.hitbox = {.x = 50, .y = 0, .w = 50, .h = 50}};
  assert_false(Entity_CheckCollision(&i, &j));
}

static void test_EntityList_Init(void** state) {
  (void) state;

  EntityList list;
  EntityList_Init(&list);

  EntityNode expectedHead = {.entity = {0}, .next = NULL};
  assert_memory_equal(&list.head, &expectedHead, sizeof(EntityNode));
  assert_ptr_equal(list.tail, &list.head);
}

static void test_EntityList_Add(void** state) {
  (void) state;

  EntityList list;
  EntityList_Init(&list);

  Entity* newEntity = EntityList_Add(&list, ENTITY_BULLET);

  assert_non_null(newEntity);
  assert_ptr_equal(list.tail, list.head.next);
  assert_ptr_equal(newEntity, &list.tail->entity);
  assert_int_equal(newEntity->type, ENTITY_BULLET);

  EntityList_Free(&list);
}

static void test_EntityList_Free(void** state) {
  (void) state;

  EntityList list;
  EntityList_Init(&list);

  for (int i = 0; i < 10; ++i) {
    EntityList_Add(&list, ENTITY_BULLET);
  }

  EntityList_Free(&list);

  assert_null(list.head.next);
  assert_ptr_equal(list.tail, &list.head);
}

static void test_EntityList_ForEach(void** state) {
  (void) state;

  EntityList list;
  EntityList_Init(&list);

  Entity *e1 = EntityList_Add(&list, ENTITY_ENEMY);
  Entity *e2 = EntityList_Add(&list, ENTITY_ENEMY);

  Entity_SetVelocity(e1, 5.0f, 10.0f);
  Entity_SetVelocity(e2, 50.0f, 100.0f);

  EntityList_ForEach(&list, &moveEntity);

  assert_int_equal(e1->hitbox.x, 5);
  assert_int_equal(e1->hitbox.y, 10);

  assert_int_equal(e2->hitbox.x, 50);
  assert_int_equal(e2->hitbox.y, 100);

  EntityList_Free(&list);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_Entity_SetRect),
    cmocka_unit_test(test_Entity_SetPositionAndSize),
    cmocka_unit_test(test_Entity_Place),
    cmocka_unit_test(test_Entity_PlaceAtCenter),
    cmocka_unit_test(test_Entity_SetVelocity),
    cmocka_unit_test(test_Entity_Move),
    cmocka_unit_test(test_Entity_CheckCollision),
    cmocka_unit_test(test_EntityList_Init),
    cmocka_unit_test(test_EntityList_Add),
    cmocka_unit_test(test_EntityList_Free),
    cmocka_unit_test(test_EntityList_ForEach)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
