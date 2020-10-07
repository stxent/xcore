/*
 * list.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
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

  while (current)
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

  while (current)
  {
    next = current->next;
    free(current);
    current = next;
  }
}
/*----------------------------------------------------------------------------*/
void listInit(struct List *list, size_t width)
{
  list->head = 0;
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
  if (list->head)
  {
    freeListChain(list->head);
    list->head = 0;
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

  while (*node)
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

  assert(current);

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

  while (node)
  {
    if (predicate(node->data, argument))
      return node;
    node = node->next;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
bool listInsert(struct List *list, struct ListNode *previous,
    const void *element)
{
  struct ListNode * const node =
      malloc(offsetof(struct ListNode, data) + list->width);

  if (node)
  {
    memcpy(node->data, element, list->width);

    if (!previous)
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

  if (node)
  {
    memcpy(node->data, element, list->width);
    node->next = 0;

    if (list->head)
    {
      struct ListNode *current = list->head;

      while (current->next)
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

  if (node)
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
