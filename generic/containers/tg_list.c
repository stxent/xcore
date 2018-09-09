/*
 * tg_list.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <xcore/containers/tg_list.h>
/*----------------------------------------------------------------------------*/
struct TgListNode
{
  struct TgListNode *next;
  unsigned char data[];
};
/*----------------------------------------------------------------------------*/
size_t tgListCountNodes(const struct TgListNode *current)
{
  size_t count = 0;

  while (current)
  {
    ++count;
    current = current->next;
  }

  return count;
}
/*----------------------------------------------------------------------------*/
struct TgListNode *tgListErase(struct TgListNode **first,
    struct TgListNode *node)
{
  struct TgListNode * const next = node->next;
  struct TgListNode **current = first;

  while (*current != node)
    current = &(*current)->next;

  assert(current);

  *current = next;
  free(node);

  return next;
}
/*----------------------------------------------------------------------------*/
struct TgListNode *tgListFindIf(struct TgListNode *first,
    const void *element, int (*comparator)(const void *, const void *))
{
  struct TgListNode *node = first;

  while (node)
  {
    if (!comparator(node->data, element))
      return node;
    node = node->next;
  }

  return 0;
}
/*----------------------------------------------------------------------------*/
void tgListFreeChain(struct TgListNode *node)
{
  struct TgListNode *next;

  while (node)
  {
    next = node->next;
    free(node);
    node = next;
  }
}
