/*
 * xcore/realtime.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef XCORE_REALTIME_H_
#define XCORE_REALTIME_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <xcore/entity.h>
/*----------------------------------------------------------------------------*/
typedef int64_t time64_t;
/*----------------------------------------------------------------------------*/
struct RtDateTime
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};
/*----------------------------------------------------------------------------*/
struct RtClockClass
{
  CLASS_HEADER

  enum result (*callback)(void *, void (*)(void *), void *);
  enum result (*setAlarm)(void *, time64_t);
  enum result (*setTime)(void *, time64_t);
  time64_t (*time)(void *);
};
/*----------------------------------------------------------------------------*/
struct RtClock
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
/**
 * Set callback function and its argument.
 * @param clock Pointer to an RtClock object.
 * @param callback Callback function.
 * @param argument Callback function argument.
 * @return @b E_OK on success.
 */
static inline enum result rtCallback(void *clock, void (*callback)(void *),
    void *argument)
{
  return ((const struct RtClockClass *)CLASS(clock))->callback(clock, callback,
      argument);
}
/*----------------------------------------------------------------------------*/
/**
 * Set alarm time.
 * @param clock Pointer to an Rtc object.
 * @param alarmTime Alarm time.
 * @return @b E_OK on success.
 */
static inline enum result rtSetAlarm(void *clock, time64_t alarmTime)
{
  return ((const struct RtClockClass *)CLASS(clock))->setAlarm(clock,
      alarmTime);
}
/*----------------------------------------------------------------------------*/
/**
 * Set current time.
 * @param clock Pointer to an RtClock object.
 * @param currentTime Current time.
 * @return @b E_OK on success.
 */
static inline enum result rtSetTime(void *clock, time64_t currentTime)
{
  return ((const struct RtClockClass *)CLASS(clock))->setTime(clock,
      currentTime);
}
/*----------------------------------------------------------------------------*/
/**
 * Get current calendar time.
 * The value returned generally represents the number of seconds
 * since 00:00 hours, Jan 1, 1970 UTC.
 * @param clock Pointer to an RtClock object.
 * @return Current calendar time.
 */
static inline time64_t rtTime(void *clock)
{
  return ((const struct RtClockClass *)CLASS(clock))->time(clock);
}
/*----------------------------------------------------------------------------*/
enum result rtMakeEpochTime(time64_t *, const struct RtDateTime *);
void rtMakeTime(struct RtDateTime *, time64_t);
/*----------------------------------------------------------------------------*/
#endif /* XCORE_REALTIME_H_ */
