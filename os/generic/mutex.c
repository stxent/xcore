/*
 * mutex.c
 * Copyright (C) 2024 xent
 * Project is distributed under the terms of the MIT License
 */

#include <xcore/os/mutex.h>
#include <stddef.h>
#include <stdlib.h>
/*----------------------------------------------------------------------------*/
[[gnu::weak]] enum Result mutexInit(struct Mutex *);
[[gnu::weak]] void mutexDeinit(struct Mutex *);
[[gnu::weak]] void mutexLock(struct Mutex *);
[[gnu::weak]] bool mutexTryLock(struct Mutex *, unsigned int);
[[gnu::weak]] void mutexUnlock(struct Mutex *);
/*----------------------------------------------------------------------------*/
enum Result mutexInit(struct Mutex *mutex)
{
  mutex->data = NULL;
  return E_OK;
}
/*----------------------------------------------------------------------------*/
void mutexDeinit(struct Mutex *)
{
}
/*----------------------------------------------------------------------------*/
void mutexLock(struct Mutex *)
{
}
/*----------------------------------------------------------------------------*/
bool mutexTryLock(struct Mutex *, unsigned int)
{
  return true;
}
/*----------------------------------------------------------------------------*/
void mutexUnlock(struct Mutex *)
{
}
