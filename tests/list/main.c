/*
 * main.c
 * Copyright (C) 2014 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include <check.h>
#include <stdlib.h>
#include <xcore/containers/list.h>
/*----------------------------------------------------------------------------*/
#define MAX_SIZE 17
/*----------------------------------------------------------------------------*/
typedef struct
{
  int64_t a;
  int32_t b;
  int8_t c;
} TestStruct;
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
  return (a->a == b->a) && (a->b == b->b) && (a->c == b->c);
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
static void checkElements(struct List *list, size_t base, size_t step)
{
  struct ListNode *node = listFront(list);
  size_t i = 0;

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
static int customComparator(const void *a, const void *b)
{
  const TestStruct * const aValue = a;
  const TestStruct * const bValue = b;

  return aValue->a - bValue->a;
}
/*----------------------------------------------------------------------------*/
START_TEST(testFind)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  ck_assert_uint_eq(listSize(&list), 0);
  ck_assert(listEmpty(&list) == true);

  /* Push elements */
  for (size_t i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(MAX_SIZE - 1 - i);
    ck_assert(listPushFront(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  ck_assert(listEmpty(&list) == false);
  checkElements(&list, 0, 1);

  /* Find elements using default comparator */
  for (size_t i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(i);
    const struct ListNode * const node = listFind(&list, &element);

    ck_assert_ptr_ne(node, 0);
  }

  /* Find elements using custom comparator in descending order */
  for (size_t i = MAX_SIZE; i > 0; --i)
  {
    const TestStruct element = createElement(i - 1);
    const struct ListNode * const node = listFindIf(&list, &element,
        customComparator);

    ck_assert_ptr_ne(node, 0);
  }

  /* Try to find non-existent elements */
  const TestStruct incorrectElement = createElement(MAX_SIZE);

  const struct ListNode * const incorrectDefaultNode = listFind(&list,
      &incorrectElement);
  ck_assert_ptr_eq(incorrectDefaultNode, 0);

  const struct ListNode * const incorrectCustomNode = listFindIf(&list,
      &incorrectElement, customComparator);
  ck_assert_ptr_eq(incorrectCustomNode, 0);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testInsert)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  ck_assert_uint_eq(listSize(&list), 0);
  ck_assert(listEmpty(&list) == true);

  /* Push odd elements */
  for (size_t i = 1; i < MAX_SIZE; i += 2)
  {
    const TestStruct element = createElement(MAX_SIZE - 1 - i);
    ck_assert(listPushFront(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE / 2);
  ck_assert(listEmpty(&list) == false);
  checkElements(&list, 1, 2);

  /* Insert first element */
  const TestStruct firstElement = createElement(0);
  ck_assert(listInsert(&list, 0, &firstElement) == true);

  /* Insert even elements */
  struct ListNode *node = listFront(&list);
  size_t id = 2;

  ck_assert_ptr_ne(node, 0);
  node = listNext(node);

  while (id < MAX_SIZE)
  {
    ck_assert_ptr_ne(node, 0);

    const TestStruct element = createElement(id);
    ck_assert(listInsert(&list, node, &element) == true);

    node = listNext(node);
    ck_assert_ptr_ne(node, 0);
    node = listNext(node);

    id += 2;
  }
  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  checkElements(&list, 0, 1);

  /* Clear list */
  listClear(&list);
  ck_assert_uint_eq(listSize(&list), 0);

  listDeinit(&list);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testPushErase)
{
  struct List list;
  listInit(&list, sizeof(TestStruct));

  ck_assert_uint_eq(listSize(&list), 0);
  ck_assert(listEmpty(&list) == true);

  /* Push elements */
  for (size_t i = 0; i < MAX_SIZE; ++i)
  {
    const TestStruct element = createElement(MAX_SIZE - 1 - i);
    ck_assert(listPushFront(&list, &element) == true);
  }

  ck_assert_uint_eq(listSize(&list), MAX_SIZE);
  ck_assert(listEmpty(&list) == false);
  checkElements(&list, 0, 1);

  struct ListNode *node;

  /* Remove even elements */
  node = listFront(&list);
  while (node)
  {
    node = listErase(&list, node);
    if (node)
      node = listNext(node);
  }
  ck_assert_uint_eq(listSize(&list), MAX_SIZE / 2);
  checkElements(&list, 1, 2);

  /* Clear list */
  listClear(&list);
  ck_assert_uint_eq(listSize(&list), 0);

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
  result = listInsert(&list, 0, &element);
  mallocHookActive = false;
  ck_assert(result == false);

  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("List");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testFind);
  tcase_add_test(testcase, testInsert);
  tcase_add_test(testcase, testPushErase);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
