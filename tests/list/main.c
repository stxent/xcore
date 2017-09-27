/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <xcore/containers/list.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 17
/*----------------------------------------------------------------------------*/
struct DummyStruct
{
  int64_t a;
  int32_t b;
  int16_t c[6];
};
/*----------------------------------------------------------------------------*/
static bool compareElements(const struct DummyStruct *,
    const struct DummyStruct *);
struct DummyStruct createElement(size_t);
static int customCompare(const void *, const void *);
static void performListTest(void);
/*----------------------------------------------------------------------------*/
static bool compareElements(const struct DummyStruct *a,
    const struct DummyStruct *b)
{
  if (a->a != b->a)
    return false;
  else if (a->b != b->b)
    return false;
  else if (memcmp(a->c, b->c, sizeof(a->c)))
    return false;
  else
    return true;
}
/*----------------------------------------------------------------------------*/
struct DummyStruct createElement(size_t index)
{
  const struct DummyStruct element = {
      .a = index * 2,
      .b = -index * 3,
      .c = {-index, index + 1, -index + 2, index + 3, -index + 4, index + 5}
  };

  return element;
}
/*----------------------------------------------------------------------------*/
static int customCompare(const void *a, const void *b)
{
  const struct DummyStruct * const aValue = a;
  const struct DummyStruct * const bValue = b;

  return aValue->a - bValue->a;
}
/*----------------------------------------------------------------------------*/
static void performListTest(void)
{
  struct List list;
  struct ListNode *node;
  size_t index;

#ifndef NDEBUG
  enum Result res;
#else
  enum Result res __attribute__((unused));
#endif

  /* List initialization */
  res = listInit(&list, sizeof(struct DummyStruct));
  assert(res == E_OK);
  assert(listCapacity(&list) == 0); /* Dynamic allocation */
  assert(listSize(&list) == 0);
  assert(listEmpty(&list) == true);

  /* List filling */
  for (index = 0; index < MAX_CAPACITY; ++index)
  {
    const struct DummyStruct element = createElement(index);

    res = listPush(&list, &element);
    assert(res == E_OK);
  }

  assert(listCapacity(&list) == MAX_CAPACITY);
  assert(listSize(&list) == MAX_CAPACITY);
  assert(listEmpty(&list) == false);

  /* Find elements */
  const struct DummyStruct firstElement = createElement(0);
  const struct DummyStruct centralElement = createElement(MAX_CAPACITY / 2);
  const struct DummyStruct lastElement = createElement(MAX_CAPACITY - 1);

  struct DummyStruct unknownElement = centralElement;
  struct ListNode *foundNode;

  unknownElement.a = -1;

  foundNode = listFind(&list, &unknownElement);
  assert(foundNode == 0);
  foundNode = listFind(&list, &firstElement);
  assert(foundNode != 0);
  foundNode = listFind(&list, &centralElement);
  assert(foundNode != 0);
  foundNode = listFind(&list, &lastElement);
  assert(foundNode != 0);

#ifdef NDEBUG
  (void)foundNode;
#endif

  /* Find elements using custom comparator */
  const struct DummyStruct customElement = {
      .a = centralElement.a,
      .b = 0,
      .c = {0}
  };

  foundNode = listFindCompared(&list, &unknownElement, customCompare);
  assert(foundNode == 0);
  foundNode = listFindCompared(&list, &customElement, customCompare);
  assert(foundNode != 0);

  /* Iterating through elements */
  index = 0;
  node = listFirst(&list);

  while (node)
  {
    const struct DummyStruct referenceElement = createElement(index);
    struct DummyStruct element;
    bool result;

    listData(&list, node, &element);
    result = compareElements(&element, &referenceElement);
    assert(result == true);

#ifdef NDEBUG
    (void)result;
#endif

    ++index;
    node = listNext(node);
  }

  /* Erasing */
  index = 0;
  node = listFirst(&list);

  while (node)
  {
    if (++index % 2 == 0)
    {
      node = listErase(&list, node);
      continue;
    }

    node = listNext(node);
  }

  assert(listCapacity(&list) == MAX_CAPACITY - MAX_CAPACITY / 2);
  assert(listSize(&list) == MAX_CAPACITY - MAX_CAPACITY / 2);
  assert(listEmpty(&list) == false);

  /* Erasure of all elements */
  listClear(&list);

  assert(listCapacity(&list) == 0);
  assert(listSize(&list) == 0);
  assert(listEmpty(&list) == true);

  listDeinit(&list);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performListTest();

  printf("Passed\n");

  return 0;
}
