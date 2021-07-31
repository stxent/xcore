/*
 * xcore/realtime.h
 * Copyright (C) 2015 xent
 * Project is distributed under the terms of the MIT License
 */

#ifndef XCORE_REALTIME_H_
#define XCORE_REALTIME_H_
/*----------------------------------------------------------------------------*/
#include <xcore/entity.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*/
typedef int64_t time64_t;

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

  enum Result (*setAlarm)(void *, time64_t);
  void (*setCallback)(void *, void (*)(void *), void *);
  enum Result (*setTime)(void *, time64_t);
  void (*stop)(void *);
  time64_t (*time)(void *);
};

struct RtClock
{
  struct Entity base;
};
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

/**
 * Set alarm time.
 * @param clock Pointer to an Rtc object.
 * @param alarmTime Alarm time.
 * @return @b E_OK on success.
 */
static inline enum Result rtSetAlarm(void *clock, time64_t alarmTime)
{
  return ((const struct RtClockClass *)CLASS(clock))->setAlarm(clock,
      alarmTime);
}

/**
 * Set callback function and its argument.
 * @param clock Pointer to an RtClock object.
 * @param callback Callback function.
 * @param argument Callback function argument.
 */
static inline void rtSetCallback(void *clock, void (*callback)(void *),
    void *argument)
{
  ((const struct RtClockClass *)CLASS(clock))->setCallback(clock, callback,
      argument);
}

/**
 * Set current time.
 * @param clock Pointer to an RtClock object.
 * @param timestamp Current time in the UNIX time format.
 * @return @b E_OK on success.
 */
static inline enum Result rtSetTime(void *clock, time64_t timestamp)
{
  return ((const struct RtClockClass *)CLASS(clock))->setTime(clock, timestamp);
}

/**
 * Stop the clock.
 * @param clock Pointer to an RtClock object.
 */
static inline void rtStop(void *clock)
{
  ((const struct RtClockClass *)CLASS(clock))->stop(clock);
}

/**
 * Get current calendar date and time.
 * The value returned generally represents the number of seconds
 * since 00:00 hours, Jan 1, 1970 UTC.
 * @param clock Pointer to an RtClock object.
 * @return Current calendar time.
 */
static inline time64_t rtTime(void *clock)
{
  return ((const struct RtClockClass *)CLASS(clock))->time(clock);
}

END_DECLS
/*----------------------------------------------------------------------------*/
BEGIN_DECLS

enum Result rtMakeEpochTime(time64_t *, const struct RtDateTime *);
void rtMakeTime(struct RtDateTime *, time64_t);

END_DECLS
/*----------------------------------------------------------------------------*/
#endif /* XCORE_REALTIME_H_ */
