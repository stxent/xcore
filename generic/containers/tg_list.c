/*
 * tg_list.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/tg_list.h>
#include <assert.h>
/*----------------------------------------------------------------------------*/
struct TgListNode
{
  struct TgListNode *next;
  unsigned char data[];
};
/*----------------------------------------------------------------------------*/
void tgListAppend(struct TgListNode **first, struct TgListNode *node)
{
  if (*first)
  {
    struct TgListNode *current = *first;

    while (current->next)
      current = current->next;

    current->next = node;
  }
  else
    *first = node;
}
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
void tgListEraseIf(struct TgListNode **first, void *argument,
    bool (*predicate)(const void *, void *))
{
  struct TgListNode **node = first;

  while (*node)
  {
    if (predicate((*node)->data, argument))
    {
      struct TgListNode * const target = *node;

      *node = (*node)->next;
      free(target);
    }
    else
      node = &(*node)->next;
  }
}
/*----------------------------------------------------------------------------*/
struct TgListNode *tgListEraseNode(struct TgListNode **first,
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
struct TgListNode *tgListFindIf(struct TgListNode *first, void *argument,
    bool (*predicate)(const void *, void *))
{
  struct TgListNode *node = first;

  while (node)
  {
    if (predicate(node->data, argument))
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
