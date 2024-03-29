/*
 * main.c
 * Copyright (C) 2018 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/tg_list.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_SIZE 17
/*----------------------------------------------------------------------------*/
typedef struct [[gnu::packed]]
{
  int64_t x;
  int32_t y;
  int8_t z;
} TestStruct;

DEFINE_LIST(TestStruct, Test, test)
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
static void checkElements(TestList *list, int base, int step)
{
  TestListNode *node = testListFront(list);
  int i = 0;

  while (node != NULL)
  {
    const TestStruct referenceElement = createElement(base + i * step);
    const TestStruct element = *testListData(node);

    ck_assert(compareElements(&element, &referenceElement) == true);

    ++i;
    node = testListNext(node);
  }
}
/*----------------------------------------------------------------------------*/
static bool evenElementFinder(const void *aObject, void *)
{
  const TestStruct * const a = aObject;
  return (a->z % 2) == 0;
}
/*----------------------------------------------------------------------------*/
static bool oddElementFinder(const void *aObject, void *)
{
  const TestStruct * const a = aObject;
  return (a->z % 2) != 0;
}
/*----------------------------------------------------------------------------*/
static bool simplifiedComparator(const void *aObject, void *bObject)
{
  const TestStruct * const a = aObject;
  const TestStruct * const b = bObject;

  return a->z == b->z;
}
/*----------------------------------------------------------------------------*/
START_TEST(testGenericFunctions)
{
  TestList list;
  testListInit(&list);

  ck_assert_uint_eq(testListSize(&list), 0);
  ck_assert(testListEmpty(&list) == true);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  ck_assert_uint_eq(testListSize(&list), MAX_SIZE);
  ck_assert(testListEmpty(&list) == false);
  checkElements(&list, 0, 1);

  /* Clear list */
  testListClear(&list);
  ck_assert_uint_eq(testListSize(&list), 0);
  ck_assert(testListEmpty(&list) == true);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testErase)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  /* Remove even elements */
  for (int i = 0; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(i);
    testListErase(&list, element);
  }

  ck_assert_uint_eq(testListSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Remove odd elements */
  for (int i = 1; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(i);
    testListErase(&list, element);
  }

  ck_assert(testListEmpty(&list) == true);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testEraseIf)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  /* Remove even elements */
  testListEraseIf(&list, 0, evenElementFinder);
  ck_assert_uint_eq(testListSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Remove odd elements */
  testListEraseIf(&list, 0, oddElementFinder);
  ck_assert(testListEmpty(&list) == true);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testEraseNode)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  TestListNode *node;

  /* Remove even elements */
  node = testListFront(&list);
  while (node != NULL)
  {
    node = testListEraseNode(&list, node);
    if (node != NULL)
      node = testListNext(node);
  }
  ck_assert_uint_eq(testListSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testFind)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  /* Find elements using default comparator */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert_ptr_nonnull(testListFind(&list, element));
  }

  /* Try to find non-existent elements */
  const TestStruct incorrectElement = createElement(MAX_SIZE);
  ck_assert_ptr_null(testListFind(&list, incorrectElement));

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testFindIf)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  TestStruct element = createElement(MAX_SIZE);

  /* Try to find non-existent elements */
  ck_assert_ptr_null(testListFindIf(&list, &element, simplifiedComparator));

  /* Find elements using both comparator types */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    element.z = i;
    ck_assert_ptr_null(testListFind(&list, element));
    ck_assert_ptr_nonnull(testListFindIf(&list, &element, simplifiedComparator));
  }

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testInsert)
{
  TestList list;
  testListInit(&list);

  /* Push odd elements */
  for (int i = 1; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  ck_assert_uint_eq(testListSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Insert first element */
  const TestStruct firstElement = createElement(0);
  ck_assert(testListInsert(&list, 0, firstElement) == true);

  /* Insert even elements */
  TestListNode *node = testListFront(&list);
  int id = 2;

  ck_assert_ptr_nonnull(node);
  node = testListNext(node);

  while (id < MAX_SIZE)
  {
    ck_assert_ptr_nonnull(node);

    const TestStruct element = createElement(id);
    ck_assert(testListInsert(&list, node, element) == true);

    node = testListNext(node);
    ck_assert_ptr_nonnull(node);
    node = testListNext(node);

    id += 2;
  }
  ck_assert_uint_eq(testListSize(&list), MAX_SIZE);
  checkElements(&list, 0, 1);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushBack)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushBack(&list, element) == true);
  }

  ck_assert_uint_eq(testListSize(&list), MAX_SIZE);
  checkElements(&list, 0, 1);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushFront)
{
  TestList list;
  testListInit(&list);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(testListPushFront(&list, element) == true);
  }

  ck_assert_uint_eq(testListSize(&list), MAX_SIZE);
  checkElements(&list, MAX_SIZE - 1, -1);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  const TestStruct element = createElement(0);
  TestList list;
  bool result;

  testListInit(&list);

  mallocHookActive = true;
  result = testListPushFront(&list, element);
  mallocHookActive = false;
  ck_assert(result == false);

  mallocHookActive = true;
  result = testListPushBack(&list, element);
  mallocHookActive = false;
  ck_assert(result == false);

  mallocHookActive = true;
  result = testListInsert(&list, 0, element);
  mallocHookActive = false;
  ck_assert(result == false);

  testListDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("TypeGenericList");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testGenericFunctions);
  tcase_add_test(testcase, testErase);
  tcase_add_test(testcase, testEraseIf);
  tcase_add_test(testcase, testEraseNode);
  tcase_add_test(testcase, testFind);
  tcase_add_test(testcase, testFindIf);
  tcase_add_test(testcase, testInsert);
  tcase_add_test(testcase, testPushBack);
  tcase_add_test(testcase, testPushFront);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
