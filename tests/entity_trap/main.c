/*
 * main.c
 * Copyright (C) 2020 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/entity.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
struct TestObject
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
static enum Result testClassInit(void *, const void *);
/*----------------------------------------------------------------------------*/
const struct EntityClass * const IncorrectEntity = &(const struct EntityClass){
    .size = sizeof(struct TestObject),
    .init = testClassInit,
    .deinit = deletedDestructorTrap
};
/*----------------------------------------------------------------------------*/
static enum Result testClassInit([[maybe_unused]] void *objectBase,
    [[maybe_unused]] const void *configBase)
{
  ck_assert_ptr_nonnull(objectBase);
  return E_OK;
}
/*----------------------------------------------------------------------------*/
START_TEST(testAbortGeneration)
{
  struct TestObject * const object = init(IncorrectEntity, NULL);
  ck_assert_ptr_nonnull(object);
  deinit(object);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Entity");
  TCase * const testcase = tcase_create("Core");

  tcase_add_exit_test(testcase, testAbortGeneration, EXIT_FAILURE);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
