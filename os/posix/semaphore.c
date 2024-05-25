/*
 * semaphore.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/semaphore.h>
#include <time.h>
/*----------------------------------------------------------------------------*/
[[gnu::weak]] enum Result semInit(struct Semaphore *, int);
/*----------------------------------------------------------------------------*/
enum Result semInit(struct Semaphore *sem, int value)
{
  return sem_init(&sem->handle, 0, value) == 0 ? E_OK : E_ERROR;
}
/*----------------------------------------------------------------------------*/
void semDeinit(struct Semaphore *sem)
{
  sem_destroy(&sem->handle);
}
/*----------------------------------------------------------------------------*/
bool semTryWait(struct Semaphore *semaphore, unsigned int timeout)
{
  int result;

  if (timeout)
  {
    struct timespec timestamp;

    clock_gettime(CLOCK_REALTIME, &timestamp);
    timestamp.tv_nsec += (timeout % 1000) * 1000000;
    timestamp.tv_sec += timeout / 1000;
    timestamp.tv_sec += timestamp.tv_nsec >= 1000000000 ? 1 : 0;
    timestamp.tv_nsec -= timestamp.tv_nsec >= 1000000000 ? 1000000000 : 0;

    result = sem_timedwait(&semaphore->handle, &timestamp);
  }
  else
  {
    result = sem_trywait(&semaphore->handle);
  }

  return result == 0;
}
