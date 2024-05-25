/*
 * main.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/semaphore.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
START_TEST(testLockUnlock)
{
  struct Semaphore sem;
  enum Result res;
  int value;
  bool ok;

  res = semInit(&sem, 1);
  ck_assert_uint_eq(res, E_OK);
  value = semValue(&sem);
  ck_assert_uint_eq(value, 1);

  semWait(&sem);
  value = semValue(&sem);
  ck_assert_uint_eq(value, 0);

  semPost(&sem);
  ok = semTryWait(&sem, 0);
  ck_assert(ok == true);

  semPost(&sem);
  ok = semTryWait(&sem, 100);
  ck_assert(ok == true);

  ok = semTryWait(&sem, 0);
  ck_assert(ok == false);

  ok = semTryWait(&sem, 10);
  ck_assert(ok == false);

  semDeinit(&sem);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Semaphore");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testLockUnlock);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
