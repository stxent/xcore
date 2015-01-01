/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <containers/list.h>
//------------------------------------------------------------------------------
#define MAX_CAPACITY 16
//------------------------------------------------------------------------------
struct DummyStruct
{
  int64_t a;
  int32_t b;
  int16_t c[6];
};
//------------------------------------------------------------------------------
void performListTest(void)
{
  struct List list;
  struct ListNode *node;
  unsigned int index;
  enum result res;

  /* List initialization */
  res = listInit(&list, sizeof(struct DummyStruct));
  assert(res == E_OK);
  assert(listCapacity(&list) == 0); /* Dynamic allocation */
  assert(listSize(&list) == 0);
  assert(listEmpty(&list) == true);

  /* List filling */
  for (index = 0; index < MAX_CAPACITY; ++index)
  {
    struct DummyStruct object = {
        .a = (int64_t)index * 8,
        .b = (int32_t)index * 4,
        .c = {(int16_t)index}
    };

    res = listPush(&list, &object);
    assert(res == E_OK);
  }

  assert(listCapacity(&list) == MAX_CAPACITY);
  assert(listSize(&list) == MAX_CAPACITY);
  assert(listEmpty(&list) == false);

  /* Iteration through elements */
  index = 0;
  node = listFirst(&list);

  while (node)
  {
    struct DummyStruct object;

    listData(&list, node, &object);

    assert(object.a == (int64_t)index * 8);
    assert(object.b == (int32_t)index * 4);
    assert(object.c[0] == (int16_t)index);

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

  assert(listCapacity(&list) == MAX_CAPACITY);
  assert(listSize(&list) == MAX_CAPACITY / 2);
  assert(listEmpty(&list) == false);

  /* Erasure of all elements */
  listClear(&list);

  assert(listCapacity(&list) == MAX_CAPACITY);
  assert(listSize(&list) == 0);
  assert(listEmpty(&list) == true);

  listDeinit(&list);
}
//------------------------------------------------------------------------------
int main(void)
{
  performListTest();

  printf("Passed\n");

  return 0;
}
