/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <xcore/containers/byte_queue.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 50
/*----------------------------------------------------------------------------*/
static void performQueueTest(void)
{
  const uint8_t *example = (const uint8_t *)
      "Lorem ipsum dolor sit amet, consectetur adipiscing elit, "
      "sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.";
  uint8_t buffer[MAX_CAPACITY];
  struct ByteQueue queue;
  unsigned int count;
  enum Result res;
  bool state;

#ifdef NDEBUG
  (void)count;
  (void)res;
  (void)state;
#endif

  /* Check validity of the prerequisites */
  assert(strlen((const char *)example) > MAX_CAPACITY);

  /* Queue initialization */
  res = byteQueueInit(&queue, MAX_CAPACITY);
  assert(res == E_OK);

  /* Check initial state */
  count = byteQueueCapacity(&queue);
  assert(count == MAX_CAPACITY);
  count = byteQueueSize(&queue);
  assert(count == 0);
  state = byteQueueEmpty(&queue);
  assert(state == true);
  state = byteQueueFull(&queue);
  assert(state == false);

  /* Queue filling */
  count = byteQueuePushArray(&queue, example, 1);
  assert(count == 1);
  count = byteQueuePushArray(&queue, example, MAX_CAPACITY / 2 - 1);
  assert(count == MAX_CAPACITY / 2 - 1);
  count = byteQueuePopArray(&queue, buffer, MAX_CAPACITY);
  assert(count == MAX_CAPACITY / 2);

  count = byteQueuePushArray(&queue, example, MAX_CAPACITY);
  assert(count == MAX_CAPACITY);
  count = byteQueuePopArray(&queue, buffer, MAX_CAPACITY);
  assert(count == MAX_CAPACITY);

  count = byteQueuePushArray(&queue, example, 0);
  assert(count == 0);
  count = byteQueuePopArray(&queue, buffer, 0);
  assert(count == 0);

  /* Check overflows */
  byteQueueClear(&queue);
  /* Offset at 1/4 of the queue */
  count = byteQueuePushArray(&queue, example, MAX_CAPACITY / 4);
  assert(count == MAX_CAPACITY / 4);
  count = byteQueuePopArray(&queue, buffer, MAX_CAPACITY);
  assert(count == MAX_CAPACITY / 4);
  /* Offset at 3/4 of the queue */
  count = byteQueuePushArray(&queue, example, MAX_CAPACITY / 2);
  assert(count == MAX_CAPACITY / 2);
  /* Offset at 1/4 of the queue */
  count = byteQueuePushArray(&queue, example, MAX_CAPACITY);
  assert(count == MAX_CAPACITY / 2);
  /* Try to overflow */
  count = byteQueuePushArray(&queue, example, MAX_CAPACITY);
  assert(count == 0);
  /* Pop all elements */
  count = byteQueuePopArray(&queue, buffer, MAX_CAPACITY / 2);
  assert(count == MAX_CAPACITY / 2);
  count = byteQueuePopArray(&queue, buffer, MAX_CAPACITY);
  assert(count == MAX_CAPACITY / 2);

  /* Check resulting state */
  count = byteQueueCapacity(&queue);
  assert(count == MAX_CAPACITY);
  count = byteQueueSize(&queue);
  assert(count == 0);
  state = byteQueueEmpty(&queue);
  assert(state == true);
  state = byteQueueFull(&queue);
  assert(state == false);

  byteQueueDeinit(&queue);
}
/*----------------------------------------------------------------------------*/
int main(void)
{
  performQueueTest();

  printf("Passed\n");

  return 0;
}
