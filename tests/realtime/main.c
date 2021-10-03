/*
 * main.c
 * Copyright (C) 2021 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/realtime.h>
#include <check.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
START_TEST(testDateTimeToTimestamp)
{
  time64_t timestamp;
  enum Result res;

  /* Test leap year calculations */

  static const struct RtDateTime dtLeapYear = {
      .year = 2000,
      .month = 3,
      .day = 1,
      .hour = 10,
      .minute = 29,
      .second = 59
  };
  res = rtMakeEpochTime(&timestamp, &dtLeapYear);
  ck_assert_uint_eq(res, E_OK);
  ck_assert_uint_eq(timestamp, 951906599);

  /* Test month errors */

  static const struct RtDateTime dtZeroMonth = {
      .year = 2000,
      .month = 0,
      .day = 1,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtZeroMonth);
  ck_assert_uint_ne(res, E_OK);

  static const struct RtDateTime dtMonthOverflow = {
      .year = 2000,
      .month = 13,
      .day = 1,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtMonthOverflow);
  ck_assert_uint_ne(res, E_OK);

  /* Test day errors */

  static const struct RtDateTime dtZeroDay = {
      .year = 2000,
      .month = 1,
      .day = 0,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtZeroDay);
  ck_assert_uint_ne(res, E_OK);

  static const struct RtDateTime dtFebLeapYear = {
      .year = 2000,
      .month = 2,
      .day = 30,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtFebLeapYear);
  ck_assert_uint_ne(res, E_OK);

  static const struct RtDateTime dtFebYear = {
      .year = 2001,
      .month = 2,
      .day = 29,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtFebYear);
  ck_assert_uint_ne(res, E_OK);

  /* Test hour, minute and second errors */

  static const struct RtDateTime dtHourOverflow = {
      .year = 2000,
      .month = 1,
      .day = 1,
      .hour = 24,
      .minute = 0,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtHourOverflow);
  ck_assert_uint_ne(res, E_OK);

  static const struct RtDateTime dtMinuteOverflow = {
      .year = 2000,
      .month = 1,
      .day = 1,
      .hour = 0,
      .minute = 60,
      .second = 0
  };
  res = rtMakeEpochTime(&timestamp, &dtMinuteOverflow);
  ck_assert_uint_ne(res, E_OK);

  static const struct RtDateTime dtSecondOverflow = {
      .year = 2000,
      .month = 1,
      .day = 1,
      .hour = 0,
      .minute = 0,
      .second = 60
  };
  res = rtMakeEpochTime(&timestamp, &dtSecondOverflow);
  ck_assert_uint_ne(res, E_OK);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTimestampToDateTime)
{
  struct RtDateTime dt;

  /* Test 01.01.1971 12:00:00 */

  static const time64_t timestamp0 = 31579200;

  rtMakeTime(&dt, timestamp0);
  ck_assert_uint_eq(dt.year, 1971);
  ck_assert_uint_eq(dt.month, 1);
  ck_assert_uint_eq(dt.day, 1);
  ck_assert_uint_eq(dt.hour, 12);
  ck_assert_uint_eq(dt.minute, 0);
  ck_assert_uint_eq(dt.second, 0);

  /* Test 01.02.2003 10:29:59 */

  static const time64_t timestamp1 = 1044095399;

  rtMakeTime(&dt, timestamp1);
  ck_assert_uint_eq(dt.year, 2003);
  ck_assert_uint_eq(dt.month, 2);
  ck_assert_uint_eq(dt.day, 1);
  ck_assert_uint_eq(dt.hour, 10);
  ck_assert_uint_eq(dt.minute, 29);
  ck_assert_uint_eq(dt.second, 59);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Realtime");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testDateTimeToTimestamp);
  tcase_add_test(testcase, testTimestampToDateTime);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int numberFailed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return numberFailed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
