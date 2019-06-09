/*
 * xcore/containers/list.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_LIST_H_
#define XCORE_CONTAINERS_LIST_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <xcore/helpers.h>
#include <xcore/error.h>
/*----------------------------------------------------------------------------*/
struct ListNode
{
  /** Pointer to the next element or to an end of the list. */
  struct ListNode *next;
  /** Beginning of the user data. */
  unsigned char data[];
};
/*----------------------------------------------------------------------------*/
struct List
{
  /** First element of the list containing data nodes. */
  struct ListNode *head;
  /** Size in bytes of each element. */
  size_t width;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

void listInit(struct List *, size_t);
void listDeinit(struct List *);
void listClear(struct List *);
void listErase(struct List *, const void *);
void listEraseIf(struct List *, void *, bool (*)(const void *, void *));
struct ListNode *listEraseNode(struct List *, struct ListNode *);
struct ListNode *listFind(struct List *, const void *);
struct ListNode *listFindIf(struct List *, void *,
    bool (*)(const void *, void *));
bool listInsert(struct List *, struct ListNode *, const void *);
bool listPushBack(struct List *, const void *);
bool listPushFront(struct List *, const void *);
size_t listSize(const struct List *);

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

static inline void listData(const struct List *list,
    const struct ListNode *node, void *element)
{
  memcpy(element, node->data, list->width);
}

static inline struct ListNode *listFront(const struct List *list)
{
  return list->head;
}

static inline struct ListNode *listNext(const struct ListNode *node)
{
  return node->next;
}

static inline bool listEmpty(const struct List *list)
{
  return list->head == 0;
}

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_LIST_H_ */
