/*
 * rtc.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the GNU General Public License v3.0
 */

#ifndef RTC_H_
#define RTC_H_
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <entity.h>
#include <error.h>
/*----------------------------------------------------------------------------*/
typedef int64_t time64_t;
/*----------------------------------------------------------------------------*/
struct RtcTime
{
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};
/*----------------------------------------------------------------------------*/
struct RtcClass
{
  CLASS_HEADER

  enum result (*callback)(void *, void (*)(void *), void *);
  enum result (*setAlarm)(void *, time64_t);
  enum result (*setTime)(void *, time64_t);
  time64_t (*time)(void *);
};
/*----------------------------------------------------------------------------*/
struct Rtc
{
  struct Entity parent;
};
/*----------------------------------------------------------------------------*/
/**
 * Set callback function and its argument.
 * @param timer Pointer to an Rtc object.
 * @param callback Callback function.
 * @param argument Callback function argument.
 * @return @b E_OK on success.
 */
static inline enum result rtcCallback(void *timer, void (*callback)(void *),
    void *argument)
{
  return ((const struct RtcClass *)CLASS(timer))->callback(timer, callback,
      argument);
}
/*----------------------------------------------------------------------------*/
/**
 * Set alarm time.
 * @param timer Pointer to an Rtc object.
 * @param alarmTime Alarm time.
 * @return @b E_OK on success.
 */
static inline enum result rtcSetAlarm(void *timer, time64_t alarmTime)
{
  return ((const struct RtcClass *)CLASS(timer))->setAlarm(timer, alarmTime);
}
/*----------------------------------------------------------------------------*/
/**
 * Set current time.
 * @param timer Pointer to an Rtc object.
 * @param currentTime Current time.
 * @return @b E_OK on success.
 */
static inline enum result rtcSetTime(void *timer, time64_t currentTime)
{
  return ((const struct RtcClass *)CLASS(timer))->setTime(timer, currentTime);
}
/*----------------------------------------------------------------------------*/
/**
 * Get current calendar time.
 * The value returned generally represents the number of seconds
 * since 00:00 hours, Jan 1, 1970 UTC.
 * @param timer Pointer to an Rtc object.
 * @return Current calendar time.
 */
static inline time64_t rtcTime(void *timer)
{
  return ((const struct RtcClass *)CLASS(timer))->time(timer);
}
/*----------------------------------------------------------------------------*/
enum result rtcMakeEpochTime(time64_t *, const struct RtcTime *);
void rtcMakeTime(struct RtcTime *, time64_t);
/*----------------------------------------------------------------------------*/
#endif /* RTC_H_ */
