#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdbool.h>
#include <cmocka.h>
#include "shooter/linked_list.h"

typedef struct Point2D {
  int x;
  int y;
} Point2D;

static void Point2D_AddOne(void* point) {
  Point2D* p = (Point2D*) point;
  p->x += 1;
  p->y += 1;
}

static void Point2D_Add(void* point1, void* point2) {
  Point2D* p = (Point2D*) point1;
  Point2D* q = (Point2D*) point2;

  p->x += q->x;
  p->y += q->y;
}

static bool Point2D_IsOnXAxis(const void* point) {
  Point2D* p = (Point2D*) point;
  return p->y == 0;
}

static void test_LinkedList_Init(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  assert_ptr_equal(&list.head, list.tail);
  assert_null(list.head.next);

  assert_null(list.head.data);
  assert_null(list.head.next);
}

static void test_LinkedList_Add(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);
  
  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));

  assert_non_null(p);
  assert_ptr_equal(p, list.tail->data);
  assert_int_equal(p->x, 0);
  assert_int_equal(p->y, 0);

  assert_ptr_equal(list.head.next, list.tail);
  assert_null(list.tail->next);

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));

  assert_non_null(q);
  assert_ptr_equal(q, list.tail->data);
  assert_int_equal(q->x, 0);
  assert_int_equal(q->y, 0);
  
  assert_ptr_equal(p, list.head.next->data);
  assert_ptr_equal(list.head.next->next, list.tail);
  assert_null(list.tail->next);

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEach(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 0;
  p->y = -1;

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));
  q->x = 4;
  q->y = 8;

  LinkedList_ForEach(&list, &Point2D_AddOne);

  assert_int_equal(p->x, 1);
  assert_int_equal(p->y, 0);

  assert_int_equal(q->x, 5);
  assert_int_equal(q->y, 9);

  assert_ptr_equal(list.head.next->data, p);
  assert_ptr_equal(list.head.next->next, list.tail);
  assert_ptr_equal(list.tail->data, q);
  assert_null(list.tail->next);

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEachWith(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 0;
  p->y = -1;

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));
  q->x = 4;
  q->y = 8;

  Point2D r = {.x = -2, .y = 16};

  LinkedList_ForEachWith(&list, &r, &Point2D_Add);

  assert_int_equal(p->x, -2);
  assert_int_equal(p->y, 15);

  assert_int_equal(q->x, 2);
  assert_int_equal(q->y, 24);

  assert_ptr_equal(list.head.next->data, p);
  assert_ptr_equal(list.head.next->next, list.tail);
  assert_ptr_equal(list.tail->data, q);
  assert_null(list.tail->next);

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEachAndPrune_RemoveSingleton(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 1;
  p->y = 0;

  LinkedList_ForEachAndPrune(&list, &Point2D_AddOne, &Point2D_IsOnXAxis);

  assert_ptr_equal(&list.head, list.tail);
  assert_ptr_equal(list.tail->next, NULL);

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEachAndPrune_RemoveFirst(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 1;
  p->y = 0;

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));
  q->x = 1;
  q->y = 2;

  Point2D* r;
  r = LinkedList_Add(&list, sizeof(Point2D));
  r->x = 0;
  r->y = 4;

  LinkedList_ForEachAndPrune(&list, &Point2D_AddOne, &Point2D_IsOnXAxis);

  assert_int_equal(q->x, 2);
  assert_int_equal(q->y, 3);

  assert_int_equal(r->x, 1);
  assert_int_equal(r->y, 5);

  assert_non_null(list.head.next);
  assert_ptr_equal(list.head.next->data, q);

  assert_non_null(list.head.next->next);
  assert_ptr_equal(list.head.next->next, list.tail);
  
  assert_ptr_equal(list.tail->data, r);
  assert_ptr_equal(list.tail->next, NULL);  

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEachAndPrune_RemoveMiddle(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 1;
  p->y = -1;

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));
  q->x = 4;
  q->y = 0;

  Point2D* r;
  r = LinkedList_Add(&list, sizeof(Point2D));
  r->x = 0;
  r->y = 4;

  LinkedList_ForEachAndPrune(&list, &Point2D_AddOne, &Point2D_IsOnXAxis);

  assert_int_equal(p->x, 2);
  assert_int_equal(p->y, 0);

  assert_int_equal(r->x, 1);
  assert_int_equal(r->y, 5);

  assert_non_null(list.head.next);
  assert_ptr_equal(list.head.next->data, p);

  assert_non_null(list.head.next->next);
  assert_ptr_equal(list.head.next->next, list.tail);
  
  assert_ptr_equal(list.tail->data, r);
  assert_ptr_equal(list.tail->next, NULL);  

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEachAndPrune_RemoveLast(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 1;
  p->y = -1;

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));
  q->x = 0;
  q->y = 4;

  Point2D* r;
  r = LinkedList_Add(&list, sizeof(Point2D));
  r->x = 4;
  r->y = 0;

  LinkedList_ForEachAndPrune(&list, &Point2D_AddOne, &Point2D_IsOnXAxis);

  assert_int_equal(p->x, 2);
  assert_int_equal(p->y, 0);

  assert_int_equal(q->x, 1);
  assert_int_equal(q->y, 5);

  assert_non_null(list.head.next);
  assert_ptr_equal(list.head.next->data, p);

  assert_non_null(list.head.next->next);
  assert_ptr_equal(list.head.next->next, list.tail);
  
  assert_ptr_equal(list.tail->data, q);
  assert_ptr_equal(list.tail->next, NULL);  

  LinkedList_Free(&list);
}

static void test_LinkedList_ForEachAndPrune_RemoveAll(void** state) {
  (void) state;

  LinkedList list;
  LinkedList_Init(&list);

  Point2D* p;
  p = LinkedList_Add(&list, sizeof(Point2D));
  p->x = 1;
  p->y = 0;

  Point2D* q;
  q = LinkedList_Add(&list, sizeof(Point2D));
  q->x = 2;
  q->y = 0;

  Point2D* r;
  r = LinkedList_Add(&list, sizeof(Point2D));
  r->x = 3;
  r->y = 0;

  LinkedList_ForEachAndPrune(&list, &Point2D_AddOne, &Point2D_IsOnXAxis);

  assert_ptr_equal(&list.head, list.tail);
  assert_ptr_equal(list.tail->next, NULL);
  assert_ptr_equal(list.tail->data, NULL);

  LinkedList_Free(&list);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_LinkedList_Init),
    cmocka_unit_test(test_LinkedList_Add),
    cmocka_unit_test(test_LinkedList_ForEach),
    cmocka_unit_test(test_LinkedList_ForEachWith),
    cmocka_unit_test(test_LinkedList_ForEachAndPrune_RemoveSingleton),
    cmocka_unit_test(test_LinkedList_ForEachAndPrune_RemoveFirst),
    cmocka_unit_test(test_LinkedList_ForEachAndPrune_RemoveMiddle),
    cmocka_unit_test(test_LinkedList_ForEachAndPrune_RemoveLast),
    cmocka_unit_test(test_LinkedList_ForEachAndPrune_RemoveAll)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
