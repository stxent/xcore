/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xcore/containers/queue.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while (0)
#endif
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
static void testIteration(struct Queue *, size_t, bool);
static void performQueueTest(void);
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
      .c = {index * 4, -index * 4, index * 5, -index * 5, index * 6, -index * 6}
  };

  return element;
}
/*----------------------------------------------------------------------------*/
static void testIteration(struct Queue *queue, size_t number, bool clear)
{
  assert(queueSize(queue) == 0);
  assert(queueEmpty(queue) == true);
  assert(queueFull(queue) == false);

  for (size_t index = 0; index < number; ++index)
  {
    const struct DummyStruct element = createElement(index);

    queuePush(queue, &element);
  }

  assert(queueSize(queue) == number);
  if (number)
    assert(queueEmpty(queue) == false);
  if (number == queueCapacity(queue))
    assert(queueFull(queue) == true);

  if (clear)
  {
    for (size_t index = 0; index < number; ++index)
    {
      const struct DummyStruct referenceElement = createElement(index);
      struct DummyStruct element;
      bool result;

      queuePeek(queue, &element);
      result = compareElements(&element, &referenceElement);
      assert(result == true);

      queuePop(queue, &element);
      result = compareElements(&element, &referenceElement);
      assert(result == true);

#ifdef NDEBUG
      (void)result;
#endif
    }

    assert(queueSize(queue) == 0);
    assert(queueEmpty(queue) == true);
    assert(queueFull(queue) == false);
  }
}
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

  /* Fill queue */
  for (size_t iteration = 1; iteration < MAX_CAPACITY * 2; ++iteration)
  {
    const size_t identifier = -abs(iteration - MAX_CAPACITY) + MAX_CAPACITY;

    DEBUG_PRINT("Iteration %zu, number %zu\n", iteration, identifier);
    testIteration(&queue, identifier, true);
  }

  /* Clear */
  testIteration(&queue, MAX_CAPACITY, false);
  queueClear(&queue);
  assert(queueSize(&queue) == 0);

  /* Pop with zero pointer as an argument */
  testIteration(&queue, 1, false);
  queuePop(&queue, 0);
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
