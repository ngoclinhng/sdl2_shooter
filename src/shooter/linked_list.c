#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "shooter/linked_list.h"

void LinkedList_Init(LinkedList* list) {
  memset(list, 0, sizeof(LinkedList));
  list->tail = &list->head;
}

void LinkedList_Free(LinkedList* list) {
  ListNode* current = list->head.next;

  while (current != NULL) {
    ListNode* next = current->next;
    free(current->data);
    free(current);
    current = next;
  }

  list->head.next = NULL;
  list->tail = &list->head;
}

void* LinkedList_Add(LinkedList* list, size_t size) {
  ListNode* node;
  void* data;

  data = malloc(size);
  assert(data != NULL);
  memset(data, 0, size);

  node = malloc(sizeof(ListNode));
  assert(node != NULL);
  memset(node, 0, sizeof(ListNode));

  node->data = data;
  list->tail->next = node;
  list->tail = node;

  return data;
}

void LinkedList_ForEach(LinkedList* list, void (*func)(void*)) {
  ListNode* current = list->head.next;

  while (current != NULL) {
    ListNode* next = current->next;
    func(current->data);
    current = next;
  }
}

void LinkedList_ForEachWith(LinkedList* list, const void* other,
			    void (*func)(void*, const void*)) {
  ListNode* current = list->head.next;

  while (current != NULL) {
    ListNode* next = current->next;
    func(current->data, other);
    current = next;
  }
}

void LinkedList_ForEachAndPrune(LinkedList* list, void (*func)(void*),
				bool (*shouldRemove)(const void*)) {
  ListNode* current = list->head.next;
  ListNode* prev = &list->head;

  while (current != NULL) {
    if (shouldRemove(current->data)) {
      if (current == list->tail) {
	list->tail = prev;
      }

      ListNode* next = current->next;

      free(current->data);
      free(current);

      prev->next = next;
      current = prev;
    } else {
      func(current->data);
    }

    prev = current;
    current = current->next;
  }
}
