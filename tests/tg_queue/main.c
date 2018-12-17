/*
 * main.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <check.h>
#include <xcore/containers/tg_queue.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 17
/*----------------------------------------------------------------------------*/
typedef struct
{
  int64_t a;
  int32_t b;
  int8_t c;
} __attribute__((packed)) TestStruct;

DEFINE_QUEUE(TestStruct, Test, test)
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
static bool compareElements(const TestStruct *a, const TestStruct *b)
{
  return memcmp(a, b, sizeof(TestStruct)) == 0;
}
/*----------------------------------------------------------------------------*/
static TestStruct createElement(size_t index)
{
  return (TestStruct){
      (int64_t)((index & 1) ? index : -index),
      (int32_t)((index & 1) ? -index : index),
      (int8_t)index
  };
}
/*----------------------------------------------------------------------------*/
static void runPushPop(TestQueue *queue, size_t count)
{
  /* Fill the queue with push back function */
  for (size_t i = 0; i < count; ++i)
    testQueuePushBack(queue, createElement(i));

  ck_assert_uint_eq(testQueueSize(queue), count);
  ck_assert(testQueueEmpty(queue) == (count == 0));
  ck_assert(testQueueFull(queue) == (count == MAX_CAPACITY));

  /* Pop elements from the queue while checking their values */
  for (size_t i = 0; i < count; ++i)
  {
    const TestStruct refElement = createElement(i);
    const TestStruct element = testQueueFront(queue);
    testQueuePopFront(queue);

    ck_assert(compareElements(&element, &refElement) == true);
  }

  ck_assert_uint_eq(testQueueSize(queue), 0);
  ck_assert(testQueueEmpty(queue) == true);
  ck_assert(testQueueFull(queue) == false);
}
/*----------------------------------------------------------------------------*/
START_TEST(testPointerReset)
{
  TestQueue queue;

  /* Queue initialization */
  const bool result = testQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);
  ck_assert_uint_eq(testQueueCapacity(&queue), MAX_CAPACITY);

  /* Phase 0 - pointer to a first element */
  testQueuePushBack(&queue, createElement(0));
  const TestStruct * const phase0 = testQueueAt(&queue, 0);
  testQueuePopFront(&queue);

  /* Phase 1 - pointer to a second element */
  testQueuePushBack(&queue, createElement(0));
  const TestStruct * const phase1 = testQueueAt(&queue, 0);
  ck_assert_ptr_ne(phase1, phase0);

  /* Reset pointers */
  testQueueClear(&queue);
  ck_assert_uint_eq(testQueueSize(&queue), 0);
  ck_assert(testQueueEmpty(&queue) == true);
  ck_assert(testQueueFull(&queue) == false);

  /* Phase 2 - pointer to the same element as in phase 0 */
  testQueuePushBack(&queue, createElement(0));
  const TestStruct * const phase2 = testQueueAt(&queue, 0);
  ck_assert_ptr_eq(phase2, phase0);

  testQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopSequence)
{
  TestQueue queue;

  /* Queue initialization */
  const bool result = testQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  /* Fill queue */
  for (int iter = 1; iter < MAX_CAPACITY * 2; ++iter)
  {
    const size_t count = (size_t)(-abs(iter - MAX_CAPACITY) + MAX_CAPACITY);
    runPushPop(&queue, count);
  }

  testQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testRandomAccess)
{
  TestQueue queue;

  /* Queue initialization */
  const bool result = testQueueInit(&queue, MAX_CAPACITY);
  ck_assert(result == true);

  /* Shift internal pointers */
  for (size_t i = 0; i < MAX_CAPACITY / 2; ++i)
  {
    testQueuePushBack(&queue, createElement(0));
    testQueuePopFront(&queue);
  }

  /* Fill queue */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
    testQueuePushBack(&queue, createElement(i));

  /* Check values and overwrite them */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
  {
    const TestStruct refElement = createElement(i);
    ck_assert(compareElements(testQueueAt(&queue, i), &refElement) == true);
    *testQueueAt(&queue, i) = createElement(MAX_CAPACITY - i);
  }

  /* Check overwritten values */
  for (size_t i = 0; i < MAX_CAPACITY; ++i)
  {
    const TestStruct refElement = createElement(MAX_CAPACITY - i);
    ck_assert(compareElements(testQueueAt(&queue, i), &refElement) == true);
  }

  testQueueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  TestQueue queue;

  mallocHookActive = true;
  const bool result = testQueueInit(&queue, MAX_CAPACITY);
  mallocHookActive = false;

  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Queue");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testPointerReset);
  tcase_add_test(testcase, testPushPopSequence);
  tcase_add_test(testcase, testRandomAccess);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}