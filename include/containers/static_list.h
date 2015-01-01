/*
 * containers/static_list.h
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef CONTAINERS_STATIC_LIST_H_
#define CONTAINERS_STATIC_LIST_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <string.h>
#include <error.h>
/*----------------------------------------------------------------------------*/
struct StaticListNode
{
  struct StaticListNode *next;
  unsigned char data[];
};
/*----------------------------------------------------------------------------*/
struct StaticList
{
  void *data;

  /** First element of the list containing data nodes. */
  struct StaticListNode *first;
  /** First element of the free nodes list. */
  struct StaticListNode *pool;
  /** Size of each element in bytes. */
  unsigned int width;
};
/*----------------------------------------------------------------------------*/
enum result staticListInit(struct StaticList *, unsigned int, unsigned int);
void staticListDeinit(struct StaticList *);
void staticListClear(struct StaticList *);
void *staticListErase(struct StaticList *, void *);
enum result staticListPush(struct StaticList *, const void *);

unsigned int staticListCapacity(const struct StaticList *);
unsigned int staticListSize(const struct StaticList *);
/*----------------------------------------------------------------------------*/
static inline void staticListData(struct StaticList *list, const void *node,
    void *element)
{
  memcpy(element, ((const struct StaticListNode *)node)->data, list->width);
}
/*----------------------------------------------------------------------------*/
static inline void *staticListFirst(const struct StaticList *list)
{
  return list->first;
}
/*----------------------------------------------------------------------------*/
static inline void *staticListNext(const void *node)
{
  return ((const struct StaticListNode *)node)->next;
}
/*----------------------------------------------------------------------------*/
static inline bool staticListEmpty(const struct StaticList *list)
{
  return list->first == 0;
}
/*----------------------------------------------------------------------------*/
static inline bool staticListFull(const struct StaticList *list)
{
  return list->pool == 0;
}
/*----------------------------------------------------------------------------*/
#endif /* CONTAINERS_STATIC_LIST_H_ */
