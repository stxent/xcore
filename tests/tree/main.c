/*
 * main.c
 * Copyright (C) 2016 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/containers/tree.h>
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
#ifdef CONFIG_DEBUG
#  define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#  define DEBUG_PRINT(...) do {} while (0)
#endif

#define MAX_SIZE 17
/*----------------------------------------------------------------------------*/
typedef struct [[gnu::packed]]
{
  int64_t x;
  int32_t y;
  int8_t z;
} TestStruct;
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
static int compareElements(const void *a, const void *b)
{
  return memcmp(a, b, sizeof(TestStruct));
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
static void shuffleElements(TestStruct *elements, size_t count, int seed)
{
  srand(seed);

  for (size_t i = count - 1; i; --i)
  {
    const size_t j = rand() % (i + 1);
    TestStruct swap;

    memcpy(&swap, elements + i, sizeof(TestStruct));
    memcpy(elements + i, elements + j, sizeof(TestStruct));
    memcpy(elements + j, &swap, sizeof(TestStruct));
  }
}
/*----------------------------------------------------------------------------*/
START_TEST(testClear)
{
  static const int SEEDS[] = {
      127, 131, 137, 139, 283, 293, 307, 311
  };
  TestStruct values[MAX_SIZE];

  struct Tree tree;
  treeInit(&tree, sizeof(TestStruct), compareElements);

  for (size_t seed = 0; seed < ARRAY_SIZE(SEEDS); ++seed)
  {
    DEBUG_PRINT("Test clearing seed %i\r\n", SEEDS[seed]);

    for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
      values[i] = createElement((int)i);

    shuffleElements(values, ARRAY_SIZE(values), SEEDS[seed]);

    for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
      ck_assert(treeInsert(&tree, &values[i]) == true);
    ck_assert_uint_eq(treeSize(&tree), ARRAY_SIZE(values));
    ck_assert(treeEmpty(&tree) == false);

    treeClear(&tree);
    ck_assert_uint_eq(treeSize(&tree), 0);
    ck_assert(treeEmpty(&tree) == true);
  }

  treeDeinit(&tree);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testInsert)
{
  static const int SEEDS[] = {
      1002109, 1002121, 1002143, 1002149, 1002151, 1002173, 1002191, 1002227,
  };
  TestStruct values[MAX_SIZE];

  struct Tree tree;
  treeInit(&tree, sizeof(TestStruct), compareElements);

  for (size_t seed = 0; seed < ARRAY_SIZE(SEEDS); ++seed)
  {
    DEBUG_PRINT("Test insertion seed %i\r\n", SEEDS[seed]);

    for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
      values[i] = createElement((int)i);

    shuffleElements(values, ARRAY_SIZE(values), SEEDS[seed]);

    for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
      ck_assert(treeInsert(&tree, &values[i]) == true);
    ck_assert_uint_eq(treeSize(&tree), ARRAY_SIZE(values));

    for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
    {
      struct TreeNode * const node = treeFind(&tree, &values[i]);
      ck_assert_ptr_nonnull(node);

      TestStruct element;
      treeData(&tree, node, &element);
      ck_assert(compareElements(&element, &values[i]) == 0);
    }

    for (size_t i = 0; i < ARRAY_SIZE(values); ++i)
    {
      struct TreeNode *node;

      node = treeFind(&tree, &values[i]);
      ck_assert_ptr_nonnull(node);

      treeErase(&tree, node);

      node = treeFind(&tree, &values[i]);
      ck_assert_ptr_null(node);
    }
    ck_assert(treeEmpty(&tree) == true);
  }

  treeDeinit(&tree);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testMemoryFailure)
{
  const TestStruct element = createElement(0);
  struct Tree tree;
  bool result;

  treeInit(&tree, sizeof(TestStruct), compareElements);

  mallocHookActive = true;
  result = treeInsert(&tree, &element);
  mallocHookActive = false;
  ck_assert(result == false);

  treeDeinit(&tree);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Tree");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testClear);
  tcase_add_test(testcase, testInsert);
  tcase_add_test(testcase, testMemoryFailure);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
