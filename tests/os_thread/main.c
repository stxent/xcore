/*
 * main.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/semaphore.h>
#include <xcore/os/thread.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
static void onThreadStopped(void *argument)
{
  *(bool *)argument = true;
}
/*----------------------------------------------------------------------------*/
static void run(void *argument)
{
  struct Semaphore * const sem = argument;

  msleep(10);
  semPost(sem);
}
/*----------------------------------------------------------------------------*/
START_TEST(testErrorCases)
{
  struct Semaphore sem;
  struct Thread thread;
  enum Result res;

  res = semInit(&sem, 0);
  ck_assert_uint_eq(res, E_OK);
  threadInit(&thread, 1024, 0, run, &sem);

  res = threadStart(&thread);
  ck_assert_uint_eq(res, E_OK);

  semWait(&sem);
  res = threadStart(&thread);
  ck_assert_uint_ne(res, E_OK);

  threadTerminate(&thread);
  threadTerminate(&thread);

  threadDeinit(&thread);
  semDeinit(&sem);
}
/*----------------------------------------------------------------------------*/
START_TEST(testStartStop)
{
  struct Semaphore sem;
  struct Thread thread;
  enum Result res;

  res = semInit(&sem, 0);
  ck_assert_uint_eq(res, E_OK);

  threadInit(&thread, 1024, 0, run, &sem);
  res = threadStart(&thread);
  ck_assert_uint_eq(res, E_OK);

  semWait(&sem);
  threadTerminate(&thread);

  threadDeinit(&thread);
  semDeinit(&sem);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testStopCallback)
{
  struct Semaphore sem;
  struct Thread thread;
  enum Result res;
  bool ok = false;

  res = semInit(&sem, 0);
  ck_assert_uint_eq(res, E_OK);
  threadInit(&thread, 1024, 0, run, &sem);
  threadOnTerminateCallback(&thread, onThreadStopped, &ok);

  res = threadStart(&thread);
  ck_assert_uint_eq(res, E_OK);

  semWait(&sem);
  threadTerminate(&thread);
  ck_assert(ok == true);

  threadDeinit(&thread);
  semDeinit(&sem);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Thread");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testErrorCases);
  tcase_add_test(testcase, testStartStop);
  tcase_add_test(testcase, testStopCallback);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
