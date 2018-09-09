/*
 * xcore/containers/tg_list.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_CONTAINERS_TG_LIST_H_
#define XCORE_CONTAINERS_TG_LIST_H_
/*----------------------------------------------------------------------------*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <xcore/helpers.h>
/*----------------------------------------------------------------------------*/
struct TgListNode;
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

size_t tgListCountNodes(const struct TgListNode *);
struct TgListNode *tgListErase(struct TgListNode **, struct TgListNode *);
struct TgListNode *tgListFindIf(struct TgListNode *, const void *,
    int (*)(const void *, const void *));
void tgListFreeChain(struct TgListNode *);

END_DECLS
/*----------------------------------------------------------------------------*/
#define DEFINE_LIST(type, name, prefix) \
    typedef struct \
    { \
      struct TgListNode *next; \
      type data; \
    } name##ListNode; \
    \
    typedef struct \
    { \
      struct TgListNode *head; \
    } name##List; \
    \
    static inline int prefix##ListGenericComparator(const void *a, \
        const void *b) \
    { \
      return memcmp(a, b, sizeof(type)); \
    } \
    \
    static inline void prefix##ListClear(name##List *list) \
    { \
      if (list->head) \
      { \
        tgListFreeChain(list->head); \
        list->head = 0; \
      } \
    } \
    \
    static inline type *prefix##ListData(name##ListNode *node) \
    { \
      return &node->data; \
    } \
    \
    static inline bool prefix##ListEmpty(const name##List *list) \
    { \
      return list->head == 0; \
    } \
    \
    static inline name##ListNode *prefix##ListErase(name##List *list, \
        name##ListNode *node) \
    { \
      return (name##ListNode *)tgListErase(&list->head, \
          (struct TgListNode *)node); \
    } \
    \
    static inline name##ListNode *prefix##ListFind(name##List *list, \
        const type element) \
    { \
      return (name##ListNode *)tgListFindIf(list->head, &element, \
          prefix##ListGenericComparator); \
    } \
    \
    static inline name##ListNode *prefix##ListFindIf(name##List *list, \
        const type element, int (*comparator)(const void *, const void *)) \
    { \
      return (name##ListNode *)tgListFindIf(list->head, &element, comparator); \
    } \
    \
    static inline name##ListNode *prefix##ListFront(name##List *list) \
    { \
      return (name##ListNode *)list->head; \
    } \
    \
    static inline bool prefix##ListInsert(name##List *list, \
        name##ListNode *previous, type element) \
    { \
      name##ListNode * const node = malloc(sizeof(name##ListNode)); \
      \
      if (node) \
      { \
        node->data = element; \
        \
        if (!previous) \
        { \
          node->next = list->head; \
          list->head = (struct TgListNode *)node; \
        } \
        else \
        { \
          node->next = previous->next; \
          previous->next = (struct TgListNode *)node; \
        } \
        \
        return true; \
      } \
      else \
        return false; \
    } \
    \
    static inline name##ListNode *prefix##ListNext(name##ListNode *node) \
    { \
      return (name##ListNode *)node->next; \
    } \
    \
    static inline bool prefix##ListPushFront(name##List *list, type element) \
    { \
      name##ListNode * const node = malloc(sizeof(name##ListNode)); \
      \
      if (node) \
      { \
        node->data = element; \
        node->next = list->head; \
        list->head = (struct TgListNode *)node; \
        \
        return true; \
      } \
      else \
        return false; \
    } \
    \
    static inline size_t prefix##ListSize(const name##List *list) \
    { \
      return tgListCountNodes(list->head); \
    } \
    \
    static inline void prefix##ListInit(name##List *list) \
    { \
      list->head = 0; \
    } \
    \
    static inline void prefix##ListDeinit(name##List *list) \
    { \
      tgListFreeChain(list->head); \
    }
/*----------------------------------------------------------------------------*/
#endif /* XCORE_CONTAINERS_TG_LIST_H_ */
