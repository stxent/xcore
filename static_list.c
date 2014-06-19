/*
 * static_list.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <static_list.h>
/*----------------------------------------------------------------------------*/
static unsigned int countListNodes(const struct StaticListNode *current)
{
  unsigned int result = 0;

  while (current)
  {
    ++result;
    current = current->next;
  }

  return result;
}
/*----------------------------------------------------------------------------*/
enum result staticListInit(struct StaticList *list, unsigned int width,
    unsigned int capacity)
{
  if (!capacity || capacity > USHRT_MAX)
    return E_VALUE;

  list->data = malloc((sizeof(struct StaticListNode *) + width) * capacity);
  list->first = list->pool = 0;
  list->width = width;

  for (unsigned int pos = 0; pos < capacity; ++pos)
  {
    struct StaticListNode *node;

    node = (struct StaticListNode *)((unsigned char *)list->data
        + (sizeof(struct StaticListNode *) + width) * pos);
    node->next = list->pool;
    list->pool = node;
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
    list->first = list->first->next;

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
unsigned int staticListCapacity(const struct StaticList *list)
{
  return countListNodes(list->pool);
}
/*----------------------------------------------------------------------------*/
unsigned int staticListSize(const struct StaticList *list)
{
  return countListNodes(list->first);
}
