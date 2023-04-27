/*
 * list.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/list.h>
#include <assert.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
struct PredicateData
{
  const void *element;
  size_t width;
};
/*----------------------------------------------------------------------------*/
static size_t countListNodes(const struct ListNode *);
static bool defaultComparator(const void *a, void *b);
static void freeListChain(struct ListNode *);
/*----------------------------------------------------------------------------*/
static size_t countListNodes(const struct ListNode *current)
{
  size_t result = 0;

  while (current != NULL)
  {
    ++result;
    current = current->next;
  }

  return result;
}
/*----------------------------------------------------------------------------*/
static bool defaultComparator(const void *a, void *b)
{
  const struct PredicateData * const data = b;
  return memcmp(a, data->element, data->width) == 0;
}
/*----------------------------------------------------------------------------*/
static void freeListChain(struct ListNode *current)
{
  struct ListNode *next;

  while (current != NULL)
  {
    next = current->next;
    free(current);
    current = next;
  }
}
/*----------------------------------------------------------------------------*/
void listInit(struct List *list, size_t width)
{
  list->head = NULL;
  list->width = width;
}
/*----------------------------------------------------------------------------*/
void listDeinit(struct List *list)
{
  freeListChain(list->head);
}
/*----------------------------------------------------------------------------*/
void listClear(struct List *list)
{
  if (list->head != NULL)
  {
    freeListChain(list->head);
    list->head = NULL;
  }
}
/*----------------------------------------------------------------------------*/
void listErase(struct List *list, const void *element)
{
  struct PredicateData data = {element, list->width};
  listEraseIf(list, &data, defaultComparator);
}
/*----------------------------------------------------------------------------*/
void listEraseIf(struct List *list, void *argument,
    bool (*predicate)(const void *, void *))
{
  struct ListNode **node = &list->head;

  while (*node != NULL)
  {
    if (predicate((*node)->data, argument))
    {
      struct ListNode * const target = *node;

      *node = (*node)->next;
      free(target);
    }
    else
      node = &(*node)->next;
  }
}
/*----------------------------------------------------------------------------*/
struct ListNode *listEraseNode(struct List *list, struct ListNode *node)
{
  struct ListNode * const next = node->next;
  struct ListNode **current = &list->head;

  while (*current != node)
    current = &(*current)->next;

  assert(current != NULL);

  *current = next;
  free(node);

  return next;
}
/*----------------------------------------------------------------------------*/
struct ListNode *listFind(struct List *list, const void *element)
{
  struct PredicateData data = {element, list->width};
  return listFindIf(list, &data, defaultComparator);
}
/*----------------------------------------------------------------------------*/
struct ListNode *listFindIf(struct List *list, void *argument,
    bool (*predicate)(const void *, void *))
{
  struct ListNode *node = list->head;

  while (node != NULL)
  {
    if (predicate(node->data, argument))
      return node;
    node = node->next;
  }

  return NULL;
}
/*----------------------------------------------------------------------------*/
bool listInsert(struct List *list, struct ListNode *previous,
    const void *element)
{
  struct ListNode * const node =
      malloc(offsetof(struct ListNode, data) + list->width);

  if (node != NULL)
  {
    memcpy(node->data, element, list->width);

    if (previous == NULL)
    {
      node->next = list->head;
      list->head = node;
    }
    else
    {
      node->next = previous->next;
      previous->next = node;
    }

    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
bool listPushBack(struct List *list, const void *element)
{
  struct ListNode * const node =
      malloc(offsetof(struct ListNode, data) + list->width);

  if (node != NULL)
  {
    memcpy(node->data, element, list->width);
    node->next = NULL;

    if (list->head != NULL)
    {
      struct ListNode *current = list->head;

      while (current->next != NULL)
        current = current->next;

      current->next = node;
    }
    else
      list->head = node;

    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
bool listPushFront(struct List *list, const void *element)
{
  struct ListNode * const node =
      malloc(offsetof(struct ListNode, data) + list->width);

  if (node != NULL)
  {
    memcpy(node->data, element, list->width);

    node->next = list->head;
    list->head = node;

    return true;
  }
  else
    return false;
}
/*----------------------------------------------------------------------------*/
size_t listSize(const struct List *list)
{
  return countListNodes(list->head);
}
