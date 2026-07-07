/*
 * main.c
 * Copyright (C) 2021 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/realtime.h>
#include <check.h>
#include <stdlib.h>
#include <time.h>
/*----------------------------------------------------------------------------*/
START_TEST(testBidirectionalConversion)
{
  static const unsigned int monthLengthMap[] = {
      31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
  };
  static const unsigned int secondsPerDay = 86400;

  static const time64_t start = 915192000LL; /* 01.01.1999 12:00:00 */
  static const time64_t end = 1104494400LL; /* 31.12.2004 12:00:00 */
  time64_t timestamp = start;

  while (timestamp <= end)
  {
    const struct tm * const ref = gmtime(&timestamp);
    struct RtDateTime dt;

    /* Unix time to date and time conversion */
    rtMakeTime(&dt, timestamp);

    ck_assert_uint_eq(ref->tm_year + 1900, dt.year);
    ck_assert_uint_eq(ref->tm_mon + 1, dt.month);
    ck_assert_uint_eq(ref->tm_mday, dt.day);
    ck_assert_uint_eq(ref->tm_hour, dt.hour);
    ck_assert_uint_eq(ref->tm_min, dt.minute);
    ck_assert_uint_eq(ref->tm_sec, dt.second);

    /* Date and time to Unix time conversion */
    time64_t converted;
    const bool result = rtMakeEpochTime(&converted, &dt);

    ck_assert(result == true);
    ck_assert_int_eq(timestamp, converted);

    /* Choose a next time stamp */
    if (dt.day == 1)
      timestamp += (monthLengthMap[dt.month - 1] - 1) * secondsPerDay;
    else
      timestamp += secondsPerDay;
  }
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testDateTimeToTimestamp)
{
  time64_t timestamp;
  bool result;

  /* Test simple cases */

  static const struct RtDateTime dtSimpleCase = {
      .year = 1970,
      .month = 1,
      .day = 1,
      .hour = 18,
      .minute = 49,
      .second = 59
  };
  result = rtMakeEpochTime(&timestamp, &dtSimpleCase);
  ck_assert(result == true);
  ck_assert_int_eq(timestamp, 67799LL);

  /* Test leap year calculations */

  static const struct RtDateTime dtLeapYear = {
      .year = 2000,
      .month = 3,
      .day = 1,
      .hour = 10,
      .minute = 29,
      .second = 59
  };
  result = rtMakeEpochTime(&timestamp, &dtLeapYear);
  ck_assert(result == true);
  ck_assert_int_eq(timestamp, 951906599LL);

  static const struct RtDateTime dtNotLeapYear = {
      .year = 2200,
      .month = 3,
      .day = 1,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtNotLeapYear);
  ck_assert(result == true);
  ck_assert_int_eq(timestamp, 7263259200LL);

  /* Test negative time stamps */

  static const struct RtDateTime dtNegativeTime = {
      .year = 1930,
      .month = 4,
      .day = 23,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtNegativeTime);
  ck_assert(result == true);
  ck_assert_int_eq(timestamp, -1252584000LL);

  /* Test month errors */

  static const struct RtDateTime dtZeroMonth = {
      .year = 2000,
      .month = 0,
      .day = 1,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtZeroMonth);
  ck_assert(result == false);

  static const struct RtDateTime dtMonthOverflow = {
      .year = 2000,
      .month = 13,
      .day = 1,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtMonthOverflow);
  ck_assert(result == false);

  /* Test day errors */

  static const struct RtDateTime dtZeroDay = {
      .year = 2000,
      .month = 1,
      .day = 0,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtZeroDay);
  ck_assert(result == false);

  static const struct RtDateTime dtFebLeapYear = {
      .year = 2000,
      .month = 2,
      .day = 30,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtFebLeapYear);
  ck_assert(result == false);

  static const struct RtDateTime dtFebYear = {
      .year = 2001,
      .month = 2,
      .day = 29,
      .hour = 12,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtFebYear);
  ck_assert(result == false);

  /* Test hour, minute and second errors */

  static const struct RtDateTime dtHourOverflow = {
      .year = 2000,
      .month = 1,
      .day = 1,
      .hour = 24,
      .minute = 0,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtHourOverflow);
  ck_assert(result == false);

  static const struct RtDateTime dtMinuteOverflow = {
      .year = 2000,
      .month = 1,
      .day = 1,
      .hour = 0,
      .minute = 60,
      .second = 0
  };
  result = rtMakeEpochTime(&timestamp, &dtMinuteOverflow);
  ck_assert(result == false);

  static const struct RtDateTime dtSecondOverflow = {
      .year = 2000,
      .month = 1,
      .day = 1,
      .hour = 0,
      .minute = 0,
      .second = 60
  };
  result = rtMakeEpochTime(&timestamp, &dtSecondOverflow);
  ck_assert(result == false);
}
END_TEST
/*----------------------------------------------------------------------------*/
START_TEST(testTimestampToDateTime)
{
  struct RtDateTime dt;

  /* Test 01.01.1970 12:00:00 */

  static const time64_t timestamp0 = 43759LL;

  rtMakeTime(&dt, timestamp0);
  ck_assert_uint_eq(dt.year, 1970);
  ck_assert_uint_eq(dt.month, 1);
  ck_assert_uint_eq(dt.day, 1);
  ck_assert_uint_eq(dt.hour, 12);
  ck_assert_uint_eq(dt.minute, 9);
  ck_assert_uint_eq(dt.second, 19);

  /* Test 01.02.2200 10:29:59 */

  static const time64_t timestamp1 = 7260834599LL;

  rtMakeTime(&dt, timestamp1);
  ck_assert_uint_eq(dt.year, 2200);
  ck_assert_uint_eq(dt.month, 2);
  ck_assert_uint_eq(dt.day, 1);
  ck_assert_uint_eq(dt.hour, 10);
  ck_assert_uint_eq(dt.minute, 29);
  ck_assert_uint_eq(dt.second, 59);

  /* Test 03.04.1933 12:00:00 */

  static const time64_t timestamp2 = -1159617600LL;

  rtMakeTime(&dt, timestamp2);
  ck_assert_uint_eq(dt.year, 1933);
  ck_assert_uint_eq(dt.month, 4);
  ck_assert_uint_eq(dt.day, 3);
  ck_assert_uint_eq(dt.hour, 12);
  ck_assert_uint_eq(dt.minute, 0);
  ck_assert_uint_eq(dt.second, 0);
}
END_TEST
/*----------------------------------------------------------------------------*/
int main(void)
{
  Suite * const suite = suite_create("Realtime");
  TCase * const testcase = tcase_create("Core");

  tcase_add_test(testcase, testBidirectionalConversion);
  tcase_add_test(testcase, testDateTimeToTimestamp);
  tcase_add_test(testcase, testTimestampToDateTime);
  suite_add_tcase(suite, testcase);

  SRunner * const runner = srunner_create(suite);

  srunner_run_all(runner, CK_NORMAL);
  const int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
