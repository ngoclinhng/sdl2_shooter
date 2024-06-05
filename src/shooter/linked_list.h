#ifndef SHOOTER_LINKED_LIST_H_
#define SHOOTER_LINKED_LIST_H_

#include <stddef.h>
#include <stdbool.h>

typedef struct ListNode {
  void* data;
  struct ListNode* next;
} ListNode;

typedef struct LinkedList {
  ListNode head;
  ListNode* tail;
} LinkedList;

void LinkedList_Init(LinkedList* list);
void LinkedList_Free(LinkedList* list);

void* LinkedList_Add(LinkedList* list, size_t size);

void LinkedList_ForEach(LinkedList* list, void (*func)(void*));

void LinkedList_ForEachWith(LinkedList* list, const void* other,
			    void (*func)(void*, const void*));

void LinkedList_ForEachAndPrune(LinkedList* list, void (*func)(void*),
				bool (*shouldRemove)(const void*));

#endif // SHOOTER_LINKED_LIST_H_
