/*
 * main.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <xcore/containers/array.h>
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
    return 0;
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
static void checkElements(struct Array *array, int base, bool reverse)
{
  const size_t total = arraySize(array);

  for (int i = 0; i < (int)total; ++i)
  {
    const int number = base + (reverse ? ((int)total - i - 1) : i);
    const TestStruct referenceElement = createElement(number);
    const TestStruct * const element = arrayAt(array, i);

    ck_assert(compareElements(element, &referenceElement) == true);
  }
}
/*----------------------------------------------------------------------------*/
static void fillArray(struct Array *array, int base, int count)
{
  for (int i = 0; i < count; ++i)
  {
    const TestStruct element = createElement(base + i);
    arrayPushBack(array, &element);
  }
}
/*----------------------------------------------------------------------------*/
static void eraseElements(struct Array *array, int start, int count)
{
  const size_t total = arraySize(array);

  for (int i = 0; i < (int)count; ++i)
    arrayErase(array, start);

  ck_assert(arraySize(array) == (size_t)(total - count));
}
/*----------------------------------------------------------------------------*/
static void popElements(struct Array *array)
{
  int number = (int)arraySize(array) - 1;

  while (arraySize(array))
  {
    const TestStruct referenceElement = createElement(number);
    TestStruct element;

    arrayBack(array, &element);
    arrayPopBack(array);

    ck_assert(compareElements(&element, &referenceElement) == true);
    --number;
  }
}
/*----------------------------------------------------------------------------*/
START_TEST(testElementErasure)
{
  struct Array array;

  /* Array initialization */
  const bool result = arrayInit(&array, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);

  /* Fill array */
  fillArray(&array, 0, MAX_CAPACITY);

  /* Erase first half */
  eraseElements(&array, 0, MAX_CAPACITY / 2);
  ck_assert_uint_eq(arraySize(&array), MAX_CAPACITY - MAX_CAPACITY / 2);
  checkElements(&array, MAX_CAPACITY / 2, false);
  /* Erase second half */
  eraseElements(&array, 0, MAX_CAPACITY - MAX_CAPACITY / 2);
  ck_assert_uint_eq(arraySize(&array), 0);

  arrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testElementInsertion)
{
  struct Array array;

  /* Array initialization */
  const bool result = arrayInit(&array, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);

  /* Insert even elements */
  for (int i = 0; i < MAX_CAPACITY; i += 2)
  {
    const TestStruct element = createElement(i);
    arrayInsert(&array, arraySize(&array), &element);
  }

  /* Insert odd elements */
  for (int i = 1; i < MAX_CAPACITY; i += 2)
  {
    const TestStruct element = createElement(i);
    arrayInsert(&array, i, &element);
  }

  ck_assert_uint_eq(arraySize(&array), MAX_CAPACITY);
  checkElements(&array, 0, false);

  arrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushPopSequence)
{
  struct Array array;

  /* Array initialization */
  const bool result = arrayInit(&array, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);

  /* Push elements */
  fillArray(&array, 0, MAX_CAPACITY);
  ck_assert_uint_eq(arraySize(&array), MAX_CAPACITY);

  checkElements(&array, 0, false);

  /* Pop elements */
  popElements(&array);
  ck_assert_uint_eq(arraySize(&array), 0);

  arrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testRandomAccess)
{
  struct Array array;

  /* Array initialization */
  const bool result = arrayInit(&array, sizeof(TestStruct), MAX_CAPACITY);
  ck_assert(result == true);
  ck_assert_uint_eq(arrayCapacity(&array), MAX_CAPACITY);

  /* Fill array */
  fillArray(&array, 0, MAX_CAPACITY);
  ck_assert_uint_eq(arraySize(&array), MAX_CAPACITY);
  ck_assert(arrayEmpty(&array) == false);
  ck_assert(arrayFull(&array) == true);
  checkElements(&array, 0, false);

  /* Reverse and check elements */
  for (int forward = 0; forward < MAX_CAPACITY / 2; ++forward)
  {
    TestStruct * const src = arrayAt(&array, forward);
    TestStruct * const dst = arrayAt(&array, MAX_CAPACITY - forward - 1);

    const TestStruct buffer = *src;
    *src = *dst;
    *dst = buffer;
  }

  checkElements(&array, 0, true);

  /* Clear array */
  arrayClear(&array);
  ck_assert_uint_eq(arraySize(&array), 0);
  ck_assert(arrayEmpty(&array) == true);
  ck_assert(arrayFull(&array) == false);

  arrayDeinit(&array);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  struct Array array;

  mallocHookActive = true;
  const bool result = arrayInit(&array, sizeof(TestStruct), MAX_CAPACITY);
  mallocHookActive = false;

  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Array");
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
