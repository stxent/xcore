/*
 * list.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdlib.h>
#include <xcore/containers/list.h>
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
enum Result listInit(struct List *list, size_t width)
{
  list->first = 0;
  list->width = width;

  return E_OK;
}
/*----------------------------------------------------------------------------*/
void listDeinit(struct List *list)
{
  freeListChain(list->first);
}
/*----------------------------------------------------------------------------*/
void listClear(struct List *list)
{
  if (list->first)
  {
    freeListChain(list->first);
    list->first = 0;
  }
}
/*----------------------------------------------------------------------------*/
struct ListNode *listErase(struct List *list, struct ListNode *node)
{
  struct ListNode * const next = node->next;
  struct ListNode **current = &list->first;

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
  struct ListNode *node = list->first;

  while (node)
  {
    if (!memcmp(node->data, element, list->width))
      return node;
    node = node->next;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
struct ListNode *listFindCompared(struct List *list, const void *element,
    int (*compare)(const void *, const void *))
{
  struct ListNode *node = list->first;

  while (node)
  {
    if (!compare(node->data, element))
      return node;
    node = node->next;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
enum Result listInsert(struct List *list, struct ListNode *previous,
    const void *element)
{
  struct ListNode * const node =
      malloc(offsetof(struct ListNode, data) + list->width);

  if (!node)
    return E_MEMORY;

  memcpy(node->data, element, list->width);

  if (!previous)
  {
    node->next = list->first;
    list->first = node;
  }
  else
  {
    node->next = previous->next;
    previous->next = node;
  }

  return E_OK;
}
/*----------------------------------------------------------------------------*/
enum Result listPush(struct List *list, const void *element)
{
  struct ListNode * const node =
      malloc(offsetof(struct ListNode, data) + list->width);

  if (!node)
    return E_MEMORY;

  memcpy(node->data, element, list->width);
  node->next = 0;

  if (list->first)
  {
    struct ListNode *current = list->first;

    while (current->next)
      current = current->next;

    current->next = node;
  }
  else
  {
    list->first = node;
  }

  return E_OK;
}
/*----------------------------------------------------------------------------*/
size_t listCapacity(const struct List *list)
{
  return countListNodes(list->first);
}
/*----------------------------------------------------------------------------*/
size_t listSize(const struct List *list)
{
  return countListNodes(list->first);
}