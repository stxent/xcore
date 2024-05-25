/*
 * mutex.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/mutex.h>
#include <time.h>
/*----------------------------------------------------------------------------*/
[[gnu::weak]] enum Result mutexInit(struct Mutex *);
/*----------------------------------------------------------------------------*/
enum Result mutexInit(struct Mutex *mutex)
{
  return pthread_mutex_init(&mutex->handle, NULL) == 0 ? E_OK : E_ERROR;
}
/*----------------------------------------------------------------------------*/
void mutexDeinit(struct Mutex *mutex)
{
  pthread_mutex_destroy(&mutex->handle);
}
/*----------------------------------------------------------------------------*/
bool mutexTryLock(struct Mutex *mutex, unsigned int interval)
{
  int result;

  if (interval)
  {
    struct timespec timestamp;

    clock_gettime(CLOCK_REALTIME, &timestamp);
    timestamp.tv_nsec += (interval % 1000) * 1000000;
    timestamp.tv_sec += interval / 1000;
    timestamp.tv_sec += timestamp.tv_nsec >= 1000000000 ? 1 : 0;
    timestamp.tv_nsec -= timestamp.tv_nsec >= 1000000000 ? 1000000000 : 0;

    result = pthread_mutex_timedlock(&mutex->handle, &timestamp);
  }
  else
  {
    result = pthread_mutex_trylock(&mutex->handle);
  }

  return result == 0;
}
