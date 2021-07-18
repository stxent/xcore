/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/byte_queue.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 51
/*----------------------------------------------------------------------------*/
extern void *__libc_malloc(size_t);

static bool mallocHookActive = false;

void *malloc(size_t size)
{
  if (!mallocHookActive)
    return __libc_malloc(size);
  else
    return 0;
}
/*----------------------------------------------------------------------------*/
static void checkElements(const uint8_t *buffer, size_t base, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    ck_assert_uint_eq(buffer[i], (uint8_t)(base + i));
}
/*----------------------------------------------------------------------------*/
static void fillTestBuffer(uint8_t *buffer, size_t base, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    buffer[i] = (uint8_t)(base + i);
}
/*----------------------------------------------------------------------------*/
START_TEST(testEmptyContainer)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, 0);
  ck_assert(result == true);

  ck_assert_uint_eq(byteQueueCapacity(&queue), 0);
  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  ck_assert(byteQueueEmpty(&queue) == true);
  ck_assert(byteQueueFull(&queue) == true);

  byteQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMultiByteInterface)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  uint8_t buffer[MAX_CAPACITY];
  size_t count;

  /*
   * Try to read from empty queue.
   */

  count = byteQueuePopArray(&queue, buffer, sizeof(buffer));
  ck_assert_uint_eq(count, 0);

  /*
   * Try to write to full queue.
   */

  fillTestBuffer(buffer, 0, sizeof(buffer));
  count = byteQueuePushArray(&queue, buffer, sizeof(buffer));
  ck_assert_uint_eq(count, sizeof(buffer));
  count = byteQueuePushArray(&queue, buffer, sizeof(buffer));
  ck_assert_uint_eq(count, 0);

  /*
   * Pop and check elements from the full queue.
   */

  count = byteQueuePopArray(&queue, buffer, sizeof(buffer));
  ck_assert_uint_eq(count, sizeof(buffer));
  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  checkElements(buffer, 0, count);

  /*
   * Simulate situation where head pointer will be lower than tail pointer.
   */

  enum
  {
    HALF  = sizeof(buffer) / 2,
    SIXTH = sizeof(buffer) / 6
  };

  fillTestBuffer(buffer, 0, HALF);
  count = byteQueuePushArray(&queue, buffer, HALF);
  ck_assert_uint_eq(count, HALF);
  /* State: [######------] */

  count = byteQueuePopArray(&queue, buffer, SIXTH);
  ck_assert_uint_eq(count, SIXTH);
  checkElements(buffer, 0, count);
  /* State: [--####------] */

  fillTestBuffer(buffer, HALF, HALF);
  count = byteQueuePushArray(&queue, buffer, HALF);
  ck_assert_uint_eq(count, HALF);
  /* State: [--##########] */

  count = byteQueuePopArray(&queue, buffer, HALF);
  ck_assert_uint_eq(count, HALF);
  checkElements(buffer, SIXTH, count);
  /* State: [--------####] */

  fillTestBuffer(buffer, 2 * HALF, SIXTH);
  count = byteQueuePushArray(&queue, buffer, SIXTH);
  ck_assert_uint_eq(count, SIXTH);
  /* State: [##------####] */

  fillTestBuffer(buffer, 2 * HALF + SIXTH, SIXTH);
  count = byteQueuePushArray(&queue, buffer, SIXTH);
  ck_assert_uint_eq(count, SIXTH);
  /* State: [####----####] */

  count = byteQueuePopArray(&queue, buffer, SIXTH);
  ck_assert_uint_eq(count, SIXTH);
  checkElements(buffer, SIXTH + HALF, count);
  /* State: [####------##]*/

  count = byteQueuePopArray(&queue, buffer, sizeof(buffer));
  ck_assert_uint_eq(count, HALF);
  checkElements(buffer, 2 * SIXTH + HALF, count);
  /* State: [------------] */

  byteQueueClear(&queue);

  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  byteQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopArenaSequence)
{
  struct ByteQueue queue;
  uint8_t arena[MAX_CAPACITY];

  /* Queue initialization */
  byteQueueInitArena(&queue, MAX_CAPACITY, arena);

  for (size_t i = 0; i < 7; ++i)
  {
    for (size_t j = 0; j < MAX_CAPACITY / 3; ++j)
    {
      const uint8_t id = (uint8_t)(i * MAX_CAPACITY / 3 + j);

      byteQueuePushBack(&queue, id);
      ck_assert_uint_eq(byteQueueFront(&queue), id);
      byteQueuePopFront(&queue);
    }
  }

  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  byteQueueDeinitArena(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopSequence)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  for (size_t i = 0; i < 7; ++i)
  {
    for (size_t j = 0; j < MAX_CAPACITY / 3; ++j)
    {
      const uint8_t id = (uint8_t)(i * MAX_CAPACITY / 3 + j);

      byteQueuePushBack(&queue, id);
      ck_assert_uint_eq(byteQueueFront(&queue), id);
      byteQueuePopFront(&queue);
    }
  }

  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  byteQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testSingleByteInterface)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  /* Check initial state */
  ck_assert_uint_eq(byteQueueCapacity(&queue), MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  ck_assert(byteQueueEmpty(&queue) == true);
  ck_assert(byteQueueFull(&queue) == false);

  /* Fill queue completely */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
    byteQueuePushBack(&queue, (uint8_t)i);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY);

  /* Clear queue */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
  {
    ck_assert_uint_eq(byteQueueFront(&queue), (uint8_t)i);
    byteQueuePopFront(&queue);
  }
  ck_assert_uint_eq(byteQueueSize(&queue), 0);

  /* Fill half of the queue and clear */
  for (size_t i = 0; i < MAX_CAPACITY / 2; ++i)
    byteQueuePushBack(&queue, (uint8_t)i);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY / 2);
  ck_assert(byteQueueEmpty(&queue) == false);
  ck_assert(byteQueueFull(&queue) == false);

  byteQueueClear(&queue);

  /* Check resulting state */
  ck_assert_uint_eq(byteQueueCapacity(&queue), MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  ck_assert(byteQueueEmpty(&queue) == true);
  ck_assert(byteQueueFull(&queue) == false);

  byteQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  struct ByteQueue queue;

  mallocHookActive = true;
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  mallocHookActive = false;

  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("ByteQueue");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testEmptyContainer);
  tcase_add_test(testcase, testSingleByteInterface);
  tcase_add_test(testcase, testPushPopArenaSequence);
  tcase_add_test(testcase, testPushPopSequence);
  tcase_add_test(testcase, testMultiByteInterface);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
