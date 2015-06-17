/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <containers/queue.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 16
/*----------------------------------------------------------------------------*/
struct DummyStruct
{
  int64_t a;
  int32_t b;
  int16_t c[6];
};
/*----------------------------------------------------------------------------*/
static void performQueueTest(void)
{
  struct Queue queue;

#ifndef NDEBUG
  enum result res;
#else
  enum result res __attribute__((unused));
#endif

  /* Queue initialization */
  res = queueInit(&queue, sizeof(struct DummyStruct), MAX_CAPACITY);
  assert(res == E_OK);
  assert(queueCapacity(&queue) == MAX_CAPACITY);
  assert(queueSize(&queue) == 0);
  assert(queueEmpty(&queue) == true);
  assert(queueFull(&queue) == false);

  /* Queue filling */
  int index = 0;

  while (queueSize(&queue) != queueCapacity(&queue))
  {
    const struct DummyStruct element = {
        .a = index * 100,
        .b = -index * 200,
        .c = {index}
    };

    queuePush(&queue, &element);
    ++index;
  }

  assert(queueSize(&queue) == MAX_CAPACITY);
  assert(queueEmpty(&queue) == false);
  assert(queueFull(&queue) == true);

  index = 0;
  while (!queueEmpty(&queue))
  {
    struct DummyStruct element;

    queuePop(&queue, &element);
    assert(element.a == index * 100);
    assert(element.b == -index * 200);
    assert(element.c[0] == index);
    ++index;
  }

  assert(queueSize(&queue) == 0);
  assert(queueEmpty(&queue) == true);
  assert(queueFull(&queue) == false);

  queueDeinit(&queue);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performQueueTest();

  printf("Passed\n");

  return 0;
}
