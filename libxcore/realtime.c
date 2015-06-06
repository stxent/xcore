/*
 * realtime.c
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#include "../include/realtime.h"
/*----------------------------------------------------------------------------*/
#define SECONDS_PER_DAY   86400
#define SECONDS_PER_HOUR  3600
#define START_YEAR        1970
#define OFFSET_YEARS      2
#define OFFSET_SECONDS    (OFFSET_YEARS * 365 * SECONDS_PER_DAY)
/*----------------------------------------------------------------------------*/
static const uint8_t monthLengths[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};
static const uint16_t yearLengths[] = {
    366, 365, 365, 365
};
/*----------------------------------------------------------------------------*/
enum result rtMakeEpochTime(time64_t *result, const struct RtDateTime *timestamp)
{
  if (!timestamp->month || timestamp->month > 12)
    return E_VALUE;

  /* Stores how many seconds have passed from 01.01.1970, 00:00:00 */
  time64_t seconds = 0;

  /* If the current year is a leap one than add one day or 86400 seconds */
  if (!(timestamp->year % 4) && (timestamp->month > 2))
    seconds += SECONDS_PER_DAY;

  uint8_t month = timestamp->month - 1;

  /* Sum the days from January to the current month */
  while (month)
    seconds += monthLengths[--month] * SECONDS_PER_DAY;

  /* Add the number of days from each year with leap years */
  seconds += ((timestamp->year - START_YEAR) * 365
      + ((timestamp->year - START_YEAR + 1) / 4)) * SECONDS_PER_DAY;

  /*
   * Add the number of days from the current month, each hour,
   * minute and second from the current day.
   */
  seconds += (timestamp->day - 1) * SECONDS_PER_DAY + timestamp->second
      + timestamp->minute * 60 + timestamp->hour * SECONDS_PER_HOUR;

  *result = seconds;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
void rtMakeTime(struct RtDateTime *timestamp, time64_t epochTime)
{
  /* TODO Add handling of negative times and years after 2100 */
  const uint64_t seconds = epochTime > 0 ? epochTime : -epochTime;
  const uint32_t dayclock = seconds % SECONDS_PER_DAY;

  timestamp->second = dayclock % 60;
  timestamp->minute = (dayclock % 3600) / 60;
  timestamp->hour = dayclock / 3600;

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

    for (uint8_t number = 0; days >= yearLengths[number]; ++number)
    {
      days -= yearLengths[number];
      ++years;
    }
  }
  else
  {
    years = days / 365;
    days -= years * 365;
  }

  timestamp->year = START_YEAR + years;

  const uint8_t offset = !(timestamp->year % 4) && days >= 60 ? 1 : 0;
  uint8_t month = 0;

  days -= offset;
  while (days >= monthLengths[month])
    days -= monthLengths[month++];

  if (month == 1)
    days += offset;

  timestamp->day = days + 1;
  timestamp->month = month + 1;
}
