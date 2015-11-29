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
  enum result res;

#ifdef NDEBUG
  (void)res;
#endif

  /* Queue initialization */
  res = queueInit(&queue, sizeof(struct DummyStruct), MAX_CAPACITY);
  assert(res == E_OK);
  assert(queueCapacity(&queue) == MAX_CAPACITY);
  assert(queueSize(&queue) == 0);
  assert(queueEmpty(&queue) == true);
  assert(queueFull(&queue) == false);

  /* Fill queue */
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

  /* Clear queue and check content */
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

  /* Push single element */
  const struct DummyStruct singleton = {
      .a = 17,
      .b = 23,
      .c = {29}
  };
  struct DummyStruct singletonCheck;

  queuePush(&queue, &singleton);
  queuePeek(&queue, &singletonCheck);
  assert(singletonCheck.a == singleton.a);
  assert(singletonCheck.b == singleton.b);
  assert(singletonCheck.c[0] == singleton.c[0]);
  queuePop(&queue, 0);
  assert(queueSize(&queue) == 0);
  assert(queueEmpty(&queue) == true);

  /* Check clearing */
  queuePush(&queue, &singleton);
  assert(queueSize(&queue) == 1);
  queueClear(&queue);
  assert(queueSize(&queue) == 0);

  queueDeinit(&queue);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performQueueTest();

  printf("Passed\n");

  return 0;
}
