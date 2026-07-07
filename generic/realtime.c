/*
 * realtime.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/realtime.h>
/*----------------------------------------------------------------------------*/
#define DAYS_PER_400_YEARS  146097
#define DAYS_PER_100_YEARS  36524
#define DAYS_PER_4_YEARS    1461
#define DAYS_PER_YEAR       365
#define DAYS_PER_JAN_FEB    59
#define SECONDS_PER_DAY     86400
#define SECONDS_PER_HOUR    3600

/* Exact number of days between Jan 1, 1 AD and the Unix Epoch on Jan 1, 1970 */
#define OFFSET_UNIX_DAYS    719162
/* Calibration offset to shift the New Year to Mar 1 */
#define OFFSET_JAN_TO_MAR   428
/*----------------------------------------------------------------------------*/
static const uint8_t monthLengthMap[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
/*----------------------------------------------------------------------------*/
/**
 * @brief Converts a date and time structure to a UNIX timestamp.
 * @param[out] result Pointer to the output value.
 * @param[in] datetime Date and time structure to be converted.
 * @return @b true if successful, @b false if the date and time structure
 * contains invalid values.
 */
bool rtMakeEpochTime(time64_t *result, const struct RtDateTime *datetime)
{
  if (!datetime->day || !datetime->month || datetime->month > 12)
    return false;
  if (datetime->hour >= 24 || datetime->minute >= 60 || datetime->second >= 60)
    return false;

  int month = datetime->month;
  int year = datetime->year;

  if (month == 2 && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
  {
    if (datetime->day > monthLengthMap[month - 1] + 1)
      return false;
  }
  else
  {
    if (datetime->day > monthLengthMap[month - 1])
      return false;
  }

  /* Shift the calendar so the year begins in March */
  if (month <= 2)
  {
    year -= 1;
    month += 12;
  }

  /* Count eras (400-year cycles) since Year 0 */
  const long zeroBasisDays = (long)year * DAYS_PER_YEAR
      + (year / 4) - (year / 100) + (year / 400)
      + (((DAYS_PER_YEAR - DAYS_PER_JAN_FEB) * (month + 1)) / 10)
      + ((long)datetime->day - OFFSET_JAN_TO_MAR);

  /* Subtract the exact number of days between epochs */
  const long days = zeroBasisDays - OFFSET_UNIX_DAYS;

  /*
   * Add the number of days from the current month, each hour,
   * minute and second from the current day.
   */
  const time64_t seconds = (days - 1) * SECONDS_PER_DAY + datetime->second
      + datetime->minute * 60 + datetime->hour * SECONDS_PER_HOUR;

  *result = seconds;
  return true;
}
/*----------------------------------------------------------------------------*/
/**
 * @brief Converts a UNIX timestamp to a date and time structure.
 * @param[out] datetime Pointer to the output date and time structure.
 * @param[in] timestamp UNIX timestamp to be converted.
*/
void rtMakeTime(struct RtDateTime *datetime, time64_t timestamp)
{
  /* Howard Hinnant's date conversion algorithm */

  long unixBaseDays = timestamp / SECONDS_PER_DAY;
  int seconds = timestamp - unixBaseDays * SECONDS_PER_DAY;

  if (seconds < 0)
  {
    seconds += SECONDS_PER_DAY;
    unixBaseDays -= 1;
  }

  /* Extract time components */
  datetime->hour = seconds / SECONDS_PER_HOUR;
  seconds %= SECONDS_PER_HOUR;
  datetime->minute = seconds / 60;
  datetime->second = seconds % 60;

  const long zeroBasisDays = unixBaseDays + OFFSET_UNIX_DAYS;

  /* 400-year era */
  const int era = zeroBasisDays / DAYS_PER_400_YEARS;
  const int dayOfEra = zeroBasisDays % DAYS_PER_400_YEARS; /* 0..146096 */

  /* 100-year block within era */
  int century = dayOfEra / DAYS_PER_100_YEARS;
  int dayOfCentury = dayOfEra % DAYS_PER_100_YEARS; /* 0..36523 */

  if (century == 4)
  {
    century = 3;
    dayOfCentury = DAYS_PER_100_YEARS;
  }

  /* 4-year block within century */
  const int block = dayOfCentury / DAYS_PER_4_YEARS;
  const int dayOfBlock = dayOfCentury % DAYS_PER_4_YEARS; /* 0..1460 */

  /* Year within 4-year block */
  int yearOfBlock = dayOfBlock / DAYS_PER_YEAR;
  int dayOfYear = dayOfBlock % DAYS_PER_YEAR; /* 0..364 */

  if (yearOfBlock == 4)
  {
    yearOfBlock = 3;
    dayOfYear = DAYS_PER_YEAR;
  }

  /* Compute full year (AD, year 1 based) */
  const int year = 1 + yearOfBlock + block * 4 + century * 100 + era * 400;
  int daysInJanFeb = DAYS_PER_JAN_FEB;

  if ((year % 4 == 0) && (year % 100 != 0 || year % 400 == 0))
    ++daysInJanFeb;

  if (dayOfYear >= daysInJanFeb)
  {
    /* Date is March 1st or later: shift backwards */
    dayOfYear -= daysInJanFeb;
  }
  else
  {
    /* Date is Jan or Feb: shift forwards */
    dayOfYear += DAYS_PER_YEAR - DAYS_PER_JAN_FEB;
  }

  const int monthIndex = (5 * dayOfYear + 2) / 153; /* 0..11 (Mar = 0) */
  const int day = dayOfYear - (153 * monthIndex + 2) / 5 + 1; /* 1..31 */
  const int month = monthIndex + (monthIndex < 10 ? 3 : -9); /* 1..12 */

  datetime->month = month;
  datetime->day = day;
  datetime->year = year;
}
