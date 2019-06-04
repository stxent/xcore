/*
 * main.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <check.h>
#include <xcore/helpers.h>
#include <xcore/containers/tg_array.h>
/*----------------------------------------------------------------------------*/
#define MAX_CAPACITY 17
/*----------------------------------------------------------------------------*/
typedef struct
{
  int64_t a;
  int32_t b;
  int8_t c;
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
static void checkElements(TestArray *array, size_t base, bool reverse)
{
  const size_t size = testArraySize(array);

  for (size_t i = 0; i < size; ++i)
  {
    const size_t id = base + (reverse ? size - i - 1 : i);
    const TestStruct referenceElement = createElement(id);
    const TestStruct * const element = testArrayAt(array, i);

    ck_assert(compareElements(element, &referenceElement) == true);
  }
}
/*----------------------------------------------------------------------------*/
static void fillArray(TestArray *array, size_t base, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    testArrayPushBack(array, createElement(base + i));
}
/*----------------------------------------------------------------------------*/
static void eraseElements(TestArray *array, size_t start, size_t count)
{
  const size_t total = testArraySize(array);

  for (size_t i = 0; i < count; ++i)
    testArrayErase(array, start);

  ck_assert(testArraySize(array) == total - count);
}
/*----------------------------------------------------------------------------*/
static void popElements(TestArray *array)
{
  size_t id = testArraySize(array) - 1;

  while (testArraySize(array))
  {
    const TestStruct referenceElement = createElement(id);
    const TestStruct element = testArrayBack(array);
    testArrayPopBack(array);

    ck_assert(compareElements(&element, &referenceElement) == true);
    --id;
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
  for (size_t i = 0; i < MAX_CAPACITY; i += 2)
    testArrayInsert(&array, testArraySize(&array), createElement(i));

  /* Insert odd elements */
  for (size_t i = 1; i < MAX_CAPACITY; i += 2)
    testArrayInsert(&array, i, createElement(i));

  ck_assert_uint_eq(testArraySize(&array), MAX_CAPACITY);
  checkElements(&array, 0, false);

  testArrayDeinit(&array);
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
  for (size_t forward = 0; forward < MAX_CAPACITY / 2; ++forward)
  {
    const size_t backward = MAX_CAPACITY - forward - 1;
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

  tcase_add_test(testcase, testRandomAccess);
  tcase_add_test(testcase, testElementErasure);
  tcase_add_test(testcase, testElementInsertion);
  tcase_add_test(testcase, testPushPopSequence);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
