/*
 * semaphore.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/atomic.h>
#include <xcore/os/semaphore.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
[[gnu::weak]] enum Result semInit(struct Semaphore *, int);
[[gnu::weak]] void semDeinit(struct Semaphore *);
[[gnu::weak]] void semPost(struct Semaphore *);
[[gnu::weak]] bool semTryWait(struct Semaphore *, unsigned int);
[[gnu::weak]] int semValue(struct Semaphore *);
[[gnu::weak]] void semWait(struct Semaphore *);
/*----------------------------------------------------------------------------*/
enum Result semInit(struct Semaphore *sem, int value)
{
  sem->data = malloc(sizeof(int));
  if (sem->data == NULL)
    return E_MEMORY;

  *(int *)sem->data = value;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
void semDeinit(struct Semaphore *sem)
{
  free(sem->data);
}
/*----------------------------------------------------------------------------*/
void semPost(struct Semaphore *sem)
{
  atomicFetchAdd((unsigned int *)sem->data, 1);
}
/*----------------------------------------------------------------------------*/
bool semTryWait(struct Semaphore *sem, [[maybe_unused]] unsigned int timeout)
{
  /* Wait is not implemented */
  assert(timeout == 0);

  bool acquired = true;
  int value;

  do
  {
    value = (int)atomicLoad((unsigned int *)sem->data);

    if (value <= 0)
    {
      acquired = false;
      break;
    }
  }
  while (!atomicCompareExchange((unsigned int *)sem->data,
      (unsigned int *)value, (unsigned int)(value - 1)));

  return acquired;
}
/*----------------------------------------------------------------------------*/
int semValue(struct Semaphore *sem)
{
  return (int)atomicLoad((unsigned int *)sem->data);
}
/*----------------------------------------------------------------------------*/
void semWait(struct Semaphore *sem)
{
  int value;

  do
  {
    value = (int)atomicLoad((unsigned int *)sem->data);
  }
  while (value <= 0 || !atomicCompareExchange((unsigned int *)sem->data,
      (unsigned int *)value, (unsigned int)(value - 1)));
}
