/*
 * main.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/mutex.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
START_TEST(testLockUnlock)
{
  struct Mutex mut;
  enum Result res;
  bool ok;

  res = mutexInit(&mut);
  ck_assert_uint_eq(res, E_OK);

  mutexLock(&mut);
  mutexUnlock(&mut);

  ok = mutexTryLock(&mut, 0);
  ck_assert(ok == true);
  mutexUnlock(&mut);

  ok = mutexTryLock(&mut, 100);
  ck_assert(ok == true);
  mutexUnlock(&mut);

  mutexLock(&mut);
  ok = mutexTryLock(&mut, 10);
  ck_assert(ok == false);
  mutexUnlock(&mut);

  mutexDeinit(&mut);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Mutex");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testLockUnlock);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
