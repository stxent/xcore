/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <byte_queue.h>
//------------------------------------------------------------------------------
#define MAX_CAPACITY 16
//------------------------------------------------------------------------------
void performQueueTest(void)
{
  const uint8_t *example = (const uint8_t *)"Extremely long string";
  uint8_t buffer[MAX_CAPACITY];
  struct ByteQueue queue;
  unsigned int count;
  enum result res;

  /* Check validity of the prerequisites */
  assert(strlen((const char *)example) > MAX_CAPACITY);

  /* Queue initialization */
  res = byteQueueInit(&queue, MAX_CAPACITY);
  assert(res == E_OK);
  assert(byteQueueCapacity(&queue) == MAX_CAPACITY);
  assert(byteQueueSize(&queue) == 0);
  assert(byteQueueEmpty(&queue) == true);

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

  assert(byteQueueCapacity(&queue) == MAX_CAPACITY);
  assert(byteQueueSize(&queue) == 0);
  assert(byteQueueEmpty(&queue) == true);

  byteQueueDeinit(&queue);
}
//------------------------------------------------------------------------------
int main(void)
{
  performQueueTest();

  printf("Passed\n");

  return 0;
}
