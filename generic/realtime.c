/*
 * realtime.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/realtime.h>
/*----------------------------------------------------------------------------*/
#define SECONDS_PER_DAY   86400
#define SECONDS_PER_HOUR  3600
#define START_YEAR        1970
#define OFFSET_YEARS      2
#define OFFSET_SECONDS    (OFFSET_YEARS * 365 * SECONDS_PER_DAY)
/*----------------------------------------------------------------------------*/
static const uint8_t monthLengthMap[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
static const uint16_t yearLengthMap[] = {
    366, 365, 365, 365
};
/*----------------------------------------------------------------------------*/
/**
 * Convert a date and time structure to a UNIX timestamp.
 * @param result Pointer to an output value.
 * @param datetime Date and time structure to convert.
 * @return @b E_OK on success, @b E_VALUE when a date and time structure
 * contains incorrect values.
 */
enum Result rtMakeEpochTime(time64_t *result,
    const struct RtDateTime *datetime)
{
  if (!datetime->day || !datetime->month || datetime->month > 12)
    return E_VALUE;
  if (datetime->hour >= 24 || datetime->minute >= 60 || datetime->second >= 60)
    return E_VALUE;

  unsigned int month = datetime->month - 1;
  const bool leapYear = datetime->year % 4 == 0;

  if (leapYear && month == 1)
  {
    if (datetime->day > monthLengthMap[month] + 1)
      return E_VALUE;
  }
  else
  {
    if (datetime->day > monthLengthMap[month])
      return E_VALUE;
  }

  /* Stores how many seconds have passed from 01.01.1970, 00:00:00 */
  time64_t seconds = 0;

  /* If the current year is a leap one than add one day or 86400 seconds */
  if (leapYear && month > 1)
    seconds += SECONDS_PER_DAY;

  /* Sum the days from January to the current month */
  while (month)
    seconds += monthLengthMap[--month] * SECONDS_PER_DAY;

  /* Add the number of days from each year with leap years */
  seconds += ((datetime->year - START_YEAR) * 365
      + ((datetime->year - START_YEAR + 1) / 4)) * SECONDS_PER_DAY;

  /*
   * Add the number of days from the current month, each hour,
   * minute and second from the current day.
   */
  seconds += (datetime->day - 1) * SECONDS_PER_DAY + datetime->second
      + datetime->minute * 60 + datetime->hour * SECONDS_PER_HOUR;

  *result = seconds;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
/**
 * Convert a UNIX timestamp to a date and time structure.
 * @param datetime Pointer to an output date and time structure.
 * @param timestamp UNIX timestamp to convert.
*/
void rtMakeTime(struct RtDateTime *datetime, time64_t timestamp)
{
  /* TODO Add handling of negative times and years after 2100 */
  const uint64_t seconds = timestamp > 0 ? timestamp : -timestamp;
  const uint32_t daySeconds = seconds % SECONDS_PER_DAY;

  datetime->second = daySeconds % 60;
  datetime->minute = (daySeconds % 3600) / 60;
  datetime->hour = daySeconds / 3600;

  uint32_t days = seconds / SECONDS_PER_DAY;
  uint32_t years;

  if (seconds > OFFSET_SECONDS)
  {
    const uint64_t offset = seconds - OFFSET_SECONDS;
    const uint32_t estimatedYears = (offset / (SECONDS_PER_DAY / 100))
        / (365 * 100 + 25);
    const uint32_t leapCycles = estimatedYears / 4;

    years = leapCycles * 4 + OFFSET_YEARS;
    days -= years * 365 + leapCycles;

    for (uint8_t number = 0; days >= yearLengthMap[number]; ++number)
    {
      days -= yearLengthMap[number];
      ++years;
    }
  }
  else
  {
    years = days / 365;
    days -= years * 365;
  }

  datetime->year = START_YEAR + years;

  const uint8_t offset = !(datetime->year % 4) && days >= 60 ? 1 : 0;
  uint8_t month = 0;

  days -= offset;
  while (days >= monthLengthMap[month])
    days -= monthLengthMap[month++];

  if (month == 1)
    days += offset;

  datetime->day = days + 1;
  datetime->month = month + 1;
}
