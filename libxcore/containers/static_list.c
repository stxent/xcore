/*
 * static_list.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <containers/static_list.h>
/*----------------------------------------------------------------------------*/
static size_t countListNodes(const struct StaticListNode *current)
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
enum result staticListInit(struct StaticList *list, size_t width,
    size_t capacity)
{
  if (!capacity)
    return E_VALUE;

  const size_t nodeSize = offsetof(struct StaticListNode, data) + width;

  list->data = malloc(nodeSize * capacity);
  list->first = list->pool = 0;
  list->width = width;

  uintptr_t position = (uintptr_t)list->data;

  for (size_t index = 0; index < capacity; ++index)
  {
    struct StaticListNode * const node = (struct StaticListNode *)position;

    node->next = list->pool;
    list->pool = node;
    position += nodeSize;
  }

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void staticListDeinit(struct StaticList *list)
{
  free(list->data);
}
/*----------------------------------------------------------------------------*/
void staticListClear(struct StaticList *list)
{
  if (list->first)
  {
    struct StaticListNode *current = list->first;

    while (current->next)
      current = current->next;

    current->next = list->pool;
    list->pool = list->first;
    list->first = 0;
  }
}
/*----------------------------------------------------------------------------*/
void *staticListErase(struct StaticList *list, void *node)
{
  struct StaticListNode * const target = node;

  if (list->first != node)
  {
    struct StaticListNode *current = list->first;

    while (current->next != node)
      current = current->next;

    current->next = target->next;
  }
  else
  {
    list->first = list->first->next;
  }

  struct StaticListNode * const next = target->next;

  target->next = list->pool;
  list->pool = target;

  return next;
}
/*----------------------------------------------------------------------------*/
enum result staticListPush(struct StaticList *list, const void *element)
{
  struct StaticListNode * const node = list->pool;

  if (!node)
    return E_MEMORY;

  list->pool = list->pool->next;

  memcpy(node->data, &element, list->width);
  node->next = list->first;
  list->first = node;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
size_t staticListCapacity(const struct StaticList *list)
{
  return countListNodes(list->pool);
}
/*----------------------------------------------------------------------------*/
size_t staticListSize(const struct StaticList *list)
{
  return countListNodes(list->first);
}
