/*
 * xcore/containers/tg_list.h
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_CONTAINERS_TG_LIST_H_
#define XCORE_CONTAINERS_TG_LIST_H_
/*----------------------------------------------------------------------------*/
#include <xcore/helpers.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
/*----------------------------------------------------------------------------*/
struct TgListNode;
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

void tgListAppend(struct TgListNode **, struct TgListNode *);
size_t tgListCountNodes(const struct TgListNode *);
struct TgListNode *tgListEraseNode(struct TgListNode **, struct TgListNode *);
void tgListEraseIf(struct TgListNode **, void *,
    bool (*)(const void *, void *));
struct TgListNode *tgListFindIf(struct TgListNode *, void *,
    bool (*)(const void *, void *));
void tgListFreeChain(struct TgListNode *);
struct TgListNode *tgListGetTail(struct TgListNode *);

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
    static inline bool prefix##ListDefaultComparator(const void *a, void *b) \
    { \
      return memcmp(a, b, sizeof(type)) == 0; \
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
    static inline void prefix##ListErase(name##List *list, type element) \
    { \
      tgListEraseIf(&list->head, &element, prefix##ListDefaultComparator); \
    } \
    \
    static inline void prefix##ListEraseIf(name##List *list, \
        void *argument, bool (*predicate)(const void *, void *)) \
    { \
      tgListEraseIf(&list->head, argument, predicate); \
    } \
    \
    static inline name##ListNode *prefix##ListEraseNode(name##List *list, \
        name##ListNode *node) \
    { \
      return (name##ListNode *)tgListEraseNode(&list->head, \
          (struct TgListNode *)node); \
    } \
    \
    static inline name##ListNode *prefix##ListFind(name##List *list, \
        type element) \
    { \
      return (name##ListNode *)tgListFindIf(list->head, &element, \
          prefix##ListDefaultComparator); \
    } \
    \
    static inline name##ListNode *prefix##ListFindIf(name##List *list, \
        void *argument, bool (*predicate)(const void *, void *)) \
    { \
      return (name##ListNode *)tgListFindIf(list->head, argument, predicate); \
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
    static inline bool prefix##ListPushBack(name##List *list, type element) \
    { \
      name##ListNode * const node = malloc(sizeof(name##ListNode)); \
      \
      if (node) \
      { \
        node->data = element; \
        node->next = 0; \
        \
        tgListAppend(&list->head, (struct TgListNode *)node); \
        return true; \
      } \
      else \
        return false; \
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
