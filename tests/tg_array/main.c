/*
 * main.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/tg_array.h>
#include <xcore/helpers.h>
#include <check.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 17
/*----------------------------------------------------------------------------*/
typedef struct
{
  int64_t x;
  int32_t y;
  int8_t z;
} __attribute__((packed)) TestStruct;

DEFINE_ARRAY(TestStruct, Test, test)
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
static void checkElements(TestArray *array, int base, bool reverse)
{
  const size_t total = testArraySize(array);

  for (int i = 0; i < (int)total; ++i)
  {
    const int number = base + (reverse ? ((int)total - i - 1) : i);
    const TestStruct referenceElement = createElement(number);
    const TestStruct * const element = testArrayAt(array, i);

    ck_assert(compareElements(element, &referenceElement) == true);
  }
}
/*----------------------------------------------------------------------------*/
static void fillArray(TestArray *array, int base, int count)
{
  for (int i = 0; i < count; ++i)
    testArrayPushBack(array, createElement(base + i));
}
/*----------------------------------------------------------------------------*/
static void eraseElements(TestArray *array, int start, int count)
{
  const size_t total = testArraySize(array);

  for (int i = 0; i < (int)count; ++i)
    testArrayErase(array, start);

  ck_assert(testArraySize(array) == (size_t)(total - count));
}
/*----------------------------------------------------------------------------*/
static void popElements(TestArray *array)
{
  int number = (int)testArraySize(array) - 1;

  while (testArraySize(array))
  {
    const TestStruct referenceElement = createElement(number);
    const TestStruct element = testArrayBack(array);
    testArrayPopBack(array);

    ck_assert(compareElements(&element, &referenceElement) == true);
    --number;
  }
}
/*----------------------------------------------------------------------------*/
START_TEST(testElementErasure)
{
  TestArray array;

  /* Array initialization */
  const bool result = testArrayInit(&array, MAX_CAPACITY);
  ck_assert(result == true);

  /* Fill array */
  fillArray(&array, 0, MAX_CAPACITY);

  /* Erase first half */
  eraseElements(&array, 0, MAX_CAPACITY / 2);
  ck_assert_uint_eq(testArraySize(&array), MAX_CAPACITY - MAX_CAPACITY / 2);
  checkElements(&array, MAX_CAPACITY / 2, false);
  /* Erase second half */
  eraseElements(&array, 0, MAX_CAPACITY - MAX_CAPACITY / 2);
  ck_assert_uint_eq(testArraySize(&array), 0);

  testArrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testElementInsertion)
{
  TestArray array;

  /* Array initialization */
  const bool result = testArrayInit(&array, MAX_CAPACITY);
  ck_assert(result == true);

  /* Insert even elements */
  for (int i = 0; i < MAX_CAPACITY; i += 2)
    testArrayInsert(&array, testArraySize(&array), createElement(i));

  /* Insert odd elements */
  for (int i = 1; i < MAX_CAPACITY; i += 2)
    testArrayInsert(&array, i, createElement(i));

  ck_assert_uint_eq(testArraySize(&array), MAX_CAPACITY);
  checkElements(&array, 0, false);

  testArrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testEmptyContainer)
{
  TestArray array;

  /* Array initialization */
  const bool result = testArrayInit(&array, 0);
  ck_assert(result == true);

  ck_assert_uint_eq(testArrayCapacity(&array), 0);
  ck_assert_uint_eq(testArraySize(&array), 0);
  ck_assert(testArrayEmpty(&array) == true);
  ck_assert(testArrayFull(&array) == true);

  testArrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopArenaSequence)
{
  TestArray array;
  TestStruct arena[MAX_CAPACITY];

  /* Array initialization */
  testArrayInitArena(&array, MAX_CAPACITY, arena);

  /* Push elements */
  fillArray(&array, 0, MAX_CAPACITY);
  ck_assert_uint_eq(testArraySize(&array), MAX_CAPACITY);

  checkElements(&array, 0, false);

  /* Pop elements */
  popElements(&array);
  ck_assert_uint_eq(testArraySize(&array), 0);

  testArrayDeinitArena(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopSequence)
{
  TestArray array;

  /* Array initialization */
  const bool result = testArrayInit(&array, MAX_CAPACITY);
  ck_assert(result == true);

  /* Push elements */
  fillArray(&array, 0, MAX_CAPACITY);
  ck_assert_uint_eq(testArraySize(&array), MAX_CAPACITY);

  checkElements(&array, 0, false);

  /* Pop elements */
  popElements(&array);
  ck_assert_uint_eq(testArraySize(&array), 0);

  testArrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testRandomAccess)
{
  TestArray array;

  /* Array initialization */
  const bool result = testArrayInit(&array, MAX_CAPACITY);
  ck_assert(result == true);
  ck_assert_uint_eq(testArrayCapacity(&array), MAX_CAPACITY);

  /* Fill array */
  fillArray(&array, 0, MAX_CAPACITY);
  ck_assert_uint_eq(testArraySize(&array), MAX_CAPACITY);
  ck_assert(testArrayEmpty(&array) == false);
  ck_assert(testArrayFull(&array) == true);
  checkElements(&array, 0, false);

  /* Reverse and check elements */
  for (int forward = 0; forward < MAX_CAPACITY / 2; ++forward)
  {
    const int backward = MAX_CAPACITY - forward - 1;
    const TestStruct buffer = *testArrayAt(&array, forward);
    *testArrayAt(&array, forward) = *testArrayAt(&array, backward);
    *testArrayAt(&array, backward) = buffer;
  }

  checkElements(&array, 0, true);

  /* Clear array */
  testArrayClear(&array);
  ck_assert_uint_eq(testArraySize(&array), 0);
  ck_assert(testArrayEmpty(&array) == true);
  ck_assert(testArrayFull(&array) == false);

  testArrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  TestArray array;

  mallocHookActive = true;
  const bool result = testArrayInit(&array, MAX_CAPACITY);
  mallocHookActive = false;

  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("TypeGenericArray");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testEmptyContainer);
  tcase_add_test(testcase, testRandomAccess);
  tcase_add_test(testcase, testElementErasure);
  tcase_add_test(testcase, testElementInsertion);
  tcase_add_test(testcase, testPushPopArenaSequence);
  tcase_add_test(testcase, testPushPopSequence);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
