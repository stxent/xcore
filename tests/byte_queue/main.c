/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/byte_queue.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define DATA_ALIGNMENT  32
#define MAX_CAPACITY    51
/*----------------------------------------------------------------------------*/
extern void *__libc_malloc(size_t);
extern void *__libc_memalign(size_t, size_t);

static bool mallocHookActive = false;

void *malloc(size_t size)
{
  if (!mallocHookActive)
    return __libc_malloc(size);
  else
    return NULL;
}

void *memalign(size_t alignment, size_t bytes)
{
  if (!mallocHookActive)
    return __libc_memalign(alignment, bytes);
  else
    return NULL;
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
static void deferredPopAndCheck(struct ByteQueue *queue, size_t base,
    size_t count)
{
  const size_t size = byteQueueSize(queue);
  const uint8_t *position;
  size_t pending;

  ck_assert_uint_ge(size, count);
  byteQueueDeferredPop(queue, &position, &pending, 0);

  if (count > pending)
  {
    const size_t offset = pending;

    /* Pop and check first part of the queue */
    checkElements(position, base, offset);

    /* Pop and check second part of the queue */
    byteQueueDeferredPop(queue, &position, &pending, offset);
    ck_assert_uint_ge(pending, count - offset);
    checkElements(position, base + offset, count - offset);
  }
  else
  {
    checkElements(position, base, count);
  }

  byteQueueAbandon(queue, count);
  ck_assert_uint_eq(byteQueueSize(queue), size - count);
}
/*----------------------------------------------------------------------------*/
static void deferredPushAndCheck(struct ByteQueue *queue, size_t base,
    size_t count)
{
  const size_t size = byteQueueSize(queue);
  size_t available;
  uint8_t *position;

  ck_assert_uint_ge(byteQueueCapacity(queue) - size, count);
  byteQueueDeferredPush(queue, &position, &available, 0);

  if (count > available)
  {
    const size_t offset = available;

    /* Initialize first part of free space */
    fillTestBuffer(position, base, offset);

    /* Initialize second part of free space */
    byteQueueDeferredPush(queue, &position, &available, offset);
    ck_assert_uint_ge(available, count - offset);
    fillTestBuffer(position, base + offset, count - offset);
  }
  else
  {
    fillTestBuffer(position, base, count);
  }

  byteQueueAdvance(queue, count);
  ck_assert_uint_eq(byteQueueSize(queue), size + count);
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
START_TEST(testDeferredInterface)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  const uint8_t *input;
  uint8_t *output;
  size_t count;

  /*
   * Try to read from empty queue.
   */

  byteQueueDeferredPop(&queue, &input, &count, 0);
  ck_assert_ptr_nonnull(input);
  ck_assert_uint_eq(count, 0);

  /*
   * Try to write to full queue.
   */

  byteQueueDeferredPush(&queue, &output, &count, 0);
  ck_assert_ptr_nonnull(output);
  ck_assert_uint_eq(count, MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  ck_assert(byteQueueEmpty(&queue) == true);

  fillTestBuffer(output, 0, MAX_CAPACITY);
  byteQueueAdvance(&queue, MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY);
  ck_assert(byteQueueFull(&queue) == true);

  byteQueueDeferredPush(&queue, &output, &count, 0);
  ck_assert_ptr_nonnull(output);
  ck_assert_uint_eq(count, 0);

  /*
   * Pop and check elements from the full queue.
   */

  byteQueueDeferredPop(&queue, &input, &count, 0);
  ck_assert_ptr_nonnull(input);
  ck_assert_uint_eq(count, MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY);
  ck_assert(byteQueueFull(&queue) == true);

  checkElements(input, 0, MAX_CAPACITY);
  byteQueueAbandon(&queue, MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  ck_assert(byteQueueEmpty(&queue) == true);

  /*
   * Write to the queue in two steps.
   */

  byteQueueDeferredPush(&queue, &output, &count, 0);
  ck_assert_uint_eq(count, MAX_CAPACITY);
  fillTestBuffer(output, 0, MAX_CAPACITY / 2);

  byteQueueDeferredPush(&queue, &output, &count, MAX_CAPACITY / 2);
  ck_assert_uint_eq(count, MAX_CAPACITY - MAX_CAPACITY / 2);
  fillTestBuffer(output, MAX_CAPACITY / 2, MAX_CAPACITY - MAX_CAPACITY / 2);

  byteQueueAdvance(&queue, MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY);

  /*
   * Pop and check elements from the queue in two steps.
   */

  byteQueueDeferredPop(&queue, &input, &count, 0);
  ck_assert_uint_eq(count, MAX_CAPACITY);
  checkElements(input, 0, MAX_CAPACITY);

  byteQueueDeferredPop(&queue, &input, &count, MAX_CAPACITY / 2);
  ck_assert_uint_eq(count, MAX_CAPACITY - MAX_CAPACITY / 2);
  checkElements(input, MAX_CAPACITY / 2, MAX_CAPACITY - MAX_CAPACITY / 2);

  byteQueueAbandon(&queue, MAX_CAPACITY);
  ck_assert_uint_eq(byteQueueSize(&queue), 0);

  /*
   * Simulate situation where head pointer will be lower than tail pointer.
   */

  enum
  {
    HALF  = MAX_CAPACITY / 2,
    SIXTH = MAX_CAPACITY / 6
  };

  deferredPushAndCheck(&queue, 0, HALF);
  /* State: [######------] */

  deferredPopAndCheck(&queue, 0, SIXTH);
  /* State: [--####------] */

  deferredPushAndCheck(&queue, HALF, HALF);
  /* State: [--##########] */

  deferredPopAndCheck(&queue, SIXTH, HALF);
  /* State: [--------####] */

  deferredPushAndCheck(&queue, 2 * HALF, SIXTH);
  /* State: [##------####] */

  deferredPushAndCheck(&queue, 2 * HALF + SIXTH, SIXTH);
  /* State: [####----####] */

  deferredPopAndCheck(&queue, SIXTH + HALF, SIXTH);
  /* State: [####------##]*/

  deferredPopAndCheck(&queue, 2 * SIXTH + HALF, HALF);
  /* State: [------------] */

  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  byteQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testDeferredPushPopSequence)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  for (size_t i = 0; i < 7; ++i)
  {
    const uint8_t id = (uint8_t)(i * MAX_CAPACITY / 3);

    /* Push and pop one-third of the queue */
    deferredPushAndCheck(&queue, id, MAX_CAPACITY / 3);
    ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY / 3);
    deferredPopAndCheck(&queue, id, MAX_CAPACITY / 3);
    ck_assert_uint_eq(byteQueueSize(&queue), 0);

    /* Push and pop full queue */

    deferredPushAndCheck(&queue, id, MAX_CAPACITY);
    ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY);
    deferredPopAndCheck(&queue, id, MAX_CAPACITY);
    ck_assert_uint_eq(byteQueueSize(&queue), 0);
  }

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

  ck_assert_uint_eq(byteQueueSize(&queue), 0);
  byteQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopAlignedSequence)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInitAligned(&queue, MAX_CAPACITY,
      DATA_ALIGNMENT);
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
START_TEST(testRandomAccess)
{
  struct ByteQueue queue;

  /* Queue initialization */
  const bool result = byteQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  /* Fill and clear half of the queue to shift head and tail pointers */
  for (size_t i = 0; i < MAX_CAPACITY / 2; ++i)
    byteQueuePushBack(&queue, (uint8_t)i);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY / 2);

  while (!byteQueueEmpty(&queue))
    byteQueuePopFront(&queue);
  ck_assert(byteQueueEmpty(&queue) == true);

  /* Fill the queue up to the limit */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
    byteQueuePushBack(&queue, (uint8_t)i);
  ck_assert(byteQueueFull(&queue) == true);

  /* Check each element of the queue using random access function */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
  {
    const uint8_t value = byteQueueAt(&queue, i);
    ck_assert_uint_eq(value, (uint8_t)i);
  }

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

  /* Fill the queue up to the limit */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
    byteQueuePushBack(&queue, (uint8_t)i);
  ck_assert_uint_eq(byteQueueSize(&queue), MAX_CAPACITY);

  /* Clear the queue */
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

  /* Reset the queue */
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
  bool result;

  mallocHookActive = true;
  result = byteQueueInit(&queue, MAX_CAPACITY);
  mallocHookActive = false;

  ck_assert(result == false);

  mallocHookActive = true;
  result = byteQueueInitAligned(&queue, MAX_CAPACITY, DATA_ALIGNMENT);
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
  tcase_add_test(testcase, testDeferredInterface);
  tcase_add_test(testcase, testDeferredPushPopSequence);
  tcase_add_test(testcase, testMultiByteInterface);
  tcase_add_test(testcase, testPushPopAlignedSequence);
  tcase_add_test(testcase, testPushPopArenaSequence);
  tcase_add_test(testcase, testPushPopSequence);
  tcase_add_test(testcase, testRandomAccess);
  tcase_add_test(testcase, testSingleByteInterface);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
