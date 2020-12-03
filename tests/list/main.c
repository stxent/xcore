/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/list.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#define MAX_SIZE 17
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
static void checkElements(struct List *list, int base, int step)
{
  struct ListNode *node = listFront(list);
  int i = 0;

  while (node)
  {
    const TestStruct referenceElement = createElement(base + i * step);

    TestStruct element;
    listData(list, node, &element);

    ck_assert(compareElements(&element, &referenceElement) == true);

    ++i;
    node = listNext(node);
  }
}
/*----------------------------------------------------------------------------*/
static bool evenElementFinder(const void *aObject,
    void *bObject __attribute__((unused)))
{
  const TestStruct * const a = aObject;
  return (a->z % 2) == 0;
}
/*----------------------------------------------------------------------------*/
static bool oddElementFinder(const void *aObject,
    void *bObject __attribute__((unused)))
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
  struct List list;
  listInit(&list, sizeof(TestStruct));

  ck_assert_uint_eq(listSize(&list), 0);
  ck_assert(listEmpty(&list) == true);

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  ck_assert(listEmpty(&list) == false);
  checkElements(&list, 0, 1);

  /* Clear list */
  listClear(&list);
  ck_assert_uint_eq(listSize(&list), 0);
  ck_assert(listEmpty(&list) == true);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testErase)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  /* Remove even elements */
  for (int i = 0; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(i);
    listErase(&list, &element);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Remove odd elements */
  for (int i = 1; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(i);
    listErase(&list, &element);
  }

  ck_assert(listEmpty(&list) == true);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testEraseIf)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  /* Remove even elements */
  listEraseIf(&list, 0, evenElementFinder);
  ck_assert_uint_eq(listSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Remove odd elements */
  listEraseIf(&list, 0, oddElementFinder);
  ck_assert(listEmpty(&list) == true);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testEraseNode)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  struct ListNode *node;

  /* Remove even elements */
  node = listFront(&list);
  while (node)
  {
    node = listEraseNode(&list, node);
    if (node)
      node = listNext(node);
  }
  ck_assert_uint_eq(listSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testFind)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  /* Find elements using default comparator */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert_ptr_nonnull(listFind(&list, &element));
  }

  /* Try to find non-existent elements */
  const TestStruct incorrectElement = createElement(MAX_SIZE);
  ck_assert_ptr_null(listFind(&list, &incorrectElement));

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testFindIf)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  TestStruct element = createElement(MAX_SIZE);

  /* Try to find non-existent elements */
  ck_assert_ptr_null(listFindIf(&list, &element, simplifiedComparator));

  /* Find elements using both comparator types */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    element.z = i;
    ck_assert_ptr_null(listFind(&list, &element));
    ck_assert_ptr_nonnull(listFindIf(&list, &element, simplifiedComparator));
  }

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testInsert)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push odd elements */
  for (int i = 1; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Insert first element */
  const TestStruct firstElement = createElement(0);
  ck_assert(listInsert(&list, 0, &firstElement) == true);

  /* Insert even elements */
  struct ListNode *node = listFront(&list);
  int id = 2;

  ck_assert_ptr_nonnull(node);
  node = listNext(node);

  while (id < MAX_SIZE)
  {
    ck_assert_ptr_nonnull(node);

    const TestStruct element = createElement(id);
    ck_assert(listInsert(&list, node, &element) == true);

    node = listNext(node);
    ck_assert_ptr_nonnull(node);
    node = listNext(node);

    id += 2;
  }
  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  checkElements(&list, 0, 1);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushBack)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushBack(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  checkElements(&list, 0, 1);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushFront)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  /* Push elements */
  for (int i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    ck_assert(listPushFront(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  checkElements(&list, MAX_SIZE - 1, -1);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  const TestStruct element = createElement(0);
  struct List list;
  bool result;

  listInit(&list, sizeof(TestStruct));

  mallocHookActive = true;
  result = listPushFront(&list, &element);
  mallocHookActive = false;
  ck_assert(result == false);

  mallocHookActive = true;
  result = listPushBack(&list, &element);
  mallocHookActive = false;
  ck_assert(result == false);

  mallocHookActive = true;
  result = listInsert(&list, 0, &element);
  mallocHookActive = false;
  ck_assert(result == false);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("List");
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
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
