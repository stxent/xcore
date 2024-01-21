/*
 * main.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/queue.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 17
/*----------------------------------------------------------------------------*/
typedef struct
{
  int64_t x;
  int32_t y;
  int8_t z;
} __attribute__((packed)) TestStruct;
/*----------------------------------------------------------------------------*/
extern void *__libc_malloc(size_t);

static bool mallocHookActive = false;

void *malloc(size_t size)
{
  if (!mallocHookActive)
    return __libc_malloc(size);
  else
    return NULL;
}
/*----------------------------------------------------------------------------*/
static bool compareElements(const TestStruct *a, const TestStruct *b)
{
  return memcmp(a, b, sizeof(TestStruct)) == 0;
}
/*----------------------------------------------------------------------------*/
static TestStruct createElement(int number)
{
  return (TestStruct){
      (int64_t)((number & 1) ? number : -number),
      (int32_t)((number & 1) ? -number : number),
      (int8_t)number
  };
}
/*----------------------------------------------------------------------------*/
static void runPushPop(struct Queue *queue, int count)
{
  /* Fill the queue with push back function */
  for (int i = 0; i < count; ++i)
  {
    const TestStruct element = createElement(i);
    queuePushBack(queue, &element);
  }

  ck_assert_uint_eq(queueSize(queue), count);
  ck_assert(queueEmpty(queue) == (count == 0));
  ck_assert(queueFull(queue) == (count == MAX_CAPACITY));

  /* Pop elements from the queue while checking their values */
  for (int i = 0; i < count; ++i)
  {
    const TestStruct refElement = createElement(i);
    TestStruct element;

    queueFront(queue, &element);
    queuePopFront(queue);

    ck_assert(compareElements(&element, &refElement) == true);
  }

  ck_assert_uint_eq(queueSize(queue), 0);
  ck_assert(queueEmpty(queue) == true);
  ck_assert(queueFull(queue) == false);
}
/*----------------------------------------------------------------------------*/
static void runPushPopReverse(struct Queue *queue, int count)
{
  /* Fill the queue with push back function */
  for (int i = 0; i < count; ++i)
  {
    const TestStruct element = createElement(i);
    queuePushFront(queue, &element);
  }

  ck_assert_uint_eq(queueSize(queue), count);
  ck_assert(queueEmpty(queue) == (count == 0));
  ck_assert(queueFull(queue) == (count == MAX_CAPACITY));

  /* Pop elements from the queue while checking their values */
  for (int i = 0; i < count; ++i)
  {
    const TestStruct refElement = createElement(i);
    TestStruct element;

    queueBack(queue, &element);
    queuePopBack(queue);

    ck_assert(compareElements(&element, &refElement) == true);
  }

  ck_assert_uint_eq(queueSize(queue), 0);
  ck_assert(queueEmpty(queue) == true);
  ck_assert(queueFull(queue) == false);
}
/*----------------------------------------------------------------------------*/
START_TEST(testEmptyContainer)
{
  struct Queue queue;

  /* Queue initialization */
  const bool result = queueInit(&queue, sizeof(TestStruct), 0);
  ck_assert(result == true);

  ck_assert_uint_eq(queueCapacity(&queue), 0);
  ck_assert_uint_eq(queueSize(&queue), 0);
  ck_assert(queueEmpty(&queue) == true);
  ck_assert(queueFull(&queue) == true);

  queueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPointerReset)
{
  const TestStruct buffer = createElement(0);
  struct Queue queue;

  /* Queue initialization */
  const bool result = queueInit(&queue, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);
  ck_assert_uint_eq(queueCapacity(&queue), MAX_CAPACITY);

  /* Phase 0 - pointer to a first element */
  queuePushBack(&queue, &buffer);
  const TestStruct * const phase0 = queueAt(&queue, 0);
  queuePopFront(&queue);

  /* Phase 1 - pointer to a second element */
  queuePushBack(&queue, &buffer);
  const TestStruct * const phase1 = queueAt(&queue, 0);
  ck_assert_ptr_ne(phase1, phase0);

  /* Reset pointers */
  queueClear(&queue);
  ck_assert_uint_eq(queueSize(&queue), 0);
  ck_assert(queueEmpty(&queue) == true);
  ck_assert(queueFull(&queue) == false);

  /* Phase 2 - pointer to the same element as in phase 0 */
  queuePushBack(&queue, &buffer);
  const TestStruct * const phase2 = queueAt(&queue, 0);
  ck_assert_ptr_eq(phase2, phase0);

  queueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopSequence)
{
  struct Queue queue;

  /* Queue initialization */
  const bool result = queueInit(&queue, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);

  /* Fill queue */
  for (int iter = 1; iter < MAX_CAPACITY * 2; ++iter)
  {
    const int count = -abs(iter - MAX_CAPACITY) + MAX_CAPACITY;
    runPushPop(&queue, count);
  }

  queueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopReverseSequence)
{
  struct Queue queue;

  /* Queue initialization */
  const bool result = queueInit(&queue, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);

  /* Fill queue */
  for (int iter = 1; iter < MAX_CAPACITY * 2; ++iter)
  {
    const int count = -abs(iter - MAX_CAPACITY) + MAX_CAPACITY;
    runPushPopReverse(&queue, count);
  }

  queueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testRandomAccess)
{
  struct Queue queue;

  /* Queue initialization */
  const bool result = queueInit(&queue, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);

  /* Shift internal pointers */
  for (int i = 0; i < MAX_CAPACITY / 2; ++i)
  {
    const TestStruct element = createElement(0);
    queuePushBack(&queue, &element);
    queuePopFront(&queue);
  }

  /* Fill queue */
  for (int i = 0; i < MAX_CAPACITY; ++i)
  {
    const TestStruct element = createElement(i);
    queuePushBack(&queue, &element);
  }

  /* Check values and overwrite them */
  for (int i = 0; i < MAX_CAPACITY; ++i)
  {
    const TestStruct refElement = createElement(i);
    ck_assert(compareElements(queueAt(&queue, i), &refElement) == true);
    *(TestStruct *)queueAt(&queue, i) = createElement(MAX_CAPACITY - i);
  }

  /* Check overwritten values */
  for (int i = 0; i < MAX_CAPACITY; ++i)
  {
    const TestStruct refElement = createElement(MAX_CAPACITY - i);
    ck_assert(compareElements(queueAt(&queue, i), &refElement) == true);
  }

  queueDeinit(&queue);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  struct Queue queue;

  mallocHookActive = true;
  const bool result = queueInit(&queue, sizeof(TestStruct), MAX_CAPACITY);
  mallocHookActive = false;

  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Queue");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testEmptyContainer);
  tcase_add_test(testcase, testPointerReset);
  tcase_add_test(testcase, testPushPopSequence);
  tcase_add_test(testcase, testPushPopReverseSequence);
  tcase_add_test(testcase, testRandomAccess);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
